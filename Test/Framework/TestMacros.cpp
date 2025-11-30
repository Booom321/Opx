#include "Framework/Framework.hpp"

Bool CreateTestCase(const Char* testSuiteName,
                    const Char* testCaseName,
                    TestCase::TestFunction func) {
    TestExecutor::Get()->GetTestSuiteByName(testSuiteName).AddTestCase(testCaseName, func);
    return true;
}

void ExpectImpl(TestCaseResult& result,
                Bool isTrue,
                const Char* file,
                Int32 line,
                ETestFailureType failureType) {
    if (isTrue) {
        return;
    }

    result.SetPassed(false);
    result.GetInfos().emplace_back(file, line, failureType);
}

Bool CompareString(const Char* lhs, const Char* rhs) {
    return strcmp(lhs, rhs) == 0;
}

template <typename U>
class ChooseUIntType;

template <>
class ChooseUIntType<Float> {
public:
    using Type = UInt32;
};

template <>
class ChooseUIntType<Double> {
public:
    using Type = UInt64;
};

template <typename T>
class FloatComparator {
public:
    using UInt = typename ChooseUIntType<T>::Type;

    static constexpr UInt kBitCount = 8 * sizeof(T);

    static constexpr UInt kSignBitMask = static_cast<UInt>(1) << (kBitCount - 1);

    static inline UInt FloatToBits(T x) {
        UInt bits;
        std::memcpy(&bits, &x, sizeof(bits));
        return bits;
    }

    static inline UInt SignMagnitude(UInt bits) {
        return (kSignBitMask & bits) ? (~bits + 1) : (kSignBitMask | bits);
    }

    static UInt UlpDistance(UInt lhsBits, UInt rhsBits) {
        const UInt lhsMag = SignMagnitude(lhsBits);
        const UInt rhsMag = SignMagnitude(rhsBits);
        return (lhsMag >= rhsMag) ? (lhsMag - rhsMag) : (rhsMag - lhsMag);
    }

    FloatComparator(T lhs, T rhs, UInt maxUlps) : mLhs(lhs), mRhs(rhs), mMaxUlps(maxUlps) {}

    Bool operator()() {
        if (std::isnan(mLhs) || std::isnan(mRhs)) {
            return false;
        }

        const UInt lhsBits = FloatToBits(mLhs);
        const UInt rhsBits = FloatToBits(mRhs);
        return UlpDistance(lhsBits, rhsBits) <= mMaxUlps;
    }

private:
    T mLhs;
    T mRhs;
    UInt mMaxUlps;
};

Bool CompareFloat(Float lhs, Float rhs, UInt32 maxUlps) {
    return FloatComparator<Float>{lhs, rhs, maxUlps}();
}

Bool CompareDouble(Double lhs, Double rhs, UInt64 maxUlps) {
    return FloatComparator<Double>{lhs, rhs, maxUlps}();
}