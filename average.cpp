#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    std::ifstream dataFile(argv[1], std::ifstream::in);

    float total = 0.0;
    float value;
    int count = 0;

    while(!dataFile.eof())
    {
        dataFile >> value;

        if(!dataFile.eof())
        {
            total += value;
            count++;
        };
    };

    std::cout << "Average value is " << total / (float)count << std::endl;
};
