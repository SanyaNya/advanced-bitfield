#include <iostream>
#include "../advanced_bitfield.hpp"

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

inline void print(S s)
{
    std::cout 
        << std::hex
        << "s:\n"
        << "s.base() = "    << s.base()    << "\n"
        << "s.offset1() = " << static_cast<unsigned>(s.offset1()) << "\n"
        << "s.offset2() = " << static_cast<unsigned>(s.offset2()) << "\n"
        << "s.addr1() = "   << s.addr1()   << "\n"
        << "s.addr2() = "   << s.addr2()   << "\n"
        << "s.is_odd1() = " << s.is_odd1() << "\n"
        << "s.is_odd2() = " << s.is_odd2() << "\n";
}

int main()
{
    S s;
    print(s);

    std::cout << "\nset s.base() = 0x8000\n";
    s.base() = 0x8000;

    std::cout << "set s.offset1() = 0xa0\n";
    s.offset1() = 0xa0;

    std::cout << "set s.offset2() = 0x41\n\n";
    s.offset2() = 0x41;

    print(s);
}
