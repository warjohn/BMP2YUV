#ifndef BMP_TO_YUV_H
#define BMP_TO_YUV_H

#include "abs/Convertor.h"
#include "../cstmIncludes.h"



class BMPToYUVConverter : public ConvertAnyFormat2YUV {
public:

    ~BMPToYUVConverter();

    void ConvertBGRToYUV420P(const std::vector<uint8_t>& bgrData, int width, int height,
                             std::vector<uint8_t>& yBuffer, std::vector<uint8_t>& uBuffer, std::vector<uint8_t>& vBuffer) override;


private:

    void BgrToY(const uint8_t* bgr, size_t bgrStride, size_t startRow, size_t endRow, size_t width,
        uint8_t* y, size_t yStride) override;
    void BgrToUV(const uint8_t* bgr, size_t bgrStride, size_t startRow, size_t endRow, size_t width, size_t height,
        uint8_t* u, size_t uStride, uint8_t* v, size_t vStride) override;
    void BgrToYuv420pParallel(const uint8_t* bgr, size_t bgrStride, size_t width, size_t height,
                              uint8_t* y, size_t yStride, uint8_t* u, size_t uStride, uint8_t* v, size_t vStride) override;
};



#endif // BMP_TO_YUV_H