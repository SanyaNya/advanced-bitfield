#pragma once

#include <cstddef>
#include "bit_ops.hpp"

namespace abf
{
namespace detail
{

template<
  typename int_type,
  std::size_t OFFSET,
  std::size_t BEGIN,
  std::size_t END,
  std::size_t... Tail>
struct merge_parts_from_src
{
  static constexpr int_type f(int_type src) noexcept
  {
    return (shift_to<OFFSET, BEGIN>(src) & mask<int_type, BEGIN, END>::f()) |
           merge_parts_from_src<int_type, OFFSET + (END - BEGIN), Tail...>::f(
             src);
  }
};

template<
  typename int_type,
  std::size_t OFFSET,
  std::size_t BEGIN,
  std::size_t END>
struct merge_parts_from_src<int_type, OFFSET, BEGIN, END>
{
  static constexpr int_type f(int_type src) noexcept
  {
    return shift_to<OFFSET, BEGIN>(src) & mask<int_type, BEGIN, END>::f();
  }
};

template<typename int_type, std::size_t... Parts>
constexpr int_type set_storage(int_type storage, int_type src) noexcept
{
  return (storage & ~mask<int_type, Parts...>::f()) |
         merge_parts_from_src<int_type, 0, Parts...>::f(src);
}

template<
  typename int_type,
  std::size_t OFFSET,
  std::size_t BEGIN,
  std::size_t END,
  std::size_t... Tail>
struct merge_parts_from_storage
{
  static constexpr int_type f(int_type storage) noexcept
  {
    return (shift_to<BEGIN, OFFSET>(storage) &
            mask<int_type, OFFSET, OFFSET + (END - BEGIN)>::f()) |
           merge_parts_from_storage<int_type, OFFSET + (END - BEGIN), Tail...>::
             f(storage);
  }
};

template<
  typename int_type,
  std::size_t OFFSET,
  std::size_t BEGIN,
  std::size_t END>
struct merge_parts_from_storage<int_type, OFFSET, BEGIN, END>
{
  static constexpr int_type f(int_type storage) noexcept
  {
    return shift_to<BEGIN, OFFSET>(storage) &
           mask<int_type, OFFSET, OFFSET + (END - BEGIN)>::f();
  }
};

template<typename int_type, std::size_t... Parts>
constexpr int_type get_from_storage(int_type storage) noexcept
{
  return merge_parts_from_storage<int_type, 0, Parts...>::f(storage);
}

}
} // namespace abf::detail
