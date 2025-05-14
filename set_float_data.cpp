#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <cassert>

template <typename T>
static void dumpBufferToFile(const char* fileName, T* data, size_t dataNumItems)
{
    std::ofstream outFile(fileName, std::ios::binary);
    if(outFile)
    {
        outFile.write(reinterpret_cast<char*>(data), dataNumItems * sizeof(T));
        outFile.close();
        printf("Wrote output to file %s\n", fileName);
    }
    else
    {
        printf("Could not open file %s for writing\n", fileName);
    }
}

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <file> <number of elements> <value>" << std::endl;

        throw std::runtime_error("Invalid command argument!");
    };

    int numElements = atoi(argv[2]);
    float value     = static_cast<float>(atof(argv[3]));

    assert(numElements > 0);

    float* dataBuffer = new float[numElements];

    for(int i = 0; i < numElements; i++)
        dataBuffer[i] = value;

    dumpBufferToFile(argv[1], dataBuffer, numElements);
};
