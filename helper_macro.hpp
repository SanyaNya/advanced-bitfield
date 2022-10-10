#pragma once

#include "field_ref.hpp"
#include "holder.hpp"

#define ABF_CONST_FIELD(BF, T, NAME, ...)                       \
    static_assert(std::is_trivially_copyable<T>::value,         \
            "T must be trivially copyable");                    \
                                                                \
    constexpr T NAME() const noexcept                           \
    {                                                           \
        return                                                  \
            static_cast<T>(                                     \
                abf::detail::field_ref<                         \
                    typename std::add_const<                    \
                        typename BF::Alias>::type,              \
                    T,                                          \
                    __VA_ARGS__>(this->BF::storage));           \
    }

#define ABF_FIELD(BF, T, NAME, ...)                             \
    ABF_CONST_FIELD(BF, T, NAME, __VA_ARGS__)                   \
                                                                \
    CONSTEXPR_IF_NOT_CPP11                                      \
    abf::detail::field_ref<                                     \
        typename BF::Alias,                                     \
        T,                                                      \
        __VA_ARGS__> NAME() noexcept                            \
    {                                                           \
        return                                                  \
            abf::detail::field_ref<                             \
                typename BF::Alias,                             \
                T,                                              \
                __VA_ARGS__>(this->BF::storage);                \
    }                                                           \
                                                                \
    using NAME##_b = abf::detail::holder<T, __VA_ARGS__>;

#define ABF_CTOR(BF, ...) BF(typename BF::ctor_tag{}, __VA_ARGS__)

