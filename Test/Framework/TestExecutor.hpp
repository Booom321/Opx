#pragma once

#include "TestSuite.hpp"

class TEST_API TestExecutor {
    TestExecutor(TestExecutor&&) = delete;
    TestExecutor(const TestExecutor&) = delete;
    TestExecutor& operator=(TestExecutor&&) = delete;
    TestExecutor& operator=(const TestExecutor&) = delete;

public:
    TestExecutor();
    ~TestExecutor() = default;

    static TestExecutor* Get();

    TestSuite& GetTestSuiteByName(const Char* suiteName);

    void Execute();

private:
    void RunTestSuite(TestSuite& suite);
    void ProcessTestCaseResult(const TestCaseResult& result);

    std::deque<TestSuite> mTestSuites;
    Double mTotalTestTime;

    Int32 mNumPassedTestCases;
    Int32 mNumFailedTestCases;
};