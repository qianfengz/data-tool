#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <cassert> 
#include <half/half.hpp>

using float16 = half_float::half; 

template <typename T>
void dumpBufferToFile(const char* fileName, T* data, size_t dataNumItems)
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

template <typename T>
void simpleBufferInit(T* data, size_t dataNumItems)
{
    const int dataOffset = 1; 

    for (int i=0; i < dataNumItems; i++) 
	 data[i] = static_cast<T>(i+dataOffset); 
}; 

int main(int argc, char *argv[])
{
     if ( argc != 4 ) {
          std::cerr << "Usage: " << argv[0] << " <fp64/fp32/fp16/int> <file> <number of elements> " << std::endl; 

	  throw std::runtime_error("Invalid command argument!"); 
     }; 

     std::string precision(argv[1]); 

     if (precision != "fp64" && precision != "fp32" && precision != "fp16" && precision != "int" ) 
	  throw std::runtime_error("Invalid precison of input data specified!"); 

     int numElements = atoi(argv[3]); 

     assert( numElements > 0 ); 

     if ( precision == "fp64" ) {
         double *dataBuffer = new double[numElements]; 

         simpleBufferInit(dataBuffer, numElements); 
	 dumpBufferToFile(argv[2], dataBuffer, numElements); 
     }
     else
     if ( precision == "fp32" ) {
         float *dataBuffer = new float[numElements]; 

         simpleBufferInit(dataBuffer, numElements); 
	 dumpBufferToFile(argv[2], dataBuffer, numElements); 
     }
     else 
     if ( precision == "fp16" ) {
         float16 *dataBuffer = new float16[numElements];


         simpleBufferInit(dataBuffer, numElements); 
	 dumpBufferToFile(argv[2], dataBuffer, numElements); 
         delete [] dataBuffer; 
     }
     else { 
         int *dataBuffer = new int[numElements];


         simpleBufferInit(dataBuffer, numElements); 
	 dumpBufferToFile(argv[2], dataBuffer, numElements); 
         delete [] dataBuffer;
     }; 
}; 


