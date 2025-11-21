#pragma once

#include <vector>

#include "Common.hpp"

enum class ETestFailureType : Int8 {
    Unknown = -1,
    Test,
    Assertion
};

class TEST_API FailedTestInfo {
public:
    FailedTestInfo(const Char* file, Int32 line, ETestFailureType failureType);

    const Char* GetFile() const { return mFile; }
    Int32 GetLine() const { return mLine; }
    ETestFailureType GetFailureType() const { return mFailureType; }

private:
    const Char* mFile = nullptr;
    Int32 mLine = -1;
    ETestFailureType mFailureType = ETestFailureType::Unknown;
};

class TEST_API TestCaseResult {
public:
    TestCaseResult() = default;
    ~TestCaseResult() = default;

    void Reset();

    std::vector<FailedTestInfo>& GetInfos() { return mInfos; }
    const std::vector<FailedTestInfo>& GetInfos() const { return mInfos; }
    const Char* GetTestCaseName() const { return mTestCaseName; }
    Double GetElapsedTime() const { return mElapsedTime; }
    Bool IsPassed() const { return mPassed; }

    void SetTestCaseName(const Char* testCaseName) { mTestCaseName = testCaseName; }
    void SetElapsedTime(Double elapsedTime) { mElapsedTime = elapsedTime; }
    void SetPassed(Bool passed) { mPassed = passed; }

private:
    std::vector<FailedTestInfo> mInfos{};
    const Char* mTestCaseName = nullptr;
    Double mElapsedTime = 0.0;
    Bool mPassed = true;
};

class TEST_API TestCase {
public:
    using TestFunction = void (*)(TestCaseResult&);
    TestCase(const Char* name, TestFunction func);

    void Run(TestCaseResult& result) const;

    const Char* GetName() const { return mName; }
    TestFunction GetFunction() const { return mFunc; }

private:
    const Char* mName;
    TestFunction mFunc;
};