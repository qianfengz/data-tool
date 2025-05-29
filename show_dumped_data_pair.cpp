#include <cassert>
#include <cstdlib>
#include <fstream>
#include <half/half.hpp>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

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
void show_floating_point_data_pair_from_file(int numElements,
                                             const char* filename1,
                                             const char* filename2)
{
    T* dataBuffer1 = new T[numElements];
    T* dataBuffer2 = new T[numElements];

    if(readBufferFromFile(dataBuffer1, numElements, filename1) &&
       readBufferFromFile(dataBuffer2, numElements, filename2))
    {
        std::cout << std::fixed << std::setprecision(11);

        for(int i = 0; i < numElements; i++)
        {
            float value1 = type_convert<float>(dataBuffer1[i]);
            float value2 = type_convert<float>(dataBuffer2[i]);
            if(std::isinf(value1) || std::isnan(value1))
            {
                std::cout << i << " : "
                          << "invalid value from file 1" << std::endl;
                continue;
            };
            if(std::isinf(value2) || std::isnan(value2))
            {
                std::cout << i << " : "
                          << "invalid value from file 1" << std::endl;
                continue;
            };

            std::cout << i << ":  " << value1 << "    " << value2 << std::endl;
        };
    };

    delete[] dataBuffer1;
    delete[] dataBuffer2;
};

template <typename T>
void show_integer_data_pair_from_file(int numElements, const char* filename1, const char* filename2)
{
    T* dataBuffer1 = new T[numElements];
    T* dataBuffer2 = new T[numElements];

    if(readBufferFromFile(dataBuffer1, numElements, filename1) &&
       readBufferFromFile(dataBuffer2, numElements, filename2))
    {
        for(int i = 0; i < numElements; i++)
        {
            if constexpr(sizeof(T) >= 2)
                std::cout << i << " : " << dataBuffer1[i] << " " << dataBuffer2[i] << std::endl;
            else
                std::cout << i << " : " << static_cast<int>(dataBuffer1[i]) << " "
                          << static_cast<int>(dataBuffer2[i]) << std::endl;
        };
    };

    delete[] dataBuffer1;
    delete[] dataBuffer2;
};

int main(int argc, char* argv[])
{
    if(argc != 5)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <fp64/fp32/fp16/bf16/int32/int16/int8/uint32/uint16/uint8> "
                     "<file1> <file2> <number of elements> "
                  << std::endl;

        throw std::runtime_error("Invalid command argument!");
    };

    std::string precision(argv[1]);

    if(precision != "fp64" && precision != "fp32" && precision != "fp16" && precision != "bf16" &&
       precision != "int8" && precision != "int32" && precision != "int16" &&
       precision != "uint32" && precision != "uint16" && precision != "uint8")
        throw std::runtime_error("Invalid precison of input data specified!");

    int numElements = atoi(argv[4]);

    assert(numElements > 0);

    if(precision == "fp64")
    {
        show_floating_point_data_pair_from_file<double>(numElements, argv[2], argv[3]);
    }
    else if(precision == "fp32")
    {
        show_floating_point_data_pair_from_file<float>(numElements, argv[2], argv[3]);
    }
    else if(precision == "fp16")
    {
        show_floating_point_data_pair_from_file<half_t>(numElements, argv[2], argv[3]);
    }
    else if(precision == "bf16")
    {
        show_floating_point_data_pair_from_file<bhalf_t>(numElements, argv[2], argv[3]);
    }
    else if(precision == "int32")
    {
        show_integer_data_pair_from_file<int32_t>(numElements, argv[2], argv[3]);
    }
    else if(precision == "uint32")
    {
        show_integer_data_pair_from_file<uint32_t>(numElements, argv[2], argv[3]);
    }
    else if(precision == "int16")
    {
        show_integer_data_pair_from_file<int16_t>(numElements, argv[2], argv[3]);
    }
    else if(precision == "uint16")
    {
        show_integer_data_pair_from_file<uint16_t>(numElements, argv[2], argv[3]);
    }
    else if(precision == "int8")
    {
        show_integer_data_pair_from_file<int8_t>(numElements, argv[2], argv[3]);
    }
    else if(precision == "uint8")
    {
        show_integer_data_pair_from_file<uint8_t>(numElements, argv[2], argv[3]);
    }
};
