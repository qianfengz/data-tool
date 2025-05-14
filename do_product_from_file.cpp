#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

#include "type_convert.hpp"

int main(int argc, char* argv[])
{
    if(argc < 5)
    {
        std::cout << "Usage: " << argv[0]
                  << "<fp32/fp16/bf16> <file1> <file2> <number of data> <1/0 show accumulation>"
                  << std::endl;
        throw std::runtime_error("Invalid argument!");
    }

    std::string precision(argv[1]);

    std::ifstream f1(argv[2]);
    std::ifstream f2(argv[3]);

    int num_elements = atoi(argv[4]);
    float result     = 0.0f;

    bool show_accumulation = false;

    if(argc > 5 && atoi(argv[5]) > 0)
        show_accumulation = true;

    for(int i = 0; i < num_elements; i++)
    {
        float data1, data2;
        float data1_conved, data2_conved;

        f1 >> data1;
        f2 >> data2;

        if(precision == "fp32")
        {
            result += data1 * data2;
            data1_conved = data1;
            data2_conved = data2;
        }
        else if(precision == "bf16")
        {
            bhalf_t val1 = type_convert<bhalf_t>(data1);
            bhalf_t val2 = type_convert<bhalf_t>(data2);

            result += type_convert<float>(val1) * type_convert<float>(val2);
            data1_conved = type_convert<float>(val1);
            data2_conved = type_convert<float>(val2);
        }
        else if(precision == "fp16")
        {
            half_t val1 = type_convert<half_t>(data1);
            half_t val2 = type_convert<half_t>(data2);

            result += type_convert<float>(val1) * type_convert<float>(val2);
            data1_conved = type_convert<float>(val1);
            data2_conved = type_convert<float>(val2);
        }
        else
            throw std::runtime_error("Not supported type!");

        if(show_accumulation)
            std::cout << i << ": " << std::fixed << result << std::endl;
        else
            std::cout << i << ": " << std::fixed << data1_conved << "  " << data2_conved
                      << std::endl;
    }

    std::cout << "Dot product: " << result << std::endl;
};
