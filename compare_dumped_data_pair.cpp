#include <cassert>
#include <cstdlib>
#include <fstream>
#include <half/half.hpp>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "type_convert.hpp"
#include "check_err.hpp"

template <typename T>
void readDataToBufferFromFile(T* data, size_t dataNumItems, const std::string& fileName)
{
    std::ifstream infile(fileName, std::ios::binary);
    if(infile)
    {
        try
        {
            infile.read(reinterpret_cast<char*>(data), dataNumItems * sizeof(T));
            infile.close();
        }
        catch(const std::runtime_error& e)
        {
            throw e;
        };
    }
    else
    {
        throw std::runtime_error("could not open the file for reading");
    }
}

// threshold for different dtypes
template <typename DataType>
auto get_threshold()
{
    double rtol = 1e-3;
    double atol = 1e-3;

    return std::make_pair(rtol, atol);
}

template <>
auto get_threshold<bhalf_t>()
{
    double rtol = 2e-3;
    double atol = 2e-3;

    return std::make_pair(rtol, atol);
}

template <typename T>
void compare_floating_point_data_pair_from_file(
    int numElements, const char* filename1, const char* filename2, double atol, double rtol)
{
    T* dataBuffer1 = new T[numElements];
    T* dataBuffer2 = new T[numElements];

    readDataToBufferFromFile(dataBuffer1, numElements, filename1);
    readDataToBufferFromFile(dataBuffer2, numElements, filename2);

    auto res = data_tool::check_err(
        dataBuffer1, dataBuffer2, numElements, "Incorrect results!", atol, rtol);

    if(res)
        std::cout << "Failed: the elements from the two files are not completely consistent!"
                  << std::endl;
    else
        std::cout << "Passed: the elements from the two files are completely consistent!"
                  << std::endl;

    delete[] dataBuffer1;
    delete[] dataBuffer2;
};

int main(int argc, char* argv[])
{
    if(argc < 5)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <fp32/fp16/bf16> "
                     "<file1> <file2> <number of elements> [atol] [rtol]"
                  << std::endl;

        throw std::runtime_error("Invalid command argument!");
    };

    std::string precision(argv[1]);

    if(precision != "fp32" && precision != "fp16" && precision != "bf16")
        throw std::runtime_error("Invalid precison of input data specified!");

    int numElements = atoi(argv[4]);

    assert(numElements > 0);

    if(precision == "fp32")
    {
        auto [atol, rtol] = get_threshold<float>();
        if(argc >= 6)
            atol = atof(argv[5]);
        if(argc >= 7)
            rtol = atof(argv[6]);

        compare_floating_point_data_pair_from_file<float>(
            numElements, argv[2], argv[3], atol, rtol);
    }
    else if(precision == "fp16")
    {
        auto [atol, rtol] = get_threshold<half_t>();
        if(argc >= 6)
            atol = atof(argv[5]);
        if(argc >= 7)
            rtol = atof(argv[6]);

        compare_floating_point_data_pair_from_file<half_t>(
            numElements, argv[2], argv[3], atol, rtol);
    }
    else if(precision == "bf16")
    {
        auto [atol, rtol] = get_threshold<bhalf_t>();
        if(argc >= 6)
            atol = atof(argv[5]);
        if(argc >= 7)
            rtol = atof(argv[6]);

        compare_floating_point_data_pair_from_file<bhalf_t>(
            numElements, argv[2], argv[3], atol, rtol);
    }
};
