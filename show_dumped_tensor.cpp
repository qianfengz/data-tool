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
void readBufferFromFile(T* data, size_t dataNumItems, const std::string& fileName)
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
}

static std::vector<int> get_integers_from_string(std::string lengthsStr)
{
    std::vector<int> lengths;
    std::size_t pos = 0;
    std::size_t new_pos;

    new_pos = lengthsStr.find(',', pos);
    while(new_pos != std::string::npos)
    {
        std::string sliceStr = lengthsStr.substr(pos, new_pos - pos);

        int len = std::stoi(sliceStr);

        lengths.push_back(len);

        pos     = new_pos + 1;
        new_pos = lengthsStr.find(',', pos);
    };

    std::string sliceStr = lengthsStr.substr(pos);

    if(!sliceStr.empty())
    {
        int len = std::stoi(sliceStr);

        lengths.push_back(len);
    };

    return (lengths);
};

template <typename T>
void show_integer_tensor_from_file(
    int numElements, const char* filename, int num_batch, int num_head, int seqlen1, int seqlen2)
{
    T* dataBuffer = new T[numElements];

    readBufferFromFile(dataBuffer, numElements, filename);

    if(num_head > 0)
    {
        int batch_stride = num_head * seqlen1 * seqlen2;
        int head_stride  = seqlen1 * seqlen2;
        for(int i_batch = 0; i_batch < num_batch; i_batch++)
        {
            for(int i_head = 0; i_head < num_head; i_head++)
            {
                std::cout << "batch: " << i_batch << " head: " << i_head << std::endl;
                T* pData = &dataBuffer[i_batch * batch_stride + i_head * head_stride];

                for(int i_seq1 = 0; i_seq1 < seqlen1; i_seq1++)
                {
                    for(int i_seq2 = 0; i_seq2 < seqlen2; i_seq2++)
                    {
                        if constexpr(sizeof(T) >= 2)
                            std::cout << pData[i_seq1 * seqlen2 + i_seq2] << " ";
                        else
                            std::cout << static_cast<int>(pData[i_seq1 * seqlen2 + i_seq2]) << " ";
                    };

                    std::cout << std::endl;
                };

                std::cout << std::endl;
            };
        };
    }
    else
    {
        int batch_stride = seqlen1 * seqlen2;
        for(int i_batch = 0; i_batch < num_batch; i_batch++)
        {
            std::cout << "batch: " << i_batch << std::endl;
            T* pData = &dataBuffer[i_batch * batch_stride];

            for(int i_seq1 = 0; i_seq1 < seqlen1; i_seq1++)
            {
                for(int i_seq2 = 0; i_seq2 < seqlen2; i_seq2++)
                {
                    if constexpr(sizeof(T) >= 2)
                        std::cout << pData[i_seq1 * seqlen2 + i_seq2] << " ";
                    else
                        std::cout << static_cast<int>(pData[i_seq1 * seqlen2 + i_seq2]) << " ";
                };

                std::cout << std::endl;
            };

            std::cout << std::endl;
        };
    };
};

template <typename T>
void show_floating_point_tensor_from_file(
    int numElements, const char* filename, int num_batch, int num_head, int seqlen1, int seqlen2)
{
    T* dataBuffer = new T[numElements];

    readBufferFromFile(dataBuffer, numElements, filename);

    if(num_head > 0)
    {
        int batch_stride = num_head * seqlen1 * seqlen2;
        int head_stride  = seqlen1 * seqlen2;
        for(int i_batch = 0; i_batch < num_batch; i_batch++)
        {
            for(int i_head = 0; i_head < num_head; i_head++)
            {
                std::cout << "batch: " << i_batch << " head: " << i_head << std::endl;
                T* pData = &dataBuffer[i_batch * batch_stride + i_head * head_stride];

                for(int i_seq1 = 0; i_seq1 < seqlen1; i_seq1++)
                {
                    std::cout << std::fixed;

                    for(int i_seq2 = 0; i_seq2 < seqlen2; i_seq2++)
                    {
                        if constexpr(sizeof(T) >= 2)
                            std::cout << pData[i_seq1 * seqlen2 + i_seq2] << " ";
                        else
                            std::cout << static_cast<int>(pData[i_seq1 * seqlen2 + i_seq2]) << " ";
                    };

                    std::cout << std::endl;
                };

                std::cout << std::endl;
            };
        };
    }
    else
    {
        int batch_stride = seqlen1 * seqlen2;
        for(int i_batch = 0; i_batch < num_batch; i_batch++)
        {
            std::cout << "batch: " << i_batch << std::endl;
            T* pData = &dataBuffer[i_batch * batch_stride];

            for(int i_seq1 = 0; i_seq1 < seqlen1; i_seq1++)
            {
                std::cout << std::fixed;

                for(int i_seq2 = 0; i_seq2 < seqlen2; i_seq2++)
                {
                    if constexpr(sizeof(T) >= 2)
                        std::cout << pData[i_seq1 * seqlen2 + i_seq2] << " ";
                    else
                        std::cout << static_cast<int>(pData[i_seq1 * seqlen2 + i_seq2]) << " ";
                };

                std::cout << std::endl;
            };

            std::cout << std::endl;
        };
    };
};

