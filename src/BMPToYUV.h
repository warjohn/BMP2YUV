#ifndef BMP_TO_YUV_H
#define BMP_TO_YUV_H

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t bfType;      // Тип файла (0x4D42)
    uint32_t bfSize;      // Размер файла
    uint16_t bfReserved1; // Зарезервировано
    uint16_t bfReserved2; // Зарезервировано
    uint32_t bfOffBits;   // Смещение от начала файла до данных изображения
};

struct DIBHeader {
    uint32_t biSize;             // Размер DIB заголовка
    int32_t biWidth;             // Ширина изображения
    int32_t biHeight;            // Высота изображения
    uint16_t biPlanes;           // Число цветовых плоскостей
    uint16_t biBitCount;         // Число бит на пиксель
    uint32_t biCompression;      // Тип сжатия
    uint32_t biSizeImage;        // Размер изображения
    int32_t biXPelsPerMeter;     // Разрешение по оси X
    int32_t biYPelsPerMeter;     // Разрешение по оси Y
    uint32_t biClrUsed;          // Число используемых цветов
    uint32_t biClrImportant;     // Число важных цветов
};
#pragma pack(pop)

class BMPToYUVConverter {
public:
    BMPToYUVConverter() = default;
    ~BMPToYUVConverter() = default;

    bool ReadBMP(const std::string& filename, std::vector<uint8_t>& pixelData, int& width, int& height);
    void ConvertBGRToYUV420P(const std::vector<uint8_t>& bgrData, int width, int height,
                             std::vector<uint8_t>& yBuffer, std::vector<uint8_t>& uBuffer, std::vector<uint8_t>& vBuffer);
    void InvertYUVVertically(std::vector<uint8_t>& yBuffer, std::vector<uint8_t>& uBuffer, std::vector<uint8_t>& vBuffer,
                             int width, int height);
    void SaveYUV(const std::string& filename, const std::vector<uint8_t>& yBuffer, const std::vector<uint8_t>& uBuffer,
                 const std::vector<uint8_t>& vBuffer, int width, int height);

private:
    void BgrToY(const uint8_t* bgr, size_t bgrStride, size_t startRow, size_t endRow, size_t width,
        uint8_t* y, size_t yStride);
    void BgrToUV(const uint8_t* bgr, size_t bgrStride, size_t startRow, size_t endRow, size_t width, size_t height,
        uint8_t* u, size_t uStride, uint8_t* v, size_t vStride);
    void BgrToYuv420pParallel(const uint8_t* bgr, size_t bgrStride, size_t width, size_t height,
                              uint8_t* y, size_t yStride, uint8_t* u, size_t uStride, uint8_t* v, size_t vStride);
};

class VideoProcessor {
public:
    VideoProcessor() = default;
    ~VideoProcessor() = default;

    bool InsertOverlay(std::vector<uint8_t>& frameData, const std::vector<uint8_t>& yBuffer,
                       const std::vector<uint8_t>& uBuffer, const std::vector<uint8_t>& vBuffer,
                       int frameWidth, int frameHeight, int overlayWidth, int overlayHeight);
    bool ProcessVideo(const std::string& inputYUV, const std::string& outputYUV, int videoWidth, int videoHeight,
                      const std::vector<uint8_t>& yBuffer, const std::vector<uint8_t>& uBuffer,
                      const std::vector<uint8_t>& vBuffer, int overlayWidth, int overlayHeight);
};

#endif // BMP_TO_YUV_H