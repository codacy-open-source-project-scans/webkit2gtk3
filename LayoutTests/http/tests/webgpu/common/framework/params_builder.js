/**
 * AUTO-GENERATED - DO NOT EDIT. Source: https://github.com/gpuweb/cts
 **/ import { mergeParams, mergeParamsChecked } from '../internal/params_utils.js';
import { comparePublicParamsPaths, Ordering } from '../internal/query/compare.js';
import { stringifyPublicParams } from '../internal/query/stringify_params.js';
import { assert, mapLazy, objectEquals } from '../util/util.js';

/**
 * Base class for `CaseParamsBuilder` and `SubcaseParamsBuilder`.
 */
export class ParamsBuilderBase {
  constructor(cases) {
    this.cases = cases;
  }

  /**
   * Hidden from test files. Use `builderIterateCasesWithSubcases` to access this.
   */
}

/**
 * Calls the (normally hidden) `iterateCasesWithSubcases()` method.
 */
export function builderIterateCasesWithSubcases(builder, caseFilter) {
  return builder.iterateCasesWithSubcases(caseFilter);
}

/**
 * Builder for combinatorial test **case** parameters.
 *
 * CaseParamsBuilder is immutable. Each method call returns a new, immutable object,
 * modifying the list of cases according to the method called.
 *
 * This means, for example, that the `unit` passed into `TestBuilder.params()` can be reused.
 */
export class CaseParamsBuilder extends ParamsBuilderBase {
  *iterateCasesWithSubcases(caseFilter) {
    for (const caseP of this.cases(caseFilter)) {
      if (caseFilter) {
        // this.cases() only filters out cases which conflict with caseFilter. Now that we have
        // the final caseP, filter out cases which are missing keys that caseFilter requires.
        const ordering = comparePublicParamsPaths(caseP, caseFilter);
        if (ordering === Ordering.StrictSuperset || ordering === Ordering.Unordered) {
          continue;
        }
      }

      yield [caseP, undefined];
    }
  }

  [Symbol.iterator]() {
    return this.cases(null);
  }

  /** @inheritDoc */
  expandWithParams(expander) {
    const baseGenerator = this.cases;
    return new CaseParamsBuilder(function* (caseFilter) {
      for (const a of baseGenerator(caseFilter)) {
        for (const b of expander(a)) {
          if (caseFilter) {
            // If the expander generated any key-value pair that conflicts with caseFilter, skip.
            const kvPairs = Object.entries(b);
            if (kvPairs.some(([k, v]) => k in caseFilter && !objectEquals(caseFilter[k], v))) {
              continue;
            }
          }

          yield mergeParamsChecked(a, b);
        }
      }
    });
  }

  /** @inheritDoc */
  expand(key, expander) {
    const baseGenerator = this.cases;
    return new CaseParamsBuilder(function* (caseFilter) {
      for (const a of baseGenerator(caseFilter)) {
        assert(!(key in a), `New key '${key}' already exists in ${JSON.stringify(a)}`);

        for (const v of expander(a)) {
          // If the expander generated a value for this key that conflicts with caseFilter, skip.
          if (caseFilter && key in caseFilter) {
            if (!objectEquals(caseFilter[key], v)) {
              continue;
            }
          }
          yield { ...a, [key]: v };
        }
      }
    });
  }

  /** @inheritDoc */
  combineWithParams(newParams) {
    assertNotGenerator(newParams);
    const seenValues = new Set();
    for (const params of newParams) {
      const paramsStr = stringifyPublicParams(params);
      assert(!seenValues.has(paramsStr), `Duplicate entry in combine[WithParams]: ${paramsStr}`);
      seenValues.add(paramsStr);
    }

    return this.expandWithParams(() => newParams);
  }

  /** @inheritDoc */
  combine(key, values) {
    assertNotGenerator(values);
    const mapped = mapLazy(values, v => ({ [key]: v }));
    return this.combineWithParams(mapped);
  }

