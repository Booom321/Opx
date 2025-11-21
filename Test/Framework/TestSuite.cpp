#include "Framework/Framework.hpp"

TestSuite::TestSuite(const Char* suiteName) : mTestCases{}, mName(suiteName) {}

void TestSuite::AddTestCase(const Char* testCaseName, TestCase::TestFunction testFunction) {
    for (SizeT i = 0, numTestCases = mTestCases.size(); i < numTestCases; ++i) {
        if (!strcmp(mTestCases[i].GetName(), testCaseName) && mTestCases[i].GetFunction() == testFunction) {
            return;
        }
    }
    mTestCases.emplace_back(testCaseName, testFunction);
}