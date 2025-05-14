#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <cassert>
#include <type_traits>

#include "type_convert.hpp"

template <typename T>
bool readBufferFromFile(T* data, size_t dataNumItems, const std::string& fileName)
{
    std::ifstream infile(fileName, std::ios::binary);
    if(infile)
    {
        infile.read(reinterpret_cast<char*>(data), dataNumItems * sizeof(T));
        infile.close();
    }
    else
    {
        throw std::runtime_error("could not open the file for reading");
    }

    return true;
}

template <typename T>
void show_integer_data_from_file(int numElements, const char* filename)
{
    T* dataBuffer = new T[numElements];

    if(readBufferFromFile(dataBuffer, numElements, filename))
    {
        for(int i = 0; i < numElements; i++)
        {
            if constexpr(sizeof(T) >= 2)
                std::cout << i << " : " << dataBuffer[i] << std::endl;
            else
                std::cout << i << " : " << static_cast<int>(dataBuffer[i]) << std::endl;
        };
    };

    delete[] dataBuffer;
};

template <typename T>
void show_floating_point_data_from_file(int numElements, const char* filename)
{
    T* dataBuffer = new T[numElements];

    if(readBufferFromFile(dataBuffer, numElements, filename))
    {
        for(int i = 0; i < numElements; i++)
        {
            float value = type_convert<float>(dataBuffer[i]);

            if(std::isinf(value) || std::isnan(value))
            {
                std::cout << i << " : "
                          << "invalid value" << std::endl;
                continue;
            }
            std::cout << i << " : " << std::fixed << value << std::endl;
        };
    };

    delete[] dataBuffer;
};

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <fp64/fp32/fp16/bf16/int32/int16/int8/uint32/uint16/uint8> <file> <number "
                     "of elements> "
                  << std::endl;

        throw std::runtime_error("Invalid command argument!");
    };

    std::string precision(argv[1]);

    if(precision != "fp64" && precision != "fp32" && precision != "fp16" && precision != "bf16" &&
       precision != "int8" && precision != "int32" && precision != "int16" &&
       precision != "uint32" && precision != "uint16" && precision != "uint8")
        throw std::runtime_error("Invalid precison of input data specified!");

    int numElements = atoi(argv[3]);

    assert(numElements > 0);

    if(precision == "fp64")
    {
        show_floating_point_data_from_file<double>(numElements, argv[2]);
    }
    else if(precision == "fp32")
    {
        show_floating_point_data_from_file<float>(numElements, argv[2]);
    }
    else if(precision == "fp16")
    {
        show_floating_point_data_from_file<half_t>(numElements, argv[2]);
    }
    else if(precision == "bf16")
    {
        show_floating_point_data_from_file<bhalf_t>(numElements, argv[2]);
    }
    else if(precision == "int32")
    {
        show_integer_data_from_file<int32_t>(numElements, argv[2]);
    }
    else if(precision == "uint32")
    {
        show_integer_data_from_file<uint32_t>(numElements, argv[2]);
    }
    else if(precision == "int16")
    {
        show_integer_data_from_file<int16_t>(numElements, argv[2]);
    }
    else if(precision == "uint16")
    {
        show_integer_data_from_file<uint16_t>(numElements, argv[2]);
    }
    else if(precision == "int8")
    {
        show_integer_data_from_file<int8_t>(numElements, argv[2]);
    }
    else if(precision == "uint8")
    {
        show_integer_data_from_file<uint8_t>(numElements, argv[2]);
    }
};
