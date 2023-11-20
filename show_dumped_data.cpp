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

int main(int argc, char *argv[])
{
     if ( argc != 4 ) {
          std::cerr << "Usage: " << argv[0] << " <fp64/fp32/fp16/bp16/int32/int16/int8/uint32/uint16/uint8> <file> <number of elements> " << std::endl; 

	  throw std::runtime_error("Invalid command argument!"); 
     }; 

     std::string precision(argv[1]); 

     if (precision != "fp64" && precision != "fp32" && precision != "fp16" && precision != "bp16" && precision != "int8" && precision != "int32" 
		     && precision != "int16" && precision != "uint32" && precision != "uint16" && precision != "uint8") 
	  throw std::runtime_error("Invalid precison of input data specified!"); 

     int numElements = atoi(argv[3]); 

     assert( numElements > 0 ); 

     if ( precision == "fp64" ) {
         double *dataBuffer = new double[numElements]; 

	 if ( readBufferFromFile(dataBuffer, numElements, argv[2]) ) {
              for (int i=0; i < numElements; i++) {
                   if ( std::isinf(dataBuffer[i]) || std::isnan(dataBuffer[i]) ) 
                        std::cout << i << " : " << "invalid value" << std::endl;
                   else 
                        std::cout << i << " : " << std::fixed << dataBuffer[i] << std::endl;
              };
         };  

         delete [] dataBuffer;
     }
     else
     if ( precision == "fp32" ) {
         float *dataBuffer = new float[numElements]; 

	 if ( readBufferFromFile(dataBuffer, numElements, argv[2]) ) {
              for (int i=0; i < numElements; i++) {
                   if ( std::isinf(dataBuffer[i]) || std::isnan(dataBuffer[i]) ) 
                        std::cout << i << " : " << "invalid value" << std::endl;
                   else 
                        std::cout << i << " : " << std::fixed << dataBuffer[i] << std::endl;
              };
         };  

         delete [] dataBuffer;
     }
     else 
     if ( precision == "fp16" ) {
         half_t *dataBuffer = new half_t[numElements];

         if ( readBufferFromFile(dataBuffer, numElements, argv[2]) ) {
              for (int i=0; i < numElements; i++) {
                   if ( std::isinf(dataBuffer[i]) || std::isnan(dataBuffer[i]) ) 
                        std::cout << i << " : " << "invalid value" << std::endl;
                   else 
                        std::cout << i << " : " << std::fixed << dataBuffer[i] << std::endl;
              };
         };

         delete [] dataBuffer; 
     }
     else 
     if ( precision == "bp16") {
         bhalf_t *dataBuffer = new bhalf_t[numElements];

         if ( readBufferFromFile(dataBuffer, numElements, argv[2]) ) {
              for (int i=0; i < numElements; i++) {
                   float value = type_convert<float>(dataBuffer[i]); 

                   if ( std::isinf(value) || std::isnan(value) )
                        std::cout << i << " : " << "invalid value" << std::endl;
                   else
                        std::cout << i << " : " << std::fixed << value << std::endl;
              };
         };

         delete [] dataBuffer;
     }
     else
     if ( precision == "int32" ) {
         int32_t *dataBuffer = new int32_t[numElements];

         if ( readBufferFromFile(dataBuffer, numElements, argv[2]) ) {
              for (int i=0; i < numElements; i++)
                   std::cout << i << " : " << dataBuffer[i] << std::endl;
         };

         delete [] dataBuffer;
     }
     else
     if ( precision == "uint32" ) {
         uint32_t *dataBuffer = new uint32_t[numElements];

         if ( readBufferFromFile(dataBuffer, numElements, argv[2]) ) {
              for (int i=0; i < numElements; i++)
                   std::cout << i << " : " << dataBuffer[i] << std::endl;
         };

         delete [] dataBuffer;
     }
     else
     if ( precision == "int16" ) {
         int16_t *dataBuffer = new int16_t[numElements];

         if ( readBufferFromFile(dataBuffer, numElements, argv[2]) ) {
              for (int i=0; i < numElements; i++)
                   std::cout << i << " : " << dataBuffer[i] << std::endl;
         };

         delete [] dataBuffer;
     }
     else
     if ( precision == "uint16" ) {
         uint16_t *dataBuffer = new uint16_t[numElements];

         if ( readBufferFromFile(dataBuffer, numElements, argv[2]) ) {
              for (int i=0; i < numElements; i++)
                   std::cout << i << " : " << dataBuffer[i] << std::endl;
         };

         delete [] dataBuffer;
     }
     else
     if ( precision == "int8" ) {
         int8_t *dataBuffer = new int8_t[numElements];

         if ( readBufferFromFile(dataBuffer, numElements, argv[2]) ) {
              for (int i=0; i < numElements; i++)
                   std::cout << i << " : " << static_cast<int>(dataBuffer[i]) << std::endl;
         };

         delete [] dataBuffer;
     }
     else
     if ( precision == "uint8" ) {
         uint8_t *dataBuffer = new uint8_t[numElements];

         if ( readBufferFromFile(dataBuffer, numElements, argv[2]) ) {
              for (int i=0; i < numElements; i++)
                   std::cout << i << " : " << static_cast<int>(dataBuffer[i]) << std::endl;
         };

         delete [] dataBuffer;
     }
}; 


