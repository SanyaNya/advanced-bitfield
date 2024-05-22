#include "doctest/doctest.h"
#include "advanced_bitfield.hpp"

class S : public abf::bitfield<S, std::uint32_t>
{
  using bf = abf::bitfield<S, std::uint32_t>;
  using bf::bf;

public:
  ABF_FIELD(bf, bool,           f1, 0,  1 )
  ABF_FIELD(bf, unsigned short, n1, 1,  17)
  ABF_FIELD(bf, unsigned short, n2, 17, 31)
  ABF_FIELD(bf, bool,           f2, 31, 32)

  constexpr S(f1_b f1_, n1_b n1_, n2_b n2_, f2_b f2_) noexcept
    : ABF_CTOR(bf, f1_, n1_, n2_, f2_)
  {
  }
};

// Using in constexpr
constexpr S get_s()
{
  return S(1, 0xffff, 0x1bcd, 1);
}

TEST_CASE("Simple bitfield")
{
  constexpr S s = get_s();

  CHECK(static_cast<std::uint32_t>(s) == 0xb79bffff);
  CHECK(s.f1() == 1);
  CHECK(s.n1() == 0xffff);
  CHECK(s.n2() == 0x1bcd);
  CHECK(s.f2() == 1);
}
