#pragma once

#include "storage_ops.hpp"

namespace abf{ namespace detail
{

template<typename T, std::size_t ... Parts>
class holder
{
    using TInt = detail::get_int_storage_t<T>;

    T value;

public:
    constexpr holder(T v) noexcept : value(v) {}

    template<typename int_type>
    constexpr int_type set_storage(int_type storage) const noexcept
    {
        return 
            detail::set_storage<
                int_type, Parts...>(
                    storage, 
                    static_cast<int_type>(
                        detail::bit_cast__<TInt>(value)));
    }
};

}} //namespace abf::detail
