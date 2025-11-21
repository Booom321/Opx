#pragma once

#include "TestExecutor.hpp"

Bool CreateTestCase(const Char* testSuiteName, const Char* testCaseName, TestCase::TestFunction func);
void ExpectImpl(TestCaseResult& result, Bool isTrue, const Char* file, Int32 line, ETestFailureType failureType);

Bool CompareString(const Char* lhs, const Char* rhs);
Bool CompareFloat(Float lhs, Float rhs, UInt32 maxUlps = 4);
Bool CompareDouble(Double lhs, Double rhs, UInt64 maxUlps = 4);

#define TEST_CASE_RESULT_VAR result

#define TEST_CASE(testSuiteName, testCaseName)                                                                                                  \
    void Test##testSuiteName##testCaseName(TestCaseResult&);                                                                                    \
    static const Bool TestCase##testSuiteName##testCaseName = CreateTestCase(#testSuiteName, #testCaseName, Test##testSuiteName##testCaseName); \
    void Test##testSuiteName##testCaseName(TestCaseResult& TEST_CASE_RESULT_VAR)

#define TEST_EXPECT_TRUE(...) ExpectImpl(TEST_CASE_RESULT_VAR, (__VA_ARGS__), __FILE__, __LINE__, ETestFailureType::Test)
#define TEST_EXPECT_FALSE(...) ExpectImpl(TEST_CASE_RESULT_VAR, !(__VA_ARGS__), __FILE__, __LINE__, ETestFailureType::Test)

#define TEST_EXPECT_EQ(x, y) TEST_EXPECT_TRUE(x == y)
#define TEST_EXPECT_NEQ(x, y) TEST_EXPECT_TRUE(x != y)

#define TEST_EXPECT_STR_EQ(x, y) TEST_EXPECT_TRUE(CompareString(x, y))
#define TEST_EXPECT_STR_NEQ(x, y) TEST_EXPECT_TRUE(!CompareString(x, y))

#define TEST_EXPECT_FLOAT_EQ(x, y, maxUlps) TEST_EXPECT_TRUE(CompareFloat(x, y, maxUlps))
#define TEST_EXPECT_FLOAT_NEQ(x, y, maxUlps) TEST_EXPECT_TRUE(!CompareFloat(x, y, maxUlps))

#define TEST_EXPECT_DOUBLE_EQ(x, y, maxUlps) TEST_EXPECT_TRUE(CompareDouble(x, y, maxUlps))
#define TEST_EXPECT_DOUBLE_NEQ(x, y, maxUlps) TEST_EXPECT_TRUE(!CompareDouble(x, y, maxUlps))

#define TEST_ASSERT_TRUE(...)                                                                           \
    do {                                                                                                \
        if (!(__VA_ARGS__)) {                                                                           \
            ExpectImpl(TEST_CASE_RESULT_VAR, (false), __FILE__, __LINE__, ETestFailureType::Assertion); \
            return;                                                                                     \
        }                                                                                               \
    } while (false)

#define TEST_ASSERT_FALSE(...)                                                                          \
    do {                                                                                                \
        if (__VA_ARGS__) {                                                                              \
            ExpectImpl(TEST_CASE_RESULT_VAR, (false), __FILE__, __LINE__, ETestFailureType::Assertion); \
            return;                                                                                     \
        }                                                                                               \
    } while (false)

#define TEST_ASSERT_EQ(x, y) TEST_ASSERT_TRUE(x == y)
#define TEST_ASSERT_NEQ(x, y) TEST_ASSERT_TRUE(x != y)

#define TEST_ASSERT_STR_EQ(x, y) TEST_ASSERT_TRUE(CompareString(x, y))
#define TEST_ASSERT_STR_NEQ(x, y) TEST_ASSERT_TRUE(!CompareString(x, y))

#define TEST_ASSERT_FLOAT_EQ(x, y, maxUlps) TEST_ASSERT_TRUE(CompareFloat(x, y, maxUlps))
#define TEST_ASSERT_FLOAT_NEQ(x, y, maxUlps) TEST_ASSERT_TRUE(!CompareFloat(x, y, maxUlps))

#define TEST_ASSERT_DOUBLE_EQ(x, y, maxUlps) TEST_ASSERT_TRUE(CompareDouble(x, y, maxUlps))
#define TEST_ASSERT_DOUBLE_NEQ(x, y, maxUlps) TEST_ASSERT_TRUE(!CompareDouble(x, y, maxUlps))

#define TEST_COMMENT(...)