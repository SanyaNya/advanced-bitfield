#pragma once

#include "storage_ops.hpp"
#include "get_int_storage.hpp"
#include "bitcast.hpp"

namespace abf
{
namespace detail
{

template<typename T, std::size_t... Parts>
class holder
{
  using TInt = get_int_storage_t<T>;

  T value;

public:
  constexpr holder(T v) noexcept
    : value(v)
  {
  }

  template<typename int_type>
  constexpr int_type set(int_type storage) const noexcept
  {
    return set_storage<int_type, Parts...>(
      storage, static_cast<int_type>(bit_cast__<TInt>(value)));
  }
};

}
} // namespace abf::detail
