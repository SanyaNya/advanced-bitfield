#include "doctest/doctest.h"
#include "advanced_bitfield.hpp"

namespace x86
{
namespace Segment
{

enum class Type : std::uint8_t
{
  Data,
  Code
};

enum class Readable : std::uint8_t
{
  No,
  Yes
};

enum class Writable : std::uint8_t
{
  No,
  Yes
};

enum class ConformingPrivilege : std::uint8_t
{
  Equal_Ring,
  EqualLess_Ring
};

enum class Privilege : std::uint8_t
{
  Kernel = 0,
  User = 3
};

enum class Direction : std::uint8_t
{
  Up,
  Down
};

template<Type type>
struct Access;

template<>
class Access<Type::Code>
  : public abf::bitfield<Access<Type::Code>, std::uint8_t>
{
  using bf = abf::bitfield<Access<Type::Code>, std::uint8_t>;
  using bf::bf;

public:
  ABF_FIELD(bf, bool,                accessed,             0, 1)
  ABF_FIELD(bf, Readable,            readable,             1, 2)
  ABF_FIELD(bf, ConformingPrivilege, conforming_privilege, 2, 3)
  ABF_FIELD(bf, Type,                type,                 3, 4)
  ABF_FIELD(bf, bool,                always1,              4, 5)
  ABF_FIELD(bf, Privilege,           privilege,            5, 7)
  ABF_FIELD(bf, bool,                present,              7, 8)

  constexpr Access(
    readable_b read,
    conforming_privilege_b conf,
    privilege_b privil) noexcept
    : ABF_CTOR(
        bf,
        accessed_b(0),
        read,
        conf,
        type_b(Type::Code),
        always1_b(1),
        privil,
        present_b(1))
  {
  }
};

template<>
class Access<Type::Data>
  : public abf::bitfield<Access<Type::Data>, std::uint8_t>
{
  using bf = abf::bitfield<Access<Type::Data>, std::uint8_t>;
  using bf::bf;

public:
  ABF_FIELD(bf, bool,      accessed,  0, 1)
  ABF_FIELD(bf, Writable,  writable,  1, 2)
  ABF_FIELD(bf, Direction, direction, 2, 3)
  ABF_FIELD(bf, Type,      type,      3, 4)
  ABF_FIELD(bf, bool,      always1,   4, 5)
  ABF_FIELD(bf, Privilege, privilege, 5, 7)
  ABF_FIELD(bf, bool,      present,   7, 8)

  constexpr Access(
    writable_b write,
    direction_b dir,
    privilege_b privil) noexcept
    : ABF_CTOR(
        bf,
        accessed_b(0),
        write,
        dir,
        type_b(Type::Data),
        always1_b(1),
        privil,
        present_b(1))
  {
  }
};

enum class Size : std::uint8_t
{
  bit16,
  bit32
};

enum class LimitGranularity : std::uint8_t
{
  byte1,
  kib4
};

class Flags : public abf::bitfield<Flags, std::uint8_t>
{
  using bf = abf::bitfield<Flags, std::uint8_t>;
  using bf::bf;

public:
  ABF_FIELD(bf, bool,             avl,         0, 1)
  ABF_FIELD(bf, bool,             longmode,    1, 2)
  ABF_FIELD(bf, Size,             size,        2, 3)
  ABF_FIELD(bf, LimitGranularity, granularity, 3, 4)

  constexpr Flags(size_b s, granularity_b lg) noexcept
    : ABF_CTOR(bf, avl_b(0), longmode_b(0), s, lg)
  {
  }
};

template<Type type>
class Descriptor : public abf::bitfield<Descriptor<type>, std::uint64_t>
{
  using bf = abf::bitfield<Descriptor<type>, std::uint64_t>;
  using bf::bf;

public:
  ABF_FIELD(bf, 
        std::uint32_t, base,   16, 32,
                               32, 40,
                               56, 64)

  ABF_FIELD(bf, 
        std::uint32_t, lim,     0, 16, 
                               48, 52)

  ABF_FIELD(bf, 
        Access<type>,  access, 40, 48)

  ABF_FIELD(bf, 
        Flags,         flags,  52, 56)

  constexpr Descriptor(base_b b, lim_b l, access_b a, flags_b f) noexcept
    : ABF_CTOR(bf, b, l, a, f)
  {
  }
};

}
} // namespace x86::Segment

constexpr std::uint64_t gdt[]{
  0,
  x86::Segment::Descriptor<x86::Segment::Type::Code>{
    0x0,
    0xffffffff,
    x86::Segment::Access<x86::Segment::Type::Code>{
      x86::Segment::Readable::Yes,
      x86::Segment::ConformingPrivilege::Equal_Ring,
      x86::Segment::Privilege::Kernel },
    x86::Segment::Flags{ x86::Segment::Size::bit32,
                         x86::Segment::LimitGranularity::kib4 } },

  x86::Segment::Descriptor<x86::Segment::Type::Data>{
    0x0,
    0xffffffff,
    x86::Segment::Access<x86::Segment::Type::Data>{
      x86::Segment::Writable::Yes,
      x86::Segment::Direction::Up,
      x86::Segment::Privilege::Kernel },
    x86::Segment::Flags{ x86::Segment::Size::bit32,
                         x86::Segment::LimitGranularity::kib4 } }
};

TEST_CASE("x86 Segment descriptor")
{
  CHECK(gdt[1] == 0x00cf9a000000ffff);
  CHECK(gdt[2] == 0x00cf92000000ffff);
}
