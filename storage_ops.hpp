#pragma once

// Part - pair of begin and end
// Field consists of multiple non-overlapping parts

#include <cstddef>
#include "bit_ops.hpp"

namespace abf
{
namespace detail
{

// Write to storage
// ================

// Writing of field to storage:
// Writing gets done by sequentially reading parts from "src"(from LSB to MSB),
// positioning them at "BEGIN" part parameter,
// merging them all together
// and setting the resulting value to storage

// Get part from number
// Example:
//            END BEGIN
//             |    |
// Number - 1010100110011101
// Part   - 0000100110000000
template<std::size_t BEGIN, std::size_t END, typename int_type>
constexpr int_type get_part(int_type i) noexcept
{
  return i & mask<int_type, BEGIN, END>::f();
}

// Get part from "src" starting at "OFFSET" and position it at "BEGIN"
template<
  std::size_t OFFSET,
  std::size_t BEGIN,
  std::size_t END,
  typename int_type>
constexpr int_type get_part_from_src(int_type src) noexcept
{
  return get_part<BEGIN, END>(shift_to<OFFSET, BEGIN>(src));
}

// Recursively merge all "src" parts
template<
  std::size_t OFFSET,
  std::size_t BEGIN,
  std::size_t END,
  std::size_t... Tail>
struct merge_parts_from_src
{
  template<typename int_type>
  static constexpr int_type f(int_type src) noexcept
  {
    return get_part_from_src<OFFSET, BEGIN, END>(src) |
           merge_parts_from_src<OFFSET + (END - BEGIN), Tail...>::f(src);
  }
};

// merge_parts_from_src recursion end
template<std::size_t OFFSET, std::size_t BEGIN, std::size_t END>
struct merge_parts_from_src<OFFSET, BEGIN, END>
{
  template<typename int_type>
  static constexpr int_type f(int_type src) noexcept
  {
    return get_part_from_src<OFFSET, BEGIN, END>(src);
  }
};

// Apply zero mask to parts of storage
template<typename int_type, std::size_t... Parts>
constexpr int_type zero_storage_parts(int_type storage) noexcept
{
  return storage & ~mask<int_type, Parts...>::f();
}

// Writing field to storage
template<typename int_type, std::size_t... Parts>
constexpr int_type set_storage(int_type storage, int_type src) noexcept
{
  return zero_storage_parts<int_type, Parts...>(storage) |
         merge_parts_from_src<0, Parts...>::f(src);
}

// Read from storage
// =================

// Reading of field from storage:
// Reading gets done by getting parts from storage
// and sequntially writing them(from LSB to MSB) to result

// Get part from "storage" starting at "BEGIN" and position it at "OFFSET"
template<
  std::size_t OFFSET,
  std::size_t BEGIN,
  std::size_t END,
  typename int_type>
constexpr int_type get_part_from_storage(int_type storage) noexcept
{
  return get_part<OFFSET, OFFSET + (END - BEGIN)>(
    shift_to<BEGIN, OFFSET>(storage));
}

// Recursively merge all "storage" parts
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
    return get_part_from_storage<OFFSET, BEGIN, END>(storage) |
           merge_parts_from_storage<int_type, OFFSET + (END - BEGIN), Tail...>::
             f(storage);
  }
};

// merge_parts_from_storage recursion end
template<
  typename int_type,
  std::size_t OFFSET,
  std::size_t BEGIN,
  std::size_t END>
struct merge_parts_from_storage<int_type, OFFSET, BEGIN, END>
{
  static constexpr int_type f(int_type storage) noexcept
  {
    return get_part_from_storage<OFFSET, BEGIN, END>(storage);
  }
};

// Reading field from storage
template<typename int_type, std::size_t... Parts>
constexpr int_type get_from_storage(int_type storage) noexcept
{
  return merge_parts_from_storage<int_type, 0, Parts...>::f(storage);
}

}
} // namespace abf::detail
