#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <cassert> 
#include <type_traits>
#include <half/half.hpp>

using half_t = half_float::half; 
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

int main(int argc, char *argv[])
{
     if ( argc != 5 ) {
          std::cerr << "Usage: " << argv[0] << " <fp64/fp32/fp16/bf16/int32/int16/int8/uint32/uint16/uint8> <file1> <file2> <number of elements> " << std::endl; 

	  throw std::runtime_error("Invalid command argument!"); 
     }; 

     std::string precision(argv[1]); 

     if (precision != "fp64" && precision != "fp32" && precision != "fp16" && precision != "bf16" && precision != "int8" && precision != "int32" 
		     && precision != "int16" && precision != "uint32" && precision != "uint16" && precision != "uint8") 
	  throw std::runtime_error("Invalid precison of input data specified!"); 

     int numElements = atoi(argv[4]); 

     assert( numElements > 0 ); 

     if ( precision == "fp64" ) {
         double *dataBuffer1 = new double[numElements]; 
         double *dataBuffer2 = new double[numElements]; 

	 if ( readBufferFromFile(dataBuffer1, numElements, argv[2]) && readBufferFromFile(dataBuffer2, numElements, argv[3]) ) {
              for (int i=0; i < numElements; i++) {
                   std::cout << i << " : " << std::fixed << dataBuffer1[i] << " " << dataBuffer2[i] << std::endl;
              };
         };  

         delete [] dataBuffer1; 
         delete [] dataBuffer2; 
     }
     else
     if ( precision == "fp32" ) {
         float *dataBuffer1 = new float[numElements]; 
         float *dataBuffer2 = new float[numElements]; 

	 if ( readBufferFromFile(dataBuffer1, numElements, argv[2]) && readBufferFromFile(dataBuffer2, numElements, argv[3]) ) {
              for (int i=0; i < numElements; i++) {
                   std::cout << i << " : " << std::fixed << dataBuffer1[i] << " " << dataBuffer2[i] << std::endl;
              };
         };  

         delete [] dataBuffer1; 
         delete [] dataBuffer2; 
     }
     else 
     if ( precision == "fp16" ) {
         half_t *dataBuffer1 = new half_t[numElements];
         half_t *dataBuffer2 = new half_t[numElements];

	 if ( readBufferFromFile(dataBuffer1, numElements, argv[2]) && readBufferFromFile(dataBuffer2, numElements, argv[3]) ) {
              for (int i=0; i < numElements; i++) {
                   std::cout << i << " : " << std::fixed << dataBuffer1[i] << " " << dataBuffer2[i] << std::endl;
              };
         };

         delete [] dataBuffer1;
         delete [] dataBuffer2;
     }
     else 
     if ( precision == "bf16") {
         bhalf_t *dataBuffer1 = new bhalf_t[numElements];
         bhalf_t *dataBuffer2 = new bhalf_t[numElements];

	 if ( readBufferFromFile(dataBuffer1, numElements, argv[2]) && readBufferFromFile(dataBuffer2, numElements, argv[3]) ) {
              for (int i=0; i < numElements; i++) {
                   float value1 = type_convert<float>(dataBuffer1[i]); 
                   float value2 = type_convert<float>(dataBuffer2[i]); 

                   std::cout << i << " : " << std::fixed << value1 << " " << value2 << std::endl;
              };
         };

         delete [] dataBuffer1;
         delete [] dataBuffer2;
     }
     else
     if ( precision == "int32" ) {
         int32_t *dataBuffer1 = new int32_t[numElements];
         int32_t *dataBuffer2 = new int32_t[numElements];

	 if ( readBufferFromFile(dataBuffer1, numElements, argv[2]) && readBufferFromFile(dataBuffer2, numElements, argv[3]) ) {
              for (int i=0; i < numElements; i++)
                   std::cout << i << " : " << dataBuffer1[i] << " " << dataBuffer2[i] << std::endl;
         };

         delete [] dataBuffer1;
         delete [] dataBuffer2;
     }
     else
     if ( precision == "uint32" ) {
         uint32_t *dataBuffer1 = new uint32_t[numElements];
         uint32_t *dataBuffer2 = new uint32_t[numElements];

	 if ( readBufferFromFile(dataBuffer1, numElements, argv[2]) && readBufferFromFile(dataBuffer2, numElements, argv[3]) ) {
              for (int i=0; i < numElements; i++)
                   std::cout << i << " : " << dataBuffer1[i] << " " << dataBuffer2[i] << std::endl;
         };

         delete [] dataBuffer1;
         delete [] dataBuffer2;
     }
     else
     if ( precision == "int16" ) {
         int16_t *dataBuffer1 = new int16_t[numElements];
         int16_t *dataBuffer2 = new int16_t[numElements];

	 if ( readBufferFromFile(dataBuffer1, numElements, argv[2]) && readBufferFromFile(dataBuffer2, numElements, argv[3]) ) {
              for (int i=0; i < numElements; i++)
                   std::cout << i << " : " << dataBuffer1[i] << " " << dataBuffer2[i] << std::endl;
         };

         delete [] dataBuffer1;
         delete [] dataBuffer2;
     }
     else
     if ( precision == "uint16" ) {
         uint16_t *dataBuffer1 = new uint16_t[numElements];
         uint16_t *dataBuffer2 = new uint16_t[numElements];

	 if ( readBufferFromFile(dataBuffer1, numElements, argv[2]) && readBufferFromFile(dataBuffer2, numElements, argv[3]) ) {
              for (int i=0; i < numElements; i++)
                   std::cout << i << " : " << dataBuffer1[i] << " " << dataBuffer2[i] << std::endl;
         };

         delete [] dataBuffer1;
         delete [] dataBuffer2;
     }
     else
     if ( precision == "int8" ) {
         int8_t *dataBuffer1 = new int8_t[numElements];
         int8_t *dataBuffer2 = new int8_t[numElements];

	 if ( readBufferFromFile(dataBuffer1, numElements, argv[2]) && readBufferFromFile(dataBuffer2, numElements, argv[3]) ) {
              for (int i=0; i < numElements; i++)
                   std::cout << i << " : " << static_cast<int>(dataBuffer1[i]) << " " << static_cast<int>(dataBuffer2[i]) << std::endl;
         };

         delete [] dataBuffer1;
         delete [] dataBuffer2;
     }
     else
     if ( precision == "uint8" ) {
         uint8_t *dataBuffer1 = new uint8_t[numElements];
         uint8_t *dataBuffer2 = new uint8_t[numElements];

	 if ( readBufferFromFile(dataBuffer1, numElements, argv[2]) && readBufferFromFile(dataBuffer2, numElements, argv[3]) ) {
              for (int i=0; i < numElements; i++)
                   std::cout << i << " : " << static_cast<int>(dataBuffer1[i]) << " " << static_cast<int>(dataBuffer2[i]) << std::endl;
         };

         delete [] dataBuffer1;
         delete [] dataBuffer2;
     }
}; 