int main(int argc, char* argv[])
{
    if(argc != 5)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <fp64/fp32/fp16/bf16/int32/int16/int8/uint32/uint16/uint8> <file> <number "
                     "of elements> <tensor shape>"
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

    std::vector<int> dim_lengths = get_integers_from_string(argv[4]);

    if(dim_lengths.size() != 3 && dim_lengths.size() != 4)
        throw std::runtime_error("Only tensors of 3 or 4 dimensions supported!");

    int prod_lengths = 1;
    for(auto len : dim_lengths)
        prod_lengths *= len;

    if(numElements != prod_lengths)
        throw std::runtime_error("The number of elments does not match the tensor shape!");

    if(dim_lengths.size() == 4)
    {
        if(precision == "fp64")
        {
            show_floating_point_tensor_from_file<double>(numElements,
                                                         argv[2],
                                                         dim_lengths[0],
                                                         dim_lengths[1],
                                                         dim_lengths[2],
                                                         dim_lengths[3]);
        }
        else if(precision == "fp32")
        {
            show_floating_point_tensor_from_file<float>(numElements,
                                                        argv[2],
                                                        dim_lengths[0],
                                                        dim_lengths[1],
                                                        dim_lengths[2],
                                                        dim_lengths[3]);
        }
        else if(precision == "fp16")
        {
            show_floating_point_tensor_from_file<half_t>(numElements,
                                                         argv[2],
                                                         dim_lengths[0],
                                                         dim_lengths[1],
                                                         dim_lengths[2],
                                                         dim_lengths[3]);
        }
        else if(precision == "bf16")
        {
            show_floating_point_tensor_from_file<bhalf_t>(numElements,
                                                          argv[2],
                                                          dim_lengths[0],
                                                          dim_lengths[1],
                                                          dim_lengths[2],
                                                          dim_lengths[3]);
        }
        else if(precision == "int32")
        {
            show_integer_tensor_from_file<int32_t>(numElements,
                                                   argv[2],
                                                   dim_lengths[0],
                                                   dim_lengths[1],
                                                   dim_lengths[2],
                                                   dim_lengths[3]);
        }
        else if(precision == "uint32")
        {
            show_integer_tensor_from_file<uint32_t>(numElements,
                                                    argv[2],
                                                    dim_lengths[0],
                                                    dim_lengths[1],
                                                    dim_lengths[2],
                                                    dim_lengths[3]);
        }
        else if(precision == "int16")
        {
            show_integer_tensor_from_file<int16_t>(numElements,
                                                   argv[2],
                                                   dim_lengths[0],
                                                   dim_lengths[1],
                                                   dim_lengths[2],
                                                   dim_lengths[3]);
        }
        else if(precision == "uint16")
        {
            show_integer_tensor_from_file<uint16_t>(numElements,
                                                    argv[2],
                                                    dim_lengths[0],
                                                    dim_lengths[1],
                                                    dim_lengths[2],
                                                    dim_lengths[3]);
        }
        else if(precision == "int8")
        {
            show_integer_tensor_from_file<int8_t>(numElements,
                                                  argv[2],
                                                  dim_lengths[0],
                                                  dim_lengths[1],
                                                  dim_lengths[2],
                                                  dim_lengths[3]);
        }
        else if(precision == "uint8")
        {
            show_integer_tensor_from_file<uint8_t>(numElements,
                                                   argv[2],
                                                   dim_lengths[0],
                                                   dim_lengths[1],
                                                   dim_lengths[2],
                                                   dim_lengths[3]);
        }
    }
    else
    {
        if(precision == "fp64")
        {
            show_floating_point_tensor_from_file<double>(
                numElements, argv[2], dim_lengths[0], 0, dim_lengths[1], dim_lengths[2]);
        }
        else if(precision == "fp32")
        {
            show_floating_point_tensor_from_file<double>(
                numElements, argv[2], dim_lengths[0], 0, dim_lengths[1], dim_lengths[2]);
        }
        else if(precision == "fp16")
        {
            show_floating_point_tensor_from_file<half_t>(
                numElements, argv[2], dim_lengths[0], 0, dim_lengths[1], dim_lengths[2]);
        }
        else if(precision == "bf16")
        {
            show_floating_point_tensor_from_file<bhalf_t>(
                numElements, argv[2], dim_lengths[0], 0, dim_lengths[1], dim_lengths[2]);
        }
        else if(precision == "int32")
        {
            show_integer_tensor_from_file<int32_t>(
                numElements, argv[2], dim_lengths[0], 0, dim_lengths[1], dim_lengths[2]);
        }
        else if(precision == "uint32")
        {
            show_integer_tensor_from_file<uint32_t>(
                numElements, argv[2], dim_lengths[0], 0, dim_lengths[1], dim_lengths[2]);
        }
        else if(precision == "int16")
        {
            show_integer_tensor_from_file<int16_t>(
                numElements, argv[2], dim_lengths[0], 0, dim_lengths[1], dim_lengths[2]);
        }
        else if(precision == "uint16")
        {
            show_integer_tensor_from_file<uint16_t>(
                numElements, argv[2], dim_lengths[0], 0, dim_lengths[1], dim_lengths[2]);
        }
        else if(precision == "int8")
        {
            show_integer_tensor_from_file<int8_t>(
                numElements, argv[2], dim_lengths[0], 0, dim_lengths[1], dim_lengths[2]);
        }
        else if(precision == "uint8")
        {
            show_integer_tensor_from_file<uint8_t>(
                numElements, argv[2], dim_lengths[0], 0, dim_lengths[1], dim_lengths[2]);
        }
    }
};
