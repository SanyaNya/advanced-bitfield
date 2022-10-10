#include <iostream>

//Include library header
#include "../advanced_bitfield.hpp"

//First of all inherit from abf::bitfield<YOUR_CLASS, INTEGRAL_ALIAS>
class S : public abf::bitfield<S, std::uint32_t>
{
    using bf = abf::bitfield<S, std::uint32_t>; //Convenient alias for base
    using bf::bf; //Using base constructors(default, from integral)

public:
    //Declaring fields
    //Bits indexed from lsb to msb
    //ABF_FIELD(BASE_BITFIELD_CLASS, FIELD_TYPE, FIELD_NAME, PART0_BEGIN_BIT, PART0_END_BIT, [PART1_BEGIN_BIT, PART1_END_BIT]...)
    ABF_FIELD(bf, bool,           f1, 0,  1 )
    ABF_FIELD(bf, unsigned short, n1, 1,  17)
    ABF_FIELD(bf, unsigned short, n2, 17, 31)
    ABF_FIELD(bf, bool,           f2, 31, 32)

    //If you want custom constructor:
    //ABF_CTOR(BASE_BITFIELD_CLASS, FIELD_NAME_b(VALUE)...)
    constexpr S(f1_b f1_, n1_b n1_, n2_b n2_, f2_b f2_) noexcept :
        ABF_CTOR(bf, f1_, n1_, n2_, f2_) {}
};

//Using in constexpr
constexpr S get_s()
{
    return S(1, 0xffff, 0x1bcd, 1);
}

int main()
{
    //To access fields(get/set) use OBJECT.FIELD_NAME()

    constexpr S s = get_s();

    std::cout << std::hex
              << "s representation: " << static_cast<std::uint32_t>(s) << "\n"
              << "s.f1() = " << s.f1() << "\n"
              << "s.n1() = " << s.n1() << "\n"
              << "s.n2() = " << s.n2() << "\n"
              << "s.f2() = " << s.f2() << "\n";
}
