#ifndef CONVERTOR
#define CONVERTOR

#include <vector>
#include <stdint.h>
#include <thread>

class ConvertAnyFormat2YUV {
    /*
    На данный момент реализована конвертация из BGR -> YUV 
    В дальнейшем возможность добавить RGB -> YUV, BGRA -> YUV
    */
public: 

    ConvertAnyFormat2YUV() : numThreads(std::max<std::size_t>(std::thread::hardware_concurrency(), 1)){}; 

    virtual ~ConvertAnyFormat2YUV() = default;

protected:


    size_t numThreads; // number of threads 
    std::vector<std::thread> threads;

    // BGR -> YUV
    virtual void ConvertBGRToYUV420P(const std::vector<uint8_t>& bgrData, int width, int height,
        std::vector<uint8_t>& yBuffer, std::vector<uint8_t>& uBuffer, std::vector<uint8_t>& vBuffer) = 0;
    virtual void BgrToY(const uint8_t* bgr, size_t bgrStride, size_t startRow, size_t endRow, size_t width,
        uint8_t* y, size_t yStride) = 0;
    virtual void BgrToUV(const uint8_t* bgr, size_t bgrStride, size_t startRow, size_t endRow, size_t width, size_t height,
        uint8_t* u, size_t uStride, uint8_t* v, size_t vStride) = 0;
    virtual void BgrToYuv420pParallel(const uint8_t* bgr, size_t bgrStride, size_t width, size_t height,
        uint8_t* y, size_t yStride, uint8_t* u, size_t uStride, uint8_t* v, size_t vStride) = 0;

           
    // RGB -> YUV
        // ... Coming soon

    // RGBA -> YUV
        // ... Coming soon



};
#endif //CONVERTOR
