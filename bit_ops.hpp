#pragma once

// LSB - least significant bit
// MSB - most significant bit
//
// Numbering goes from LSB to MSB
//
//                MSB           LSB
//                 |             |
// Number       -  0 1 0 1 0 1 1 0
// Bit position -  7 6 5 4 3 2 1 0
//
// Shift left (<<) moves bits towards MSB
// Shift right(>>) moves bits towards LSB

#include <cstddef>
#include <limits>

namespace abf
{
namespace detail
{

// Get left mask starting from "begin" position
//             begin
//               |
// Example - 11111000
template<typename T>
constexpr T lmask(std::size_t begin) noexcept
{
  return std::numeric_limits<T>::max() << begin;
}

// Get right mask ending in "end" position
//              end
//               |
// Example - 00000111
template<typename T>
constexpr T rmask(std::size_t end) noexcept
{
  return std::numeric_limits<T>::max() >>
         (std::numeric_limits<T>::digits - end);
}

// Combination of lmask and rmask
//         end begin
//          |   |
// Example 00111100
template<typename T>
constexpr T lrmask(std::size_t begin, std::size_t end) noexcept
{
  return lmask<T>(begin) & rmask<T>(end);
}

// Recursive merge of multiple masks
template<typename T, std::size_t BEGIN, std::size_t END, std::size_t... Tail>
struct mask
{
  static constexpr T f() noexcept
  {
    return lrmask<T>(BEGIN, END) | mask<T, Tail...>::f();
  }
};

// End of recursion
template<typename T, std::size_t BEGIN, std::size_t END>
struct mask<T, BEGIN, END>
{
  static constexpr T f() noexcept { return lrmask<T>(BEGIN, END); }
};

// Shift integer from "SRC_POS" to "DEST_POS"
template<std::size_t SRC_POS, std::size_t DEST_POS, typename T>
constexpr T shift_to(T t) noexcept
{
  return static_cast<T>(
    SRC_POS < DEST_POS ? t << (DEST_POS - SRC_POS) : t >> (SRC_POS - DEST_POS));
}

}
} // namespace abf::detail
