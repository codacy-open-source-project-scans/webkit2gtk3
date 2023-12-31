'use strict';

const assert = require('assert');

const MockData = require('./resources/mock-data.js');
const TestServer = require('./resources/test-server.js');
const addWorkerForReport = require('./resources/common-operations.js').addWorkerForReport;
const prepareServerTest = require('./resources/common-operations.js').prepareServerTest;
const assertThrows = require('../server-tests/resources/common-operations.js').assertThrows;

async function createAnalysisTask(name, webkitRevisions = ["191622", "191623"])
{
    const reportWithRevision = [{
        "buildTag": "124",
        "buildTime": "2015-10-27T15:34:51",
        "revisions": {
            "WebKit": {
                "revision": webkitRevisions[0],
                "timestamp": '2015-10-27T11:36:56.878473Z',
            },
            "macOS": {
                "revision": "15A284",
            }
        },
        "builderName": "someBuilder",
        "workerName": "someWorker",
        "workerPassword": "somePassword",
        "platform": "some platform",
        "tests": {
            "some test": {
                "metrics": {
                    "Time": ["Arithmetic"],
                },
                "tests": {
                    "test1": {
                        "metrics": {"Time": { "current": [11] }},
                    }
                }
            },
        }}];

    const anotherReportWithRevision = [{
        "buildTag": "125",
        "buildTime": "2015-10-27T17:27:41",
        "revisions": {
            "WebKit": {
                "revision": webkitRevisions[1],
                "timestamp": '2015-10-27T16:38:10.768995Z',
            },
            "macOS": {
                "revision": "15A284",
            }
        },
        "builderName": "someBuilder",
        "workerName": "someWorker",
        "workerPassword": "somePassword",
        "platform": "some platform",
        "tests": {
            "some test": {
                "metrics": {
                    "Time": ["Arithmetic"],
                },
                "tests": {
                    "test1": {
                        "metrics": {"Time": { "current": [12] }},
                    }
                }
            },
        }}];

    const db = TestServer.database();
    const remote = TestServer.remoteAPI();
    await addWorkerForReport(reportWithRevision[0]);
    await remote.postJSON('/api/report/', reportWithRevision);
    await remote.postJSON('/api/report/', anotherReportWithRevision);
    await Manifest.fetch();
    const test = Test.findByPath(['some test', 'test1']);
    const platform = Platform.findByName('some platform');
    const configRow = await db.selectFirstRow('test_configurations', {metric: test.metrics()[0].id(), platform: platform.id()});
    const testRuns = await db.selectRows('test_runs', {config: configRow['id']});

    assert.strictEqual(testRuns.length, 2);
    const content = await PrivilegedAPI.sendRequest('create-analysis-task', {
        name: name,
        startRun: testRuns[0]['id'],
        endRun: testRuns[1]['id'],
        needsNotification: true,
    });
    return content['taskId'];
}

async function addTriggerableAndCreateTask(name, webkitRevisions, testParametersList=[[], []])
{
    const report = {
        'workerName': 'anotherWorker',
        'workerPassword': 'anotherPassword',
        'triggerable': 'build-webkit',
        'configurations': [
            {test: MockData.someTestId(), platform: MockData.somePlatformId(), supportedRepetitionTypes: ['alternating', 'sequential'], testParameters: testParametersList[0]},
            {test: MockData.someTestId(), platform: MockData.otherPlatformId(), supportedRepetitionTypes: ['alternating', 'sequential'], testParameters: testParametersList[1]},
        ],
        'repositoryGroups': [
            {name: 'os-only', acceptsRoot: true, repositories: [
                    {repository: MockData.macosRepositoryId(), acceptsPatch: false},
                ]},
            {name: 'webkit-only', acceptsRoot: true, repositories: [
                    {repository: MockData.webkitRepositoryId(), acceptsPatch: true},
                ]},
            {name: 'system-and-webkit', acceptsRoot: true, repositories: [
                    {repository: MockData.macosRepositoryId(), acceptsPatch: false},
                    {repository: MockData.webkitRepositoryId(), acceptsPatch: true}
                ]},
            {name: 'system-webkit-sjc', acceptsRoot: true, repositories: [
                    {repository: MockData.macosRepositoryId(), acceptsPatch: false},
                    {repository: MockData.jscRepositoryId(), acceptsPatch: false},
                    {repository: MockData.webkitRepositoryId(), acceptsPatch: true}
                ]},
        ]
    };
    await MockData.addMockData(TestServer.database());
    await addWorkerForReport(report);
    await TestServer.remoteAPI().postJSON('/api/update-triggerable/', report);
    await createAnalysisTask(name, webkitRevisions);
}

