#pragma once

#if __cplusplus >= 202002L
#include <bit>
#else
#include <cstring>
#endif

#include <type_traits>

namespace abf{ namespace detail
{

template<typename To, typename From>
#if __cplusplus >= 202002L
constexpr 
#endif
typename std::enable_if<!std::is_convertible<From, To>::value, To>::type 
bit_cast__(const From& from) noexcept
{
#if __cplusplus >= 202002L
    return std::bit_cast<To, From>(from);
#else
    static_assert(sizeof(To) == sizeof(From),
            "Size of To must be equal to size of From");
    static_assert(std::is_trivially_copyable<To>::value, 
            "To must be trivially copyable");
    static_assert(std::is_trivially_copyable<From>::value,
            "From must be trivially copyable");

    To to;
    std::memcpy(&to, &from, sizeof(To));

    return to;
#endif
}

template<typename To, typename From>
constexpr 
typename std::enable_if<std::is_convertible<From, To>::value, To>::type
bit_cast__(const From& from) noexcept
{
    static_assert(noexcept(static_cast<To>(from)),
            "Conversion must be noexcept");

    return static_cast<To>(from);
}

}} //namespace abf::detail
