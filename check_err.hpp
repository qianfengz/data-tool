// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2025, Advanced Micro Devices, Inc. All rights reserved.

#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <limits>
#include <type_traits>
#include <vector>

#include "type_convert.hpp"

namespace data_tool {

template <typename T>
bool check_err(
    const T* out, const T* ref, int num_elements, const std::string& msg, double rtol, double atol)
{
    const auto is_infinity_error = [=](auto o, auto r) {
        const bool either_not_finite = !std::isfinite(o) || !std::isfinite(r);

        return either_not_finite;
    };

    std::vector<int> infinite_element_indexes;
    std::vector<int> errorneous_element_indexes;

    for(std::size_t i = 0; i < num_elements; ++i)
    {
        const double o = type_convert<double>(out[i]);
        const double r = type_convert<double>(ref[i]);

        if(is_infinity_error(o, r))
        {
            infinite_element_indexes.push_back(i);
            continue;
        };

        double err = std::abs(o - r);
        if(err > atol + rtol * std::abs(r))
        {
            errorneous_element_indexes.push_back(i);
            continue;
        }
    }

    if(!infinite_element_indexes.empty())
    {
        std::cout << infinite_element_indexes.size() << " infinite values found:" << std::endl;

        std::cout << std::fixed << std::setprecision(10);
        for(int i : infinite_element_indexes)
            std::cout << i << ":  " << type_convert<float>(out[i]) << "  "
                      << type_convert<float>(ref[i]) << std::endl;
    }

    if(!errorneous_element_indexes.empty())
    {
        std::cout << errorneous_element_indexes.size() << " errorneous values found:" << std::endl;

        std::cout << std::fixed << std::setprecision(12);
        for(int i : errorneous_element_indexes)
        {
            const double o = type_convert<double>(out[i]);
            const double r = type_convert<double>(ref[i]);

            auto abs_err = std::abs(o - r);
            auto rel_err = abs_err / std::abs(r);
            std::cout << i << ":  " << o << "  " << r << " abs_error: " << abs_err
                      << " rel_error: " << rel_err << std::endl;
        }
    }

    return !infinite_element_indexes.empty() || !errorneous_element_indexes.empty();
}

} // namespace data_tool
