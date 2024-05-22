#include "doctest/doctest.h"
#include "advanced_bitfield.hpp"

class S : public abf::bitfield<S, std::uint32_t>
{
  using bf = abf::bitfield<S, std::uint32_t>;
  using bf::bf;

public:
  ABF_FIELD(bf, std::uint16_t, base,    0,  16)
  ABF_FIELD(bf, std::uint8_t,  offset1, 16, 24)
  ABF_FIELD(bf, std::uint8_t,  offset2, 24, 32)

  ABF_CONST_FIELD(bf, 
        std::uint32_t, addr1, 16, 24, //(base << 8) | offset1
                              0,  16) //

  ABF_CONST_FIELD(bf, 
        std::uint32_t, addr2, 24, 32, //(base << 8) | offset2
                              0,  16)

  ABF_CONST_FIELD(bf, std::uint32_t, is_odd1, 16, 17);
  ABF_CONST_FIELD(bf, std::uint32_t, is_odd2, 24, 25);
};

TEST_CASE("Bitfield with overlapping parts")
{
  S s;

  SUBCASE("Check init")
  {
    CHECK(static_cast<std::uint32_t>(s) == 0);
    CHECK(s.base() == 0);
    CHECK(s.offset1() == 0);
    CHECK(s.offset2() == 0);
    CHECK(s.addr1() == 0);
    CHECK(s.addr2() == 0);
    CHECK(s.is_odd1() == 0);
    CHECK(s.is_odd2() == 0);
  }

  s.base() = 0x8000;
  s.offset1() = 0xa0;
  s.offset2() = 0x41;

  SUBCASE("Check after setting some")
  {
    CHECK(static_cast<std::uint32_t>(s) == 0x41a08000);
    CHECK(s.base() == 0x8000);
    CHECK(s.offset1() == 0xa0);
    CHECK(s.offset2() == 0x41);
    CHECK(s.addr1() == 0x8000a0);
    CHECK(s.addr2() == 0x800041);
    CHECK(s.is_odd1() == 0);
    CHECK(s.is_odd2() == 1);
  }
}
