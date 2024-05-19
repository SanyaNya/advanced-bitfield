#pragma once

#include "helper_macro.hpp"

namespace abf
{

template<typename TImpl, typename TAlias>
class bitfield
{
  TAlias storage;

  friend TImpl;

  template<typename Holder, typename... Tail>
  struct init_storage
  {
    static constexpr TAlias f(TAlias s, Holder h, Tail... tail) noexcept
    {
      return init_storage<Tail...>::f(h.template set<TAlias>(s), tail...);
    }
  };

  template<typename Holder>
  struct init_storage<Holder>
  {
    static constexpr TAlias f(TAlias s, Holder h) noexcept
    {
      return h.template set<TAlias>(s);
    }
  };

  struct ctor_tag
  {
  };

  template<typename... Holders>
  constexpr bitfield(ctor_tag, Holders... holders) noexcept
    : storage(init_storage<Holders...>::f(0, holders...))
  {
  }

public:
  using Alias = TAlias;

  constexpr bitfield() noexcept
    : storage(0)
  {
    static_assert(
      sizeof(TImpl) == sizeof(Alias),
      "Size of bitfield must be equal to size of Alias");
  }

  constexpr bitfield(TAlias t) noexcept
    : storage(t)
  {
    static_assert(
      sizeof(TImpl) == sizeof(Alias),
      "Size of bitfield must be equal to size of Alias");
  }

  CONSTEXPR_IF_NOT_CPP11 TImpl& operator=(Alias t) noexcept
  {
    static_assert(
      sizeof(TImpl) == sizeof(Alias),
      "Size of bitfield must be equal to size of Alias");

    storage = t;
    return *static_cast<TImpl*>(this);
  }

  constexpr operator Alias() const noexcept
  {
    static_assert(
      sizeof(TImpl) == sizeof(Alias),
      "Size of bitfield must be equal to size of Alias");

    return storage;
  }
};

} // namespace abf
