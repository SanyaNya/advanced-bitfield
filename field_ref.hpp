#pragma once

#include "storage_ops.hpp"
#include "get_int_storage.hpp"
#include "bitcast.hpp"

#if __cplusplus > 201103L
#define CONSTEXPR_IF_NOT_CPP11 constexpr
#else
#define CONSTEXPR_IF_NOT_CPP11
#endif

namespace abf
{
namespace detail
{

template<typename int_type, typename T, std::size_t... Parts>
class field_ref
{
  static_assert(sizeof...(Parts) != 0, "Parts list must be non empty");

  static_assert(
    sizeof...(Parts) % 2 == 0,
    "Every part must contain begin and end");

  using TInt = get_int_storage_t<T>;

  int_type& storage;

public:
  constexpr field_ref(int_type& storage_) noexcept
    : storage(storage_)
  {
  }

  constexpr operator T() const noexcept
  {
    return bit_cast__<T>(
      static_cast<TInt>(get_from_storage<int_type, Parts...>(storage)));
  }

  CONSTEXPR_IF_NOT_CPP11 field_ref& operator=(T t) noexcept
  {
    storage = set_storage<int_type, Parts...>(
      storage, static_cast<int_type>(bit_cast__<TInt>(t)));

    return *this;
  }
};

}
} // namespace abf::detail
