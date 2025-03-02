#include "BMPreader.h"


BMPreader::BMPreader(const std::string& inputBMP, int& width, int& height, std::vector<uint8_t>& pixelData) :  ImageReader(inputBMP, width, height, pixelData) {
    file.open(inputBMP, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("failed with open file");
    }
}
     
BMPreader::~BMPreader(){
    if (file.is_open()) {
        file.close();
    }
}

void BMPreader::read() {
    BMPHeader_1 bmpHeader;
    file.read(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));
    if (bmpHeader.bfType != 0x4D42) {
        throw std::invalid_argument("Error with file format");
    }

    DIBHeader_1 dibHeader;
    file.read(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader));
    if (dibHeader.biBitCount != 24) {
        throw std::invalid_argument("The image can contains only 24 bit per pixel");
    }

    width = dibHeader.biWidth;
    height = dibHeader.biHeight;

    // Рассчитываем stride (выровненную длину строки)
    size_t bytesPerRow = ((static_cast<size_t>(width) * dibHeader.biBitCount / 8) + 3) & ~3;

    // Общий размер данных (с учетом padding)
    size_t imageSize = bytesPerRow * static_cast<size_t>(abs(height));

    // Выделяем память для пиксельных данных
    pixelData.resize(imageSize);

    // Переходим к началу пиксельных данных
    file.seekg(bmpHeader.bfOffBits, std::ios::beg);

    std::vector<uint8_t> tempPixelData(imageSize);
    file.read(reinterpret_cast<char*>(tempPixelData.data()), imageSize);
    
    // Переворачивание строк через цикл

    /*
    Можно было бы сделать чтения данных через общий цикл начиная с конца так как формат .BMP хранит массив пикселей перевёрнутый по вертикали
    НО! тогда каждый раз необходимо было бы вызывать функцию read() что довольго не целисообразно для файлов разных размеров 
    Поэтому более выгодно прочитать одним общим блоком - вызвать функция read() один раз и потом уже работать с эим массивом
    */

    for (auto y = 0; y < abs(height); ++y) {
        memcpy(pixelData.data() + y * bytesPerRow,
               tempPixelData.data() + (abs(height) - 1 - y) * bytesPerRow,
               bytesPerRow);
    }

}

