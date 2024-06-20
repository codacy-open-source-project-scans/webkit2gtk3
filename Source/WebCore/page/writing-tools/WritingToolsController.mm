/*
 * Copyright (C) 2024 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#if ENABLE(WRITING_TOOLS)

#include "config.h"
#include "WritingToolsController.h"

#include "Chrome.h"
#include "ChromeClient.h"
#include "CompositeEditCommand.h"
#include "DocumentInlines.h"
#include "DocumentMarkerController.h"
#include "Editor.h"
#include "FocusController.h"
#include "FrameSelection.h"
#include "GeometryUtilities.h"
#include "HTMLConverter.h"
#include "Logging.h"
#include "NodeRenderStyle.h"
#include "RenderedDocumentMarker.h"
#include "TextIterator.h"
#include "VisibleUnits.h"
#include "WebContentReader.h"

namespace WebCore {

#pragma mark - Static utility helper methods.

static constexpr auto defaultTextIteratorBehaviors = TextIteratorBehaviors {
    TextIteratorBehavior::EmitsObjectReplacementCharactersForImages,
#if ENABLE(ATTACHMENT_ELEMENT)
    TextIteratorBehavior::EmitsObjectReplacementCharactersForAttachments
#endif
};

CharacterRange WritingToolsController::characterRange(const SimpleRange& scope, const SimpleRange& range)
{
    return WebCore::characterRange(scope, range, defaultTextIteratorBehaviors);
}

uint64_t WritingToolsController::characterCount(const SimpleRange& range)
{
    return WebCore::characterCount(range, defaultTextIteratorBehaviors);
}

SimpleRange WritingToolsController::resolveCharacterRange(const SimpleRange& scope, CharacterRange range)
{
    return WebCore::resolveCharacterRange(scope, range, defaultTextIteratorBehaviors);
}

String WritingToolsController::plainText(const SimpleRange& range)
{
    return WebCore::plainText(range, defaultTextIteratorBehaviors);
}

#pragma mark - WritingToolsController implementation.

WritingToolsController::WritingToolsController(Page& page)
    : m_page(page)
{
}

static std::optional<SimpleRange> contextRangeForDocument(const Document& document)
{
    auto selection = document.selection().selection();

    if (selection.isRange()) {
        auto startOfFirstParagraph = startOfParagraph(selection.start());
        auto endOfLastParagraph = endOfParagraph(selection.end());

        auto paragraphRange = makeSimpleRange(startOfFirstParagraph, endOfLastParagraph);

        if (paragraphRange && hasAnyPlainText(*paragraphRange, defaultTextIteratorBehaviors))
            return paragraphRange;
    }

    auto startOfFirstEditableContent = startOfEditableContent(selection.start());
    auto endOfLastEditableContent = endOfEditableContent(selection.end());

    auto editableContentRange = makeSimpleRange(startOfFirstEditableContent, endOfLastEditableContent);
    if (editableContentRange && hasAnyPlainText(*editableContentRange, defaultTextIteratorBehaviors))
        return editableContentRange;

    return selection.firstRange();
}

void WritingToolsController::willBeginTextReplacementSession(const std::optional<WritingTools::Session>& session, CompletionHandler<void(const Vector<WritingTools::Context>&)>&& completionHandler)
{
    RELEASE_LOG(UnifiedTextReplacement, "WritingToolsController::willBeginTextReplacementSession (%s)", session ? session->identifier.toString().utf8().data() : "");

    RefPtr document = this->document();
    if (!document) {
        ASSERT_NOT_REACHED();
        completionHandler({ });
        return;
    }

    auto contextRange = contextRangeForDocument(*document);

    if (!contextRange) {
        RELEASE_LOG(UnifiedTextReplacement, "WritingToolsController::willBeginTextReplacementSession (%s) => no context range", session ? session->identifier.toString().utf8().data() : "");
        completionHandler({ });
        return;
    }

    auto selectedTextRange = document->selection().selection().firstRange();

    if (session && session->compositionType == WritingTools::Session::CompositionType::SmartReply) {
        ASSERT(session->type == WritingTools::Session::Type::Composition);

        auto liveRange = createLiveRange(*selectedTextRange);
        m_states.set(session->identifier, CompositionState { liveRange, { } });

        completionHandler({ { WTF::UUID { 0 }, AttributedString::fromNSAttributedString(adoptNS([[NSAttributedString alloc] initWithString:@""])), CharacterRange { 0, 0 } } });
        return;
    }

    auto attributedStringFromRange = editingAttributedString(*contextRange, { IncludedElement::Images, IncludedElement::Attachments });
    auto selectedTextCharacterRange = characterRange(*contextRange, *selectedTextRange);

    if (attributedStringFromRange.string.isEmpty())
        RELEASE_LOG(UnifiedTextReplacement, "WritingToolsController::willBeginTextReplacementSession (%s) => attributed string is empty", session ? session->identifier.toString().utf8().data() : "");

    if (!session) {
        completionHandler({ { WTF::UUID { 0 }, attributedStringFromRange, selectedTextCharacterRange } });
        return;
    }

    ASSERT(!m_states.contains(session->identifier));

    auto liveRange = createLiveRange(*contextRange);

    switch (session->type) {
    case WritingTools::Session::Type::Proofreading:
        m_states.set(session->identifier, ProofreadingState { liveRange, 0 });
        break;

    case WritingTools::Session::Type::Composition:
        m_states.set(session->identifier, CompositionState { liveRange, { } });
        break;
    }

    auto attributedStringCharacterCount = attributedStringFromRange.string.length();
    auto contextRangeCharacterCount = characterCount(*contextRange);

    // Postcondition: the selected text character range must be a valid range within the
    // attributed string formed by the context range; the length of the entire context range
    // being equal to the length of the attributed string implies the range is valid.
    if (UNLIKELY(attributedStringCharacterCount != contextRangeCharacterCount)) {
        RELEASE_LOG_ERROR(UnifiedTextReplacement, "WritingToolsController::willBeginTextReplacementSession (%s) => attributed string length (%u) != context range length (%llu)", session->identifier.toString().utf8().data(), attributedStringCharacterCount, contextRangeCharacterCount);
        ASSERT_NOT_REACHED();
        completionHandler({ });
        return;
    }

    completionHandler({ { WTF::UUID { 0 }, attributedStringFromRange, selectedTextCharacterRange } });
}

void WritingToolsController::didBeginTextReplacementSession(const WritingTools::Session& session, const Vector<WritingTools::Context>& contexts)
{
    RELEASE_LOG(UnifiedTextReplacement, "WritingToolsController::didBeginTextReplacementSession (%s) [received contexts: %zu]", session.identifier.toString().utf8().data(), contexts.size());
}

void WritingToolsController::textReplacementSessionDidReceiveReplacements(const WritingTools::Session& session, const Vector<WritingTools::TextSuggestion>& replacements, const WritingTools::Context& context, bool finished)
{
    RELEASE_LOG(UnifiedTextReplacement, "WritingToolsController::textReplacementSessionDidReceiveReplacements (%s) [received replacements: %zu, finished: %d]", session.identifier.toString().utf8().data(), replacements.size(), finished);

    RefPtr document = this->document();
    if (!document) {
        ASSERT_NOT_REACHED();
        return;
    }

    // FIXME: Text indicator styles are not used within this method, so is this still needed?
    m_page->chrome().client().removeTextAnimationForID(session.identifier);

    document->selection().clear();

    CheckedPtr state = stateForSession<WritingTools::Session::Type::Proofreading>(session);
    if (!state) {
        ASSERT_NOT_REACHED();
        return;
    }

    auto sessionRange = makeSimpleRange(state->contextRange);

    // The tracking of the additional replacement location offset needs to be scoped to a particular instance
    // of this class, instead of just this function, because the function may need to be called multiple times.
    // This ensures that subsequent calls of this function should effectively be treated as just more iterations
    // of the following for-loop.

    for (const auto& replacementData : replacements) {
        auto locationWithOffset = replacementData.originalRange.location + state->replacementLocationOffset;

        auto resolvedRange = resolveCharacterRange(sessionRange, { locationWithOffset, replacementData.originalRange.length });

        replaceContentsOfRangeInSession(*state, resolvedRange, replacementData.replacement);

        sessionRange = makeSimpleRange(state->contextRange);

        auto newRangeWithOffset = CharacterRange { locationWithOffset, replacementData.replacement.length() };
        auto newResolvedRange = resolveCharacterRange(sessionRange, newRangeWithOffset);

        auto originalString = [context.attributedText.nsAttributedString() attributedSubstringFromRange:replacementData.originalRange];

        auto markerData = DocumentMarker::UnifiedTextReplacementData { originalString.string, replacementData.identifier, session.identifier, DocumentMarker::UnifiedTextReplacementData::State::Pending };
        addMarker(newResolvedRange, DocumentMarker::Type::UnifiedTextReplacement, markerData);

        state->replacementLocationOffset += static_cast<int>(replacementData.replacement.length()) - static_cast<int>(replacementData.originalRange.length);
    }

    if (finished)
        document->selection().setSelection({ sessionRange });
}

void WritingToolsController::textReplacementSessionDidUpdateStateForReplacement(const WritingTools::Session& session, WritingTools::TextSuggestion::State newTextSuggestionState, const WritingTools::TextSuggestion& textSuggestion, const WritingTools::Context&)
{
    RELEASE_LOG(UnifiedTextReplacement, "WritingToolsController::textReplacementSessionDidUpdateStateForReplacement (%s) [new state: %hhu, replacement: %s]", session.identifier.toString().utf8().data(), enumToUnderlyingType(newTextSuggestionState), textSuggestion.identifier.toString().utf8().data());

    RefPtr document = this->document();
    if (!document) {
        ASSERT_NOT_REACHED();
        return;
    }

    CheckedPtr state = stateForSession<WritingTools::Session::Type::Proofreading>(session);
    if (!state) {
        ASSERT_NOT_REACHED();
        return;
    }

    auto sessionRange = makeSimpleRange(state->contextRange);

    auto nodeAndMarker = findTextSuggestionMarkerByID(sessionRange, textSuggestion.identifier);
    if (!nodeAndMarker)
        return;

    auto& [node, marker] = *nodeAndMarker;

    auto rangeToReplace = makeSimpleRange(node, marker);

    switch (newTextSuggestionState) {
    case WritingTools::TextSuggestion::State::Reviewing: {
        document->selection().setSelection({ rangeToReplace });
        document->selection().revealSelection();

        auto rect = document->view()->contentsToRootView(unionRect(RenderObject::absoluteTextRects(rangeToReplace)));

        if (CheckedPtr renderStyle = node.renderStyle()) {
            const auto& font = renderStyle->fontCascade();
            auto [_, height] = DocumentMarkerController::markerYPositionAndHeightForFont(font);

            rect.setY(rect.y() + std::round(height / 2.0));
        }

        m_page->chrome().client().textReplacementSessionShowInformationForReplacementWithIDRelativeToRect(session.identifier, textSuggestion.identifier, rect);

        return;
    }

    case WritingTools::TextSuggestion::State::Rejected: {
        auto data = std::get<DocumentMarker::UnifiedTextReplacementData>(marker.data());

        auto offsetRange = OffsetRange { marker.startOffset(), marker.endOffset() };
        document->markers().removeMarkers(node, offsetRange, { DocumentMarker::Type::UnifiedTextReplacement });

        replaceContentsOfRangeInSession(*state, rangeToReplace, data.originalText);

        return;
    }

    default:
        return;
    }
}

void WritingToolsController::textReplacementSessionDidReceiveTextWithReplacementRange(const WritingTools::Session& session, const AttributedString& attributedText, const CharacterRange& range, const WritingTools::Context& context, bool finished)
{
    auto hasAttributes = attributedText.attributes.containsIf([](const auto& rangeAndAttributeValues) {
        return !rangeAndAttributeValues.second.isEmpty();
    });

    RELEASE_LOG(UnifiedTextReplacement, "WritingToolsController::textReplacementSessionDidReceiveTextWithReplacementRange (%s) [range: %llu, %llu; has attributes: %d; finished: %d]", session.identifier.toString().utf8().data(), range.location, range.length, hasAttributes, finished);

    auto contextTextCharacterCount = context.attributedText.string.length();

    // Precondition: the range is always relative to the context's attributed text, so by definition it must
    // be strictly less than the length of the attributed string.
    if (UNLIKELY(contextTextCharacterCount < range.location + range.length)) {
        RELEASE_LOG_ERROR(UnifiedTextReplacement, "WritingToolsController::textReplacementSessionDidReceiveTextWithReplacementRange (%s) => trying to replace a range larger than the context range (context range length: %u, range.location %llu, range.length %llu)", session.identifier.toString().utf8().data(), contextTextCharacterCount, range.location, range.length);
        ASSERT_NOT_REACHED();
        return;
    }

    RefPtr document = this->document();
    if (!document) {
        ASSERT_NOT_REACHED();
        return;
    }

    CheckedPtr state = stateForSession<WritingTools::Session::Type::Composition>(session);
    if (!state) {
        ASSERT_NOT_REACHED();
        return;
    }

    m_page->chrome().client().removeTextAnimationForID(session.identifier);

    document->selection().clear();

    auto sessionRange = makeSimpleRange(state->contextRange);
    auto sessionRangeCharacterCount = characterCount(sessionRange);

    if (UNLIKELY(range.length + sessionRangeCharacterCount < contextTextCharacterCount)) {
        RELEASE_LOG_ERROR(UnifiedTextReplacement, "WritingToolsController::textReplacementSessionDidReceiveTextWithReplacementRange (%s) => the range offset by the character count delta must have a non-negative size (context range length: %u, range.length %llu, session length: %llu)", session.identifier.toString().utf8().data(), contextTextCharacterCount, range.length, sessionRangeCharacterCount);
        ASSERT_NOT_REACHED();
        return;
    }

    // The character count delta is `sessionRangeCharacterCount - contextTextCharacterCount`;
    // the above check ensures that the full range length expression will never underflow.

    auto characterCountDelta = sessionRangeCharacterCount - contextTextCharacterCount;
    auto adjustedCharacterRange = CharacterRange { range.location, range.length + characterCountDelta };
    auto resolvedRange = resolveCharacterRange(sessionRange, adjustedCharacterRange);

    RefPtr fragment = createFragment(*document->frame(), attributedText.nsAttributedString().get(), { FragmentCreationOptions::NoInterchangeNewlines, FragmentCreationOptions::SanitizeMarkup });
    if (!fragment) {
        ASSERT_NOT_REACHED();
        return;
    }

    m_page->chrome().client().addSourceTextAnimation(session.identifier, range);

    replaceContentsOfRangeInSession(*state, resolvedRange, WTFMove(fragment), hasAttributes ? MatchStyle::No : MatchStyle::Yes);

    m_page->chrome().client().addDestinationTextAnimation(session.identifier, adjustedCharacterRange);
}

template<>
void WritingToolsController::textReplacementSessionDidReceiveEditAction<WritingTools::Session::Type::Proofreading>(const WritingTools::Session& session, WritingTools::Action action)
{
    RELEASE_LOG(UnifiedTextReplacement, "WritingToolsController::textReplacementSessionDidReceiveEditAction<PlainText> (%s) [action: %hhu]", session.identifier.toString().utf8().data(), enumToUnderlyingType(action));

    RefPtr document = this->document();
    if (!document) {
        ASSERT_NOT_REACHED();
        return;
    }

    CheckedPtr state = stateForSession<WritingTools::Session::Type::Proofreading>(session);
    if (!state) {
        ASSERT_NOT_REACHED();
        return;
    }

    auto sessionRange = makeSimpleRange(state->contextRange);

    auto& markers = document->markers();

    markers.forEach<DocumentMarkerController::IterationDirection::Backwards>(sessionRange, { DocumentMarker::Type::UnifiedTextReplacement }, [&](auto& node, auto& marker) {
        auto rangeToReplace = makeSimpleRange(node, marker);

        auto currentText = plainText(rangeToReplace);

        auto oldData = std::get<DocumentMarker::UnifiedTextReplacementData>(marker.data());
        auto previousText = oldData.originalText;
        auto offsetRange = OffsetRange { marker.startOffset(), marker.endOffset() };

        markers.removeMarkers(node, offsetRange, { DocumentMarker::Type::UnifiedTextReplacement });

        auto newState = [&] {
            switch (action) {
            case WritingTools::Action::ShowOriginal:
                return DocumentMarker::UnifiedTextReplacementData::State::Reverted;

            case WritingTools::Action::ShowRewritten:
                return DocumentMarker::UnifiedTextReplacementData::State::Pending;

            default:
                ASSERT_NOT_REACHED();
                return DocumentMarker::UnifiedTextReplacementData::State::Pending;
            }
        }();

        replaceContentsOfRangeInSession(*state, rangeToReplace, previousText);

        auto newData = DocumentMarker::UnifiedTextReplacementData { currentText, oldData.replacementID, session.identifier, newState };
        auto newOffsetRange = OffsetRange { offsetRange.start, offsetRange.end + previousText.length() - currentText.length() };

        markers.addMarker(node, DocumentMarker { DocumentMarker::Type::UnifiedTextReplacement, newOffsetRange, WTFMove(newData) });

        return false;
    });
}

template<>
void WritingToolsController::textReplacementSessionDidReceiveEditAction<WritingTools::Session::Type::Composition>(const WritingTools::Session& session, WritingTools::Action action)
{
    RELEASE_LOG(UnifiedTextReplacement, "WritingToolsController::textReplacementSessionDidReceiveEditAction<RichText> (%s) [action: %hhu]", session.identifier.toString().utf8().data(), enumToUnderlyingType(action));

    CheckedPtr state = stateForSession<WritingTools::Session::Type::Composition>(session);
    if (!state) {
        ASSERT_NOT_REACHED();
        return;
    }

    switch (action) {
    case WritingTools::Action::ShowOriginal: {
        for (auto it = state->commands.rbegin(); it != state->commands.rend(); it++)
            (*it)->ensureComposition().unapply();

        break;
    }

    case WritingTools::Action::ShowRewritten: {
        for (auto it = state->commands.begin(); it != state->commands.end(); it++)
            (*it)->ensureComposition().reapply();

        break;
    }

    case WritingTools::Action::Restart: {
        for (auto it = state->commands.rbegin(); it != state->commands.rend(); it++)
            (*it)->ensureComposition().unapply();

        state->commands.clear();

        break;
    }
    }
}

void WritingToolsController::textReplacementSessionDidReceiveEditAction(const WritingTools::Session& session, WritingTools::Action action)
{
    RELEASE_LOG(UnifiedTextReplacement, "WritingToolsController::textReplacementSessionDidReceiveEditAction (%s) [action: %hhu]", session.identifier.toString().utf8().data(), enumToUnderlyingType(action));

    switch (session.type) {
    case WritingTools::Session::Type::Proofreading: {
        textReplacementSessionDidReceiveEditAction<WritingTools::Session::Type::Proofreading>(session, action);
        break;
    }

    case WritingTools::Session::Type::Composition: {
        textReplacementSessionDidReceiveEditAction<WritingTools::Session::Type::Composition>(session, action);
        break;
    }
    }
}

template<>
void WritingToolsController::didEndTextReplacementSession<WritingTools::Session::Type::Proofreading>(const WritingTools::Session& session, bool accepted)
{
    RefPtr document = this->document();

    CheckedPtr state = stateForSession<WritingTools::Session::Type::Proofreading>(session);
    if (!state) {
        ASSERT_NOT_REACHED();
        return;
    }

    auto sessionRange = makeSimpleRange(state->contextRange);

    auto& markers = document->markers();

    markers.forEach<DocumentMarkerController::IterationDirection::Backwards>(sessionRange, { DocumentMarker::Type::UnifiedTextReplacement }, [&](auto& node, auto& marker) {
        auto data = std::get<DocumentMarker::UnifiedTextReplacementData>(marker.data());

        auto offsetRange = OffsetRange { marker.startOffset(), marker.endOffset() };

        auto rangeToReplace = makeSimpleRange(node, marker);

        markers.removeMarkers(node, offsetRange, { DocumentMarker::Type::UnifiedTextReplacement });

        if (!accepted && data.state != DocumentMarker::UnifiedTextReplacementData::State::Reverted)
            replaceContentsOfRangeInSession(*state, rangeToReplace, data.originalText);

        return false;
    });
}

template<>
void WritingToolsController::didEndTextReplacementSession<WritingTools::Session::Type::Composition>(const WritingTools::Session& session, bool accepted)
{
    if (accepted)
        return;

    textReplacementSessionDidReceiveEditAction<WritingTools::Session::Type::Composition>(session, WritingTools::Action::ShowOriginal);
}

void WritingToolsController::didEndTextReplacementSession(const WritingTools::Session& session, bool accepted)
{
    RELEASE_LOG(UnifiedTextReplacement, "WritingToolsController::didEndTextReplacementSession (%s) [accepted: %d]", session.identifier.toString().utf8().data(), accepted);

    RefPtr document = this->document();
    if (!document) {
        ASSERT_NOT_REACHED();
        return;
    }

    switch (session.type) {
    case WritingTools::Session::Type::Proofreading:
        didEndTextReplacementSession<WritingTools::Session::Type::Proofreading>(session, accepted);
        break;
    case WritingTools::Session::Type::Composition:
        didEndTextReplacementSession<WritingTools::Session::Type::Composition>(session, accepted);
        break;
    }

    auto sessionRange = contextRangeForSessionWithID(session.identifier);
    if (!sessionRange) {
        ASSERT_NOT_REACHED();
        return;
    }

    m_page->chrome().client().removeTextAnimationForID(session.identifier);

    if (session.compositionType != WritingTools::Session::CompositionType::SmartReply)
        document->selection().setSelection({ *sessionRange });

    m_page->chrome().client().cleanUpTextAnimationsForSessionID(session.identifier);

    m_states.remove(session.identifier);
}

void WritingToolsController::updateStateForSelectedReplacementIfNeeded()
{
    // Optimization: If there are no ongoing sessions, there is no need for any of this logic to
    // be executed, since there will be no relevant document markers anyways.
    if (m_states.isEmpty())
        return;

    RefPtr document = this->document();
    if (!document) {
        ASSERT_NOT_REACHED();
        return;
    }

    auto selectionRange = document->selection().selection().firstRange();
    if (!selectionRange)
        return;

    if (!document->selection().isCaret())
        return;

    auto nodeAndMarker = findTextSuggestionMarkerContainingRange(*selectionRange);
    if (!nodeAndMarker)
        return;

    auto& [node, marker] = *nodeAndMarker;
    auto data = std::get<DocumentMarker::UnifiedTextReplacementData>(marker.data());

    m_page->chrome().client().textReplacementSessionUpdateStateForReplacementWithID(data.sessionID, WritingTools::TextSuggestion::State::Reviewing, data.replacementID);
}

#pragma mark - Private instance helper methods.

std::optional<SimpleRange> WritingToolsController::contextRangeForSessionWithID(const WritingTools::Session::ID& sessionID) const
{
    auto it = m_states.find(sessionID);
    if (it == m_states.end())
        return std::nullopt;

    auto range = WTF::switchOn(it->value,
        [](std::monostate) -> Ref<Range> { RELEASE_ASSERT_NOT_REACHED(); },
        [](const ProofreadingState& state) { return state.contextRange; },
        [](const CompositionState& state) { return state.contextRange; }
    );

    return makeSimpleRange(range);
}

template<WritingTools::Session::Type Type>
WritingToolsController::StateFromSessionType<Type>::Value* WritingToolsController::stateForSession(const WritingTools::Session& session)
{
    if (UNLIKELY(session.type != Type)) {
        ASSERT_NOT_REACHED();
        return nullptr;
    }

    auto it = m_states.find(session.identifier);
    if (UNLIKELY(it == m_states.end())) {
        ASSERT_NOT_REACHED();
        return nullptr;
    }

    return std::get_if<typename WritingToolsController::StateFromSessionType<Type>::Value>(&it->value);
}

RefPtr<Document> WritingToolsController::document() const
{
    if (!m_page) {
        ASSERT_NOT_REACHED();
        return nullptr;
    }

    RefPtr frame = m_page->checkedFocusController()->focusedOrMainFrame();
    if (!frame) {
        ASSERT_NOT_REACHED();
        return nullptr;
    }

    return frame->document();
}

std::optional<std::tuple<Node&, DocumentMarker&>> WritingToolsController::findTextSuggestionMarkerByID(const SimpleRange& outerRange, const WritingTools::TextSuggestion::ID& textSuggestionID) const
{
    RefPtr document = this->document();
    if (!document) {
        ASSERT_NOT_REACHED();
        return std::nullopt;
    }

    RefPtr<Node> targetNode;
    WeakPtr<DocumentMarker> targetMarker;

    document->markers().forEach(outerRange, { DocumentMarker::Type::UnifiedTextReplacement }, [&textSuggestionID, &targetNode, &targetMarker] (auto& node, auto& marker) mutable {
        auto data = std::get<DocumentMarker::UnifiedTextReplacementData>(marker.data());
        if (data.replacementID != textSuggestionID)
            return false;

        targetNode = &node;
        targetMarker = &marker;

        return true;
    });

    if (targetNode && targetMarker)
        return { { *targetNode, *targetMarker } };

    return std::nullopt;
}

std::optional<std::tuple<Node&, DocumentMarker&>> WritingToolsController::findTextSuggestionMarkerContainingRange(const SimpleRange& range) const
{
    RefPtr document = this->document();
    if (!document) {
        ASSERT_NOT_REACHED();
        return std::nullopt;
    }

    RefPtr<Node> targetNode;
    WeakPtr<DocumentMarker> targetMarker;

    document->markers().forEach(range, { DocumentMarker::Type::UnifiedTextReplacement }, [&range, &targetNode, &targetMarker] (auto& node, auto& marker) mutable {
        auto data = std::get<DocumentMarker::UnifiedTextReplacementData>(marker.data());

        auto markerRange = makeSimpleRange(node, marker);
        if (!contains(TreeType::ComposedTree, markerRange, range))
            return false;

        targetNode = &node;
        targetMarker = &marker;

        return true;
    });

    if (targetNode && targetMarker)
        return { { *targetNode, *targetMarker } };

    return std::nullopt;
}

template<typename State>
void WritingToolsController::replaceContentsOfRangeInSessionInternal(State& state, const SimpleRange& range, WTF::Function<void()>&& replacementOperation)
{
    RefPtr document = this->document();
    if (!document) {
        ASSERT_NOT_REACHED();
        return;
    }

    auto sessionRange = makeSimpleRange(state.contextRange);
    auto sessionRangeCount = characterCount(sessionRange);
    auto resolvedCharacterRange = characterRange(sessionRange, range);

    document->selection().setSelection({ range });

    replacementOperation();

    auto selectedTextRange = document->selection().selection().firstRange();
    if (!selectedTextRange) {
        ASSERT_NOT_REACHED();
        return;
    }

    auto extendedPosition = [](const BoundaryPoint& point, uint64_t characterCount, SelectionDirection direction) {
        auto visiblePosition = VisiblePosition { makeContainerOffsetPosition(point) };

        for (uint64_t i = 0; i < characterCount; ++i) {
            auto nextVisiblePosition = positionOfNextBoundaryOfGranularity(visiblePosition, TextGranularity::CharacterGranularity, direction);
            if (nextVisiblePosition.isNull())
                break;

            visiblePosition = nextVisiblePosition;
        }

        return visiblePosition;
    };

    auto extendedSelection = [extendedPosition](const SimpleRange& range, uint64_t charactersToExtendBackwards, uint64_t charactersToExtendForwards) {
        auto start = extendedPosition(range.start, charactersToExtendBackwards, SelectionDirection::Backward);
        auto end = extendedPosition(range.end, charactersToExtendForwards, SelectionDirection::Forward);

        return makeSimpleRange(start, end);
    };

    auto newSessionRange = extendedSelection(*selectedTextRange, resolvedCharacterRange.location, sessionRangeCount - (resolvedCharacterRange.location + resolvedCharacterRange.length));
    if (!newSessionRange) {
        ASSERT_NOT_REACHED();
        return;
    }

    auto updatedLiveRange = createLiveRange(*newSessionRange);
    state.contextRange = updatedLiveRange;
}

void WritingToolsController::replaceContentsOfRangeInSession(ProofreadingState& state, const SimpleRange& range, const String& replacementText)
{
    replaceContentsOfRangeInSessionInternal(state, range, [&] {
        RefPtr document = this->document();
        document->editor().replaceSelectionWithText(replacementText, Editor::SelectReplacement::Yes, Editor::SmartReplace::No, EditAction::InsertReplacement);
    });
}

void WritingToolsController::replaceContentsOfRangeInSession(CompositionState& state, const SimpleRange& range, RefPtr<DocumentFragment>&& fragment, MatchStyle matchStyle)
{
    OptionSet<ReplaceSelectionCommand::CommandOption> options { ReplaceSelectionCommand::PreventNesting, ReplaceSelectionCommand::SanitizeFragment, ReplaceSelectionCommand::SelectReplacement };
    if (matchStyle == MatchStyle::Yes)
        options.add(ReplaceSelectionCommand::MatchStyle);

    replaceContentsOfRangeInSessionInternal(state, range, [&] {
        RefPtr document = this->document();

        auto selection = document->selection().selection();
        if (selection.isNone() || !selection.isContentEditable())
            return;

        auto command = WebCore::ReplaceSelectionCommand::create(Ref { *document }, WTFMove(fragment), options, WebCore::EditAction::InsertReplacement);
        command->apply();

        state.commands.append(command);
    });
}

} // namespace WebKit

#endif
