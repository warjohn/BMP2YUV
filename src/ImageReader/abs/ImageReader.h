#ifndef IMAGE_READER
#define IMAGE_READER

#include <string>
#include <vector>
#include <stdint.h>
#include <fstream>

class ImageReader {
    /*Абстрактный класс для чтения изображений разных форматов
    Есть возможность в дальнейшем добавлять другие форматы изображений .png, .jpg и другие ...
    */
public:
    ImageReader(const std::string& inputBMP, int& width, int& height, std::vector<uint8_t>& pixelData)
        : inputBMP(inputBMP), width(width), height(height), pixelData(pixelData) {};
    virtual ~ImageReader() = default;

    virtual void read() = 0;

protected:
    std::ifstream file;
    const std::string& inputBMP; // Путь к файлу изображения
    int& width; // Ширина изображения
    int& height; // Высота изображения
    std::vector<uint8_t>& pixelData; // Данные пикселей
};

#endif // IMAGE_READER