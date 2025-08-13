#pragma once

#include <type_traits>
#include <half/half.hpp>

using half_t  = half_float::half;
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

    if(~u.int32 & 0x7f800000)
    {
        // When the exponent bits are not all 1s, then the value is zero, normal,
        // or subnormal. We round the bfloat16 mantissa up by adding 0x7FFF, plus
        // 1 if the least significant bit of the bfloat16 mantissa is 1 (odd).
        // This causes the bfloat16's mantissa to be incremented by 1 if the 16
        // least significant bits of the float mantissa are greater than 0x8000,
        // or if they are equal to 0x8000 and the least significant bit of the
        // bfloat16 mantissa is 1 (odd). This causes it to be rounded to even when
        // the lower 16 bits are exactly 0x8000. If the bfloat16 mantissa already
        // has the value 0x7f, then incrementing it causes it to become 0x00 and
        // the exponent is incremented by one, which is the next higher FP value
        // to the unrounded bfloat16 value. When the bfloat16 value is subnormal
        // with an exponent of 0x00 and a mantissa of 0x7F, it may be rounded up
        // to a normal value with an exponent of 0x01 and a mantissa of 0x00.
        // When the bfloat16 value has an exponent of 0xFE and a mantissa of 0x7F,
        // incrementing it causes it to become an exponent of 0xFF and a mantissa
        // of 0x00, which is Inf, the next higher value to the unrounded value.
        u.int32 += 0x7fff + ((u.int32 >> 16) & 1); // Round to nearest, round to even
    }
    else if(u.int32 & 0xffff)
    {
        // When all of the exponent bits are 1, the value is Inf or NaN.
        // Inf is indicated by a zero mantissa. NaN is indicated by any nonzero
        // mantissa bit. Quiet NaN is indicated by the most significant mantissa
        // bit being 1. Signaling NaN is indicated by the most significant
        // mantissa bit being 0 but some other bit(s) being 1. If any of the
        // lower 16 bits of the mantissa are 1, we set the least significant bit
        // of the bfloat16 mantissa, in order to preserve signaling NaN in case
        // the bloat16's mantissa bits are all 0.
        u.int32 |= 0x10000; // Preserve signaling NaN
    }
    return uint16_t(u.int32 >> 16);
}

// convert bfp16 to fp32
template <>
constexpr double type_convert<double, bhalf_t>(bhalf_t x)
{
    union
    {
        uint32_t int32;
        float fp32;
    } u = {uint32_t(x) << 16};

    return static_cast<double>(u.fp32);
}

template <>
constexpr bhalf_t type_convert<bhalf_t, double>(double x)
{
    return type_convert<bhalf_t>(static_cast<float>(x));
}
