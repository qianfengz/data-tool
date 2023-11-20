#pragma once

#include <type_traits>
#include <half/half.hpp>

using half_t = half_float::half; 
using bhalf_t = ushort;

// Convert X to Y
template <typename Y, typename X>
constexpr Y type_convert(X x)
{
    static_assert(!std::is_reference_v<Y> && !std::is_reference_v<X>);

    return static_cast<Y>(x);
}

// convert bfp16 to fp32
template <>
constexpr float type_convert<float, bhalf_t>(bhalf_t x)
{
    union
    {
        uint32_t int32;
        float fp32;
    } u = {uint32_t(x) << 16};

    return u.fp32;
}

template <>
constexpr bhalf_t type_convert<bhalf_t, float>(float x)
{
    union
    {
        float fp32;
        uint32_t int32;
    } u = {x};

    return uint16_t(u.int32 >> 16);
}

