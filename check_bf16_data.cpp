#include <iostream>
#include <stdexcept>
#include <cmath>
#include <cstdlib>

#include "type_convert.hpp"

int main(int argc, char* argv[])
{
    if(argc < 3 || argc % 2 == 0)
    {
        std::cerr << "Usage: " << argv[0] << " <float-point val_1 ... val_n val_n+1 ... val_2n>"
                  << std::endl;
        std::cerr << "Need to input 2n float-point values, we will calculate the dot-product of "
                     "two n-length vectors !"
                  << std::endl;
        throw std::runtime_error("Invalid command-line!");
    }

    int n = (argc - 1) / 2;

    std::vector<float> vector_fp32_0;
    std::vector<float> vector_fp32_1;

    for(int i = 0; i < n; i++)
    {
        float tmp0 = static_cast<float>(atof(argv[i + 1]));
        float tmp1 = static_cast<float>(atof(argv[i + n + 1]));

        vector_fp32_0.push_back(tmp0);
        vector_fp32_1.push_back(tmp1);
    };

    float dot_prod_first = 0.f;

    for(int i = 0; i < n; i++)
        dot_prod_first += vector_fp32_0[i] * vector_fp32_1[i];

    float dot_prod_second = 0.f;

    std::vector<bhalf_t> vector_bf16_0;
    std::vector<bhalf_t> vector_bf16_1;

    for(auto val : vector_fp32_0)
        vector_bf16_0.push_back(type_convert<bhalf_t>(val));

    for(auto val : vector_fp32_1)
        vector_bf16_1.push_back(type_convert<bhalf_t>(val));

    for(int i = 0; i < n; i++)
        dot_prod_second +=
            type_convert<float>(vector_bf16_0[i]) * type_convert<float>(vector_bf16_1[i]);

    float abs_error = std::abs(dot_prod_second - dot_prod_first);
    float rel_error = abs_error / std::abs(dot_prod_first);

    std::cout << "Dot-product using fp32: " << dot_prod_first << std::endl
              << "Dot-produt using bf16: " << dot_prod_second << std::endl
              << "Abs_error: " << abs_error << std::endl
              << "Rel_error: " << rel_error << std::endl;
};