function assertOrderOfRequests(requests, expectedOrder)
{
    assert.deepEqual(requests.map(request => request.order()), expectedOrder);
}

describe('/privileged-api/add-build-requests', function() {
    prepareServerTest(this, 'node');
    beforeEach(() => {
        PrivilegedAPI.configure('test', 'password');
    });

    it('should be able to add build requests to test group', async () => {
        await addTriggerableAndCreateTask('some task');
        const webkit = Repository.all().filter((repository) => repository.name() == 'WebKit')[0];
        const revisionSets = [{[webkit.id()]: {revision: '191622'}}, {[webkit.id()]: {revision: '191623'}}];
        let result = await PrivilegedAPI.sendRequest('create-test-group',
            {name: 'test', taskName: 'other task', platform: MockData.somePlatformId(), test: MockData.someTestId(),
                needsNotification: false, repetitionCount: 2, revisionSets, commitSet: null});
        const insertedGroupId = result['testGroupId'];

        await PrivilegedAPI.sendRequest('update-test-group', {'group': insertedGroupId, mayNeedMoreRequests: true});

        const testGroups = await TestGroup.fetchForTask(result['taskId'], true);
        assert.strictEqual(testGroups.length, 1);
        const group = testGroups[0];
        assert.strictEqual(group.id(), insertedGroupId);
        assert.strictEqual(group.mayNeedMoreRequests(), true);
        for (const commitSet of group.requestedCommitSets())
            assert.strictEqual(+group.repetitionCountForCommitSet(commitSet), 2);
        assert.strictEqual(+group.initialRepetitionCount(), 2);

        await PrivilegedAPI.sendRequest('add-build-requests', {group: insertedGroupId, addCount: 2});

        const updatedGroups = await TestGroup.fetchForTask(result['taskId'], true);
        assert.strictEqual(updatedGroups.length, 1);
        for (const commitSet of updatedGroups[0].requestedCommitSets())
            assert.strictEqual(+updatedGroups[0].repetitionCountForCommitSet(commitSet), 4);
        assert.strictEqual(+updatedGroups[0].initialRepetitionCount(), 2);
        assert.strictEqual(group.mayNeedMoreRequests(), true);
        for (const commitSet of updatedGroups[0].requestedCommitSets()) {
            const buildRequests = updatedGroups[0].requestsForCommitSet(commitSet);
            assert.strictEqual(buildRequests.length, 4);
        }
    });

    it('should be able to add build requests with the same test parameter sets to a test group', async () => {
        await addTriggerableAndCreateTask('some task',  ["191622", "191623"], [[1, 2], [1, 2]]);
        const webkit = Repository.all().filter((repository) => repository.name() == 'WebKit')[0];
        const revisionSets = [{[webkit.id()]: {revision: '191622'}}, {[webkit.id()]: {revision: '191623'}}];
        const testParametersList = [{1: {value: true, file: null}, 2: {value: 'Xcode 14.3'}}, {2: {value: 'Xcode 14.3'}}];
        let result = await PrivilegedAPI.sendRequest('create-test-group',
            {name: 'test', taskName: 'other task', platform: MockData.somePlatformId(), test: MockData.someTestId(),
                needsNotification: true, revisionSets, testParametersList, repetitionType: 'alternating', repetitionCount: 2});
        const insertedGroupId = result['testGroupId'];

        await PrivilegedAPI.sendRequest('update-test-group', {'group': insertedGroupId, mayNeedMoreRequests: true});

        const [analysisTask, testGroups] = await Promise.all([AnalysisTask.fetchById(result['taskId']), TestGroup.fetchForTask(result['taskId'], true)]);
        assert.strictEqual(analysisTask.name(), 'other task');

        assert.strictEqual(testGroups.length, 1);
        const group = testGroups[0];
        assert.strictEqual(group.id(), insertedGroupId);
        assert.strictEqual(group.initialRepetitionCount(), 2);
        assert.strictEqual(group.repetitionType(), 'alternating')
        assert.ok(group.needsNotification());
        assert.ok(group.mayNeedMoreRequests());
        let requests = group.buildRequests();
        assert.strictEqual(requests.length, 6);

        assert.strictEqual(requests[0].order(), -2);
        assert.strictEqual(requests[1].order(), -1);
        assert.strictEqual(requests[2].order(), 0);
        assert.strictEqual(requests[3].order(), 1);
        assert.strictEqual(requests[4].order(), 2);
        assert.strictEqual(requests[5].order(), 3);

        assert.strictEqual(group.requestedCommitSets().length, 2);
        assert(requests[0].commitSet().equals(requests[2].commitSet()));
        assert(requests[0].commitSet().equals(requests[4].commitSet()));
        assert(requests[1].commitSet().equals(requests[3].commitSet()));
        assert(requests[1].commitSet().equals(requests[5].commitSet()));

        const set0 = requests[0].commitSet();
        const set1 = requests[1].commitSet();
        assert.deepStrictEqual(set0.repositories(), [webkit]);
        assert.deepStrictEqual(set0.customRoots(), []);
        assert.deepStrictEqual(set1.repositories(), [webkit]);
        assert.deepStrictEqual(set1.customRoots(), []);
        assert.strictEqual(set0.revisionForRepository(webkit), '191622');
        assert.strictEqual(set1.revisionForRepository(webkit), '191623');

        assert.strictEqual(group.requestedTestParameterSets().length, 2);
        const parameterSet0 = requests[0].testParameterSet();
        const parameterSet1 = requests[1].testParameterSet();
        assert(parameterSet0 === requests[2].testParameterSet());
        assert(parameterSet0 === requests[4].testParameterSet());
        assert(parameterSet1 === requests[3].testParameterSet());
        assert(parameterSet1 === requests[5].testParameterSet());

        assert.strictEqual(parameterSet0.parameters.length, 2);
        assert.strictEqual(parameterSet0.buildTypeParameters.length, 1);
        assert.strictEqual(parameterSet0.testTypeParameters.length, 1);
        assert.strictEqual(parameterSet0.valueForParameterName('diagnose'), true);
        assert.strictEqual(parameterSet0.fileForParameterName('diagnose'), undefined);
        assert.strictEqual(parameterSet0.valueForParameterName('Custom SDK'), 'Xcode 14.3');
        assert.strictEqual(parameterSet0.fileForParameterName('Custom SDK'), undefined);

        assert.strictEqual(parameterSet1.parameters.length, 1);
        assert.strictEqual(parameterSet1.buildTypeParameters.length, 1);
        assert.strictEqual(parameterSet1.testTypeParameters.length, 0);
        assert.strictEqual(parameterSet1.valueForParameterName('Custom SDK'), 'Xcode 14.3');
        assert.strictEqual(parameterSet1.fileForParameterName('Custom SDK'), undefined);

        await PrivilegedAPI.sendRequest('add-build-requests', {group: insertedGroupId, addCount: 2});

        const updatedGroups = await TestGroup.fetchForTask(result['taskId'], true);
        assert.strictEqual(updatedGroups.length, 1);
        for (const commitSet of updatedGroups[0].requestedCommitSets())
            assert.strictEqual(+updatedGroups[0].repetitionCountForCommitSet(commitSet), 4);
        assert.strictEqual(+updatedGroups[0].initialRepetitionCount(), 2);
        assert.strictEqual(group.mayNeedMoreRequests(), true);
        for (const commitSet of updatedGroups[0].requestedCommitSets()) {
            const buildRequests = updatedGroups[0].requestsForCommitSet(commitSet);
            assert.strictEqual(buildRequests.length, 5);
        }
        requests = updatedGroups[0].buildRequests();
        assert.strictEqual(updatedGroups[0].requestedTestParameterSets().length, 2);
        assert(parameterSet0 === requests[6].testParameterSet());
        assert(parameterSet0 === requests[8].testParameterSet());
        assert(parameterSet1 === requests[7].testParameterSet());
        assert(parameterSet1 === requests[9].testParameterSet());
    });

    it('should not be able to add build requests to a hidden test group', async () => {
        await addTriggerableAndCreateTask('some task');
        const webkit = Repository.all().filter((repository) => repository.name() == 'WebKit')[0];
        const revisionSets = [{[webkit.id()]: {revision: '191622'}}, {[webkit.id()]: {revision: '191623'}}];
        let result = await PrivilegedAPI.sendRequest('create-test-group',
            {name: 'test', taskName: 'other task', platform: MockData.somePlatformId(), test: MockData.someTestId(),
                hidden: true, needsNotification: false, repetitionCount: 2, revisionSets});
        const insertedGroupId = result['testGroupId'];

        const testGroups = await TestGroup.fetchForTask(result['taskId'], true);
        assert.strictEqual(testGroups.length, 1);
        const group = testGroups[0];
        assert.strictEqual(group.id(), insertedGroupId);
        assert.strictEqual(group.mayNeedMoreRequests(), false);
        for (const commitSet of group.requestedCommitSets())
            assert.strictEqual(+group.repetitionCountForCommitSet(commitSet), 2);
        assert.strictEqual(+group.initialRepetitionCount(), 2);
        await group.updateHiddenFlag(true);

        await assertThrows('CannotAddToHiddenTestGroup', async () => await PrivilegedAPI.sendRequest('add-build-requests', {group: insertedGroupId, addCount: 2}))
    });

    it('should reject with "CommitSetNotSupportedRepetitionType" when adding build requests for one commit set in an alternating test group', async () => {
        await addTriggerableAndCreateTask('some task');
        const webkit = Repository.all().filter((repository) => repository.name() == 'WebKit')[0];
        const revisionSets = [{[webkit.id()]: {revision: '191622'}}, {[webkit.id()]: {revision: '191623'}}];
        const result = await PrivilegedAPI.sendRequest('create-test-group', {
            name: 'test', taskName: 'other task', platform: MockData.somePlatformId(), test: MockData.someTestId(),
            needsNotification: false, repetitionCount: 2, repetitionType: 'alternating', revisionSets});
        const insertedGroupId = result['testGroupId'];

        await PrivilegedAPI.sendRequest('update-test-group', {'group': insertedGroupId, mayNeedMoreRequests: true});

        const ignoreCache = true;
        const testGroups = await TestGroup.fetchForTask(result['taskId'], ignoreCache);
        assert.strictEqual(testGroups.length, 1);
        const group = testGroups[0];
        assert.strictEqual(group.id(), insertedGroupId);
        assert.strictEqual(group.mayNeedMoreRequests(), true);
        assert.strictEqual(+group.initialRepetitionCount(), 2);
        assert.strictEqual(group.repetitionType(), 'alternating');
        assert.strictEqual(group.requestedCommitSets().length, 2);
        for (const commitSet of group.requestedCommitSets())
            assert.strictEqual(+group.repetitionCountForCommitSet(commitSet), 2);

        const firstCommitSet = group.requestedCommitSets()[0];
        assertOrderOfRequests(group.requestsForCommitSet(firstCommitSet), [0, 2]);
        const secondCommitSet = group.requestedCommitSets()[1];
        assertOrderOfRequests(group.requestsForCommitSet(secondCommitSet), [1, 3]);

        await assertThrows('CommitSetNotSupportedRepetitionType', () => {
            return PrivilegedAPI.sendRequest('add-build-requests',
                {group: insertedGroupId, addCount: 2, commitSet: secondCommitSet.id()})
        });
    });

    it('should be able to build requests for first commit set with order shifted in a sequential test group', async () => {
        await addTriggerableAndCreateTask('some task');
        const webkit = Repository.all().filter((repository) => repository.name() == 'WebKit')[0];
        const revisionSets = [{[webkit.id()]: {revision: '191622'}}, {[webkit.id()]: {revision: '191623'}}];
        const result = await PrivilegedAPI.sendRequest('create-test-group',
            {name: 'test', taskName: 'other task', platform: MockData.somePlatformId(), repetitionCount: 2,
            test: MockData.someTestId(), needsNotification: false, repetitionType: 'sequential', revisionSets});
        const insertedGroupId = result['testGroupId'];

        const ignoreCache = true;
        const testGroups = await TestGroup.fetchForTask(result['taskId'], ignoreCache);
        assert.strictEqual(testGroups.length, 1);
        let group = testGroups[0];
        assert.strictEqual(group.id(), insertedGroupId);
        assert.strictEqual(+group.initialRepetitionCount(), 2);
        assert.strictEqual(group.repetitionType(), 'sequential');
        assert.strictEqual(group.requestedCommitSets().length, 2);
        for (const commitSet of group.requestedCommitSets())
            assert.strictEqual(+group.repetitionCountForCommitSet(commitSet), 2);

        let firstCommitSet = group.requestedCommitSets()[0];
        assertOrderOfRequests(group.requestsForCommitSet(firstCommitSet), [0, 1]);
        let secondCommitSet = group.requestedCommitSets()[1];
        assertOrderOfRequests(group.requestsForCommitSet(secondCommitSet), [2, 3]);

        await PrivilegedAPI.sendRequest('add-build-requests',
            {group: insertedGroupId, addCount: 2, commitSet: firstCommitSet.id()});

        const updatedGroups = await TestGroup.fetchForTask(result['taskId'], ignoreCache);
        assert.strictEqual(updatedGroups.length, 1);
        group = updatedGroups[0];
        assert.strictEqual(+group.initialRepetitionCount(), 2);
        assert.strictEqual(group.repetitionType(), 'sequential');
        assert.strictEqual(group.requestedCommitSets().length, 2);

        firstCommitSet = group.requestedCommitSets()[0];
        assertOrderOfRequests(group.requestsForCommitSet(firstCommitSet), [0, 1, 2, 3]);
        secondCommitSet = group.requestedCommitSets()[1];
        assertOrderOfRequests(group.requestsForCommitSet(secondCommitSet), [4, 5]);
    });

    it('should not modify the order of preceding build requests when adding new build requests in a sequential test group', async () => {
        await addTriggerableAndCreateTask('some task');
        const webkit = Repository.all().filter((repository) => repository.name() == 'WebKit')[0];
        const revisionSets = [{[webkit.id()]: {revision: '191622'}}, {[webkit.id()]: {revision: '191623'}}];
        const result = await PrivilegedAPI.sendRequest('create-test-group',
            {name: 'test', taskName: 'other task', platform: MockData.somePlatformId(), repetitionCount: 2,
             test: MockData.someTestId(), needsNotification: false, repetitionType: 'sequential', revisionSets});
        const insertedGroupId = result['testGroupId'];

        const ignoreCache = true;
        const testGroups = await TestGroup.fetchForTask(result['taskId'], ignoreCache);
        assert.strictEqual(testGroups.length, 1);
        let group = testGroups[0];
        assert.strictEqual(group.id(), insertedGroupId);
        assert.strictEqual(+group.initialRepetitionCount(), 2);
        assert.strictEqual(group.repetitionType(), 'sequential');
        assert.strictEqual(group.requestedCommitSets().length, 2);
        for (const commitSet of group.requestedCommitSets())
            assert.strictEqual(+group.repetitionCountForCommitSet(commitSet), 2);

        let firstCommitSet = group.requestedCommitSets()[0];
        assertOrderOfRequests(group.requestsForCommitSet(firstCommitSet), [0, 1]);
        let secondCommitSet = group.requestedCommitSets()[1];
        assertOrderOfRequests(group.requestsForCommitSet(secondCommitSet), [2, 3]);

        await PrivilegedAPI.sendRequest('add-build-requests',
            {group: insertedGroupId, addCount: 2, commitSet: secondCommitSet.id()});

        const updatedGroups = await TestGroup.fetchForTask(result['taskId'], ignoreCache);
        assert.strictEqual(updatedGroups.length, 1);
        group = updatedGroups[0];
        assert.strictEqual(+group.initialRepetitionCount(), 2);
        assert.strictEqual(group.repetitionType(), 'sequential');
        assert.strictEqual(group.requestedCommitSets().length, 2);

        firstCommitSet = group.requestedCommitSets()[0];
        assertOrderOfRequests(group.requestsForCommitSet(firstCommitSet), [0, 1]);
        secondCommitSet = group.requestedCommitSets()[1];
        assertOrderOfRequests(group.requestsForCommitSet(secondCommitSet), [2, 3, 4, 5]);
    });

    it('should shift the order of following build requests when adding retry for a specific commit set in a sequential test group with 3 commit sets', async () => {
        await addTriggerableAndCreateTask('some task');
        const webkit = Repository.all().filter((repository) => repository.name() == 'WebKit')[0];
        const revisionSets = [{[webkit.id()]: {revision: '191622'}}, {[webkit.id()]: {revision: '191623'}},
            {[webkit.id()]: {revision: '192736'}}];
        const result = await PrivilegedAPI.sendRequest('create-test-group',
            {name: 'test', taskName: 'other task', platform: MockData.somePlatformId(), repetitionCount: 2,
            test: MockData.someTestId(), needsNotification: false, repetitionType: 'sequential', revisionSets});
        const insertedGroupId = result['testGroupId'];

        const ignoreCache = true;
        const testGroups = await TestGroup.fetchForTask(result['taskId'], ignoreCache);
        assert.strictEqual(testGroups.length, 1);
        let group = testGroups[0];
        assert.strictEqual(group.id(), insertedGroupId);
        assert.strictEqual(+group.initialRepetitionCount(), 2);
        assert.strictEqual(group.repetitionType(), 'sequential');
        assert.strictEqual(group.requestedCommitSets().length, 3);
        for (const commitSet of group.requestedCommitSets())
            assert.strictEqual(+group.repetitionCountForCommitSet(commitSet), 2);

        let firstCommitSet = group.requestedCommitSets()[0];
        assertOrderOfRequests(group.requestsForCommitSet(firstCommitSet), [0, 1]);
        let secondCommitSet = group.requestedCommitSets()[1];
        assertOrderOfRequests(group.requestsForCommitSet(secondCommitSet), [2, 3]);
        let thirdCommitSet = group.requestedCommitSets()[2];
        assertOrderOfRequests(group.requestsForCommitSet(thirdCommitSet), [4, 5]);

        await PrivilegedAPI.sendRequest('add-build-requests',
            {group: insertedGroupId, addCount: 2, commitSet: firstCommitSet.id()});

        const updatedGroups = await TestGroup.fetchForTask(result['taskId'], ignoreCache);
        assert.strictEqual(updatedGroups.length, 1);
        group = updatedGroups[0];
        assert.strictEqual(+updatedGroups[0].initialRepetitionCount(), 2);
        assert.strictEqual(group.repetitionType(), 'sequential');
        assert.strictEqual(group.requestedCommitSets().length, 3);

        firstCommitSet = group.requestedCommitSets()[0];
        assertOrderOfRequests(group.requestsForCommitSet(firstCommitSet), [0, 1, 2, 3]);
        secondCommitSet = group.requestedCommitSets()[1];
        assertOrderOfRequests(group.requestsForCommitSet(secondCommitSet), [4, 5]);
        thirdCommitSet = group.requestedCommitSets()[2];
        assertOrderOfRequests(group.requestsForCommitSet(thirdCommitSet), [6, 7]);
    });

    it('should shift the order of build requests when adding retry for all commit sets in a sequential test group with 3 commit sets', async () => {
        await addTriggerableAndCreateTask('some task');
        const webkit = Repository.all().filter((repository) => repository.name() == 'WebKit')[0];
        const revisionSets = [{[webkit.id()]: {revision: '191622'}}, {[webkit.id()]: {revision: '191623'}},
            {[webkit.id()]: {revision: '192736'}}];
        const result = await PrivilegedAPI.sendRequest('create-test-group',
            {name: 'test', taskName: 'other task', platform: MockData.somePlatformId(), repetitionCount: 2,
            test: MockData.someTestId(), needsNotification: false, repetitionType: 'sequential', revisionSets});
        const insertedGroupId = result['testGroupId'];

        const ignoreCache = true;
        const testGroups = await TestGroup.fetchForTask(result['taskId'], ignoreCache);
        assert.strictEqual(testGroups.length, 1);
        let group = testGroups[0];
        assert.strictEqual(group.id(), insertedGroupId);
        assert.strictEqual(+group.initialRepetitionCount(), 2);
        assert.strictEqual(group.repetitionType(), 'sequential');
        assert.strictEqual(group.requestedCommitSets().length, 3);
        for (const commitSet of group.requestedCommitSets())
            assert.strictEqual(+group.repetitionCountForCommitSet(commitSet), 2);

        let firstCommitSet = group.requestedCommitSets()[0];
        assertOrderOfRequests(group.requestsForCommitSet(firstCommitSet), [0, 1]);
        let secondCommitSet = group.requestedCommitSets()[1];
        assertOrderOfRequests(group.requestsForCommitSet(secondCommitSet), [2, 3]);
        let thirdCommitSet = group.requestedCommitSets()[2];
        assertOrderOfRequests(group.requestsForCommitSet(thirdCommitSet), [4, 5]);

        await PrivilegedAPI.sendRequest('add-build-requests',
            {group: insertedGroupId, addCount: 2});

        const updatedGroups = await TestGroup.fetchForTask(result['taskId'], ignoreCache);
        group = updatedGroups[0];
        assert.strictEqual(updatedGroups.length, 1);
        assert.strictEqual(+group.initialRepetitionCount(), 2);
        assert.strictEqual(group.repetitionType(), 'sequential');
        assert.strictEqual(group.requestedCommitSets().length, 3);

        firstCommitSet = group.requestedCommitSets()[0];
        assertOrderOfRequests(group.requestsForCommitSet(firstCommitSet), [0, 1, 2, 3]);
        secondCommitSet = group.requestedCommitSets()[1];
        assertOrderOfRequests(group.requestsForCommitSet(secondCommitSet), [4, 5, 6, 7]);
        thirdCommitSet = group.requestedCommitSets()[2];
        assertOrderOfRequests(group.requestsForCommitSet(thirdCommitSet), [8, 9, 10, 11]);
    });

    it('should reject with "NoCommitSetInTestGroup" if commit set is not in a sequential test group', async () => {
        await addTriggerableAndCreateTask('some task');
        const webkit = Repository.all().filter((repository) => repository.name() == 'WebKit')[0];
        const revisionSets = [{[webkit.id()]: {revision: '191622'}}, {[webkit.id()]: {revision: '191623'}}];
        let result = await PrivilegedAPI.sendRequest('create-test-group',
            {name: 'test', taskName: 'other task', platform: MockData.somePlatformId(), test: MockData.someTestId(),
            needsNotification: false, repetitionCount: 2, repetitionType: 'sequential', revisionSets});
        const insertedGroupId = result['testGroupId'];

        await PrivilegedAPI.sendRequest('update-test-group', {'group': insertedGroupId, mayNeedMoreRequests: true});

        const testGroups = await TestGroup.fetchForTask(result['taskId'], true);
        assert.strictEqual(testGroups.length, 1);
        const group = testGroups[0];
        assert.strictEqual(group.id(), insertedGroupId);
        assert.strictEqual(group.mayNeedMoreRequests(), true);
        assert.strictEqual(+group.initialRepetitionCount(), 2);
        assert.strictEqual(group.repetitionType(), 'sequential');
        assert.strictEqual(group.requestedCommitSets().length, 2);
        for (const commitSet of group.requestedCommitSets())
            assert.strictEqual(group.repetitionCountForCommitSet(commitSet), 2);

        const firstCommitSet = group.requestedCommitSets()[0];
        assertOrderOfRequests(group.requestsForCommitSet(firstCommitSet), [0, 1]);
        const secondCommitSet = group.requestedCommitSets()[1];
        assertOrderOfRequests(group.requestsForCommitSet(secondCommitSet), [2, 3]);

        const commitSet = firstCommitSet.id() + secondCommitSet.id() + 1;
        await assertThrows('NoCommitSetInTestGroup', () => {
            return PrivilegedAPI.sendRequest('add-build-requests', {group: insertedGroupId, addCount: 2, commitSet});
        });
    });

    it('should reject with "InvalidCommitSet" if commit set id is not an integer for a sequential test group', async () => {
        await addTriggerableAndCreateTask('some task');
        const webkit = Repository.all().filter((repository) => repository.name() == 'WebKit')[0];
        const revisionSets = [{[webkit.id()]: {revision: '191622'}}, {[webkit.id()]: {revision: '191623'}}];
        let result = await PrivilegedAPI.sendRequest('create-test-group',
            {name: 'test', taskName: 'other task', platform: MockData.somePlatformId(), test: MockData.someTestId(),
            needsNotification: false, repetitionCount: 2, repetitionType: 'sequential', revisionSets});
        const insertedGroupId = result['testGroupId'];

        await PrivilegedAPI.sendRequest('update-test-group', {'group': insertedGroupId, mayNeedMoreRequests: true});

        const testGroups = await TestGroup.fetchForTask(result['taskId'], true);
        assert.strictEqual(testGroups.length, 1);
        const group = testGroups[0];
        assert.strictEqual(group.id(), insertedGroupId);
        assert.strictEqual(group.mayNeedMoreRequests(), true);
        assert.strictEqual(+group.initialRepetitionCount(), 2);
        assert.strictEqual(group.repetitionType(), 'sequential');
        assert.strictEqual(group.requestedCommitSets().length, 2);
        for (const commitSet of group.requestedCommitSets())
            assert.strictEqual(group.repetitionCountForCommitSet(commitSet), 2);

        const commitSet = 'invalid';
        await assertThrows('InvalidCommitSet', () => {
            return PrivilegedAPI.sendRequest('add-build-requests', {group: insertedGroupId, addCount: 2, commitSet});
        });
    });

    it('should reject with "InvalidCommitSet" if commit set id is not an integer for an alternating test group', async () => {
        await addTriggerableAndCreateTask('some task');
        const webkit = Repository.all().filter((repository) => repository.name() == 'WebKit')[0];
        const revisionSets = [{[webkit.id()]: {revision: '191622'}}, {[webkit.id()]: {revision: '191623'}}];
        let result = await PrivilegedAPI.sendRequest('create-test-group',
            {name: 'test', taskName: 'other task', platform: MockData.somePlatformId(), test: MockData.someTestId(),
            needsNotification: false, repetitionCount: 2, repetitionType: 'alternating', revisionSets});
        const insertedGroupId = result['testGroupId'];

        await PrivilegedAPI.sendRequest('update-test-group', {'group': insertedGroupId, mayNeedMoreRequests: true});

        const testGroups = await TestGroup.fetchForTask(result['taskId'], true);
        assert.strictEqual(testGroups.length, 1);
        const group = testGroups[0];
        assert.strictEqual(group.id(), insertedGroupId);
        assert.strictEqual(group.mayNeedMoreRequests(), true);
        assert.strictEqual(+group.initialRepetitionCount(), 2);
        assert.strictEqual(group.repetitionType(), 'alternating');
        assert.strictEqual(group.requestedCommitSets().length, 2);
        for (const commitSet of group.requestedCommitSets())
            assert.strictEqual(group.repetitionCountForCommitSet(commitSet), 2);

        const commitSet = 'invalid';
        await assertThrows('InvalidCommitSet', () => {
            return PrivilegedAPI.sendRequest('add-build-requests', {group: insertedGroupId, addCount: 2, commitSet})
        });
    });
});