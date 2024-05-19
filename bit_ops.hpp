#pragma once

#include <cstddef>
#include <limits>

namespace abf
{
namespace detail
{

template<typename T>
constexpr T lmask(std::size_t begin) noexcept
{
  return std::numeric_limits<T>::max() << begin;
}

template<typename T>
constexpr T rmask(std::size_t end) noexcept
{
  return std::numeric_limits<T>::max() >>
         (std::numeric_limits<T>::digits - end);
}

template<typename T, std::size_t BEGIN, std::size_t END, std::size_t... Tail>
struct mask
{
  static constexpr T f() noexcept
  {
    return (lmask<T>(BEGIN) & rmask<T>(END)) | mask<T, Tail...>::f();
  }
};

template<typename T, std::size_t BEGIN, std::size_t END>
struct mask<T, BEGIN, END>
{
  static constexpr T f() noexcept { return lmask<T>(BEGIN) & rmask<T>(END); }
};

template<std::size_t SRC_POS, std::size_t DEST_POS, typename T>
constexpr T shift_to(T t) noexcept
{
  return SRC_POS < DEST_POS ? t << (DEST_POS - SRC_POS)
                            : t >> (SRC_POS - DEST_POS);
}

}
} // namespace abf::detail
