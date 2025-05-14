#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        std::cout << "Usage: " << argv[0] << " <filename1> <filename2> <number of data>"
                  << std::endl;
        throw std::runtime_error("Invalid argument!");
    }

    std::ifstream f1(argv[1]);
    std::ifstream f2(argv[2]);

    int num_elements = atoi(argv[3]);

    for(int i = 0; i < num_elements; i++)
    {
        float data1, data2;

        f1 >> data1;
        f2 >> data2;

        std::cout << i << ": " << std::fixed << data1 << "  " << data2 << std::endl;
    }
};
