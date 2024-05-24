#pragma once

// Get int int_type with size corresponding to "T" type

#include <type_traits>
#include <cstdint>

namespace abf
{
namespace detail
{

template<typename T>
struct get_int_storage
{
  using type = typename std::conditional<
    sizeof(T) <= sizeof(std::uint8_t),
    std::uint8_t,
    typename std::conditional<
      sizeof(T) <= sizeof(std::uint16_t),
      std::uint16_t,
      typename std::conditional<
        sizeof(T) <= sizeof(std::uint32_t),
        std::uint32_t,
        typename std::
          conditional<sizeof(T) <= sizeof(std::uint64_t), std::uint64_t, void>::
            type>::type>::type>::type;

  static_assert(
    !std::is_same<type, void>::value,
    "sizeof(T) can`t be greater than sizeof(std::uint64_t)");
};

template<typename T>
using get_int_storage_t = typename get_int_storage<T>::type;

}
} // namespace abf::detail
