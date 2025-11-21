#pragma once

#include <deque>

#include "TestCase.hpp"

class TEST_API TestSuite {
public:
    explicit TestSuite(const Char* name);
    ~TestSuite() = default;

    void AddTestCase(const Char* testCaseName, TestCase::TestFunction testFunction);
    const Char* GetName() const { return mName; }
    std::deque<TestCase>& GetTestCases() { return mTestCases; };

private:
    std::deque<TestCase> mTestCases;
    const Char* mName;
};