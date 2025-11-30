#include "Framework/Framework.hpp"

FailedTestInfo::FailedTestInfo(const Char* file, Int32 line, ETestFailureType failureType)
    : mFile(file), mLine(line), mFailureType(failureType) {}

void TestCaseResult::Reset() {
    mInfos.clear();
    mTestCaseName = nullptr;
    mElapsedTime = 0.0;
    mPassed = true;
}

TestCase::TestCase(const Char* name, TestFunction func) : mName(name), mFunc(func) {}

void TestCase::Run(TestCaseResult& result) const {
    mFunc(result);
}