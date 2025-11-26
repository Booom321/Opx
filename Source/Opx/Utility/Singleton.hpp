#pragma once

#include "../Assert.hpp"
#include "Utility.hpp"

OPX_NAMESPACE_BEGIN

template <typename T>
class Singleton {
protected:
    Singleton() = default;
    ~Singleton() = default;

private:
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) noexcept = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) noexcept = delete;

public:
    template <typename U /* = T */, typename... ArgTypes>
    static OPX_INLINE void Create(ArgTypes&&... args) {
        OPX_ASSERT(!instance);
        instance = new U(Forward<ArgTypes>(args)...);
    }

    template <typename U = T>
    static OPX_INLINE U* Get() noexcept {
        if constexpr (IsSame_V<U, T>) {
            return instance;
        } else {
            return dynamic_cast<U*>(instance);
        }
    }

    static OPX_INLINE void Delete() {
        if (instance) {
            delete instance;
            instance = nullptr;
        }
    }

protected:
    static T* instance;
};

template <typename T>
T* Singleton<T>::instance = nullptr;

OPX_NAMESPACE_END