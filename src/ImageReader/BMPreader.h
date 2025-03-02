#ifndef BMP_READER
#define BMP_READER

#include "abs/ImageReader.h"
#include "../cstmIncludes.h"



#pragma pack(push, 1)
struct BMPHeader_1 {
    uint16_t bfType;      // Тип файла (0x4D42)
    uint32_t bfSize;      // Размер файла
    uint16_t bfReserved1; // Зарезервировано
    uint16_t bfReserved2; // Зарезервировано
    uint32_t bfOffBits;   // Смещение от начала файла до данных изображения
};

// Структура DIBHeader
struct DIBHeader_1 {
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

class BMPreader : public ImageReader {
public:
    BMPreader(const std::string& inputBMP, int& width, int& height, std::vector<uint8_t>& pixelData);
    ~BMPreader();

    void read() override;
};

#endif // BMP_READER