  /** @inheritDoc */
  filter(pred) {
    const baseGenerator = this.cases;
    return new CaseParamsBuilder(function* (caseFilter) {
      for (const a of baseGenerator(caseFilter)) {
        if (pred(a)) yield a;
      }
    });
  }

  /** @inheritDoc */
  unless(pred) {
    return this.filter(x => !pred(x));
  }

  /**
   * "Finalize" the list of cases and begin defining subcases.
   * Returns a new SubcaseParamsBuilder. Methods called on SubcaseParamsBuilder
   * generate new subcases instead of new cases.
   */
  beginSubcases() {
    return new SubcaseParamsBuilder(this.cases, function* () {
      yield {};
    });
  }
}

/**
 * The unit CaseParamsBuilder, representing a single case with no params: `[ {} ]`.
 *
 * `punit` is passed to every `.params()`/`.paramsSubcasesOnly()` call, so `kUnitCaseParamsBuilder`
 * is only explicitly needed if constructing a ParamsBuilder outside of a test builder.
 */
export const kUnitCaseParamsBuilder = new CaseParamsBuilder(function* () {
  yield {};
});

/**
 * Builder for combinatorial test _subcase_ parameters.
 *
 * SubcaseParamsBuilder is immutable. Each method call returns a new, immutable object,
 * modifying the list of subcases according to the method called.
 */
export class SubcaseParamsBuilder extends ParamsBuilderBase {
  constructor(cases, generator) {
    super(cases);
    this.subcases = generator;
  }

  *iterateCasesWithSubcases(caseFilter) {
    for (const caseP of this.cases(caseFilter)) {
      if (caseFilter) {
        // this.cases() only filters out cases which conflict with caseFilter. Now that we have
        // the final caseP, filter out cases which are missing keys that caseFilter requires.
        const ordering = comparePublicParamsPaths(caseP, caseFilter);
        if (ordering === Ordering.StrictSuperset || ordering === Ordering.Unordered) {
          continue;
        }
      }

      const subcases = Array.from(this.subcases(caseP));
      if (subcases.length) {
        yield [caseP, subcases];
      }
    }
  }

  /** @inheritDoc */
  expandWithParams(expander) {
    const baseGenerator = this.subcases;
    return new SubcaseParamsBuilder(this.cases, function* (base) {
      for (const a of baseGenerator(base)) {
        for (const b of expander(mergeParams(base, a))) {
          yield mergeParamsChecked(a, b);
        }
      }
    });
  }

  /** @inheritDoc */
  expand(key, expander) {
    const baseGenerator = this.subcases;
    return new SubcaseParamsBuilder(this.cases, function* (base) {
      for (const a of baseGenerator(base)) {
        const before = mergeParams(base, a);
        assert(!(key in before), () => `Key '${key}' already exists in ${JSON.stringify(before)}`);

        for (const v of expander(before)) {
          yield { ...a, [key]: v };
        }
      }
    });
  }

  /** @inheritDoc */
  combineWithParams(newParams) {
    assertNotGenerator(newParams);
    return this.expandWithParams(() => newParams);
  }

  /** @inheritDoc */
  combine(key, values) {
    assertNotGenerator(values);
    return this.expand(key, () => values);
  }

  /** @inheritDoc */
  filter(pred) {
    const baseGenerator = this.subcases;
    return new SubcaseParamsBuilder(this.cases, function* (base) {
      for (const a of baseGenerator(base)) {
        if (pred(mergeParams(base, a))) yield a;
      }
    });
  }

  /** @inheritDoc */
  unless(pred) {
    return this.filter(x => !pred(x));
  }
}

/** Assert an object is not a Generator (a thing returned from a generator function). */
function assertNotGenerator(x) {
  if ('constructor' in x) {
    assert(
      x.constructor !== (function* () {})().constructor,
      'Argument must not be a generator, as generators are not reusable'
    );
  }
}
