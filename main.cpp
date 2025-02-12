#include "src/BMPToYUV.h"
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {
    if (argc != 3) {
        throw std::invalid_argument("Необходимо указать 2 аргумента. 1 - Путь до файла изображения; 2 - Путь до файла видео");
    }
    const std::string inputBMP = argv[1];  // Путь к BMP изображению
    const std::string video = argv[2];

    const std::string outputYUV = "outputImage.yuv"; // Путь к YUV выходному файлу изображнию
    const std::string outputYUVVideo = "outputVideo.yuv"; // путь до файла видео  

    BMPToYUVConverter converter;
    VideoProcessor processor;

    int width, height;
    std::vector<uint8_t> pixelData;

    if (!converter.ReadBMP(inputBMP, pixelData, width, height)) {
        return -1;
    }

    std::cout << width << " - " << height << std::endl;

    size_t ySize = width * height;
    size_t uvSize = (width / 2) * (height / 2);

    std::vector<uint8_t> yBuffer(ySize);
    std::vector<uint8_t> uBuffer(uvSize);
    std::vector<uint8_t> vBuffer(uvSize);

    converter.ConvertBGRToYUV420P(pixelData, width, height, yBuffer, uBuffer, vBuffer);
    converter.InvertYUVVertically(yBuffer, uBuffer, vBuffer, width, height);

    const int VideoWidth = 352;  // Узнал из просмоторщика vooya 
    const int VideoHeight = 288;

    processor.ProcessVideo(video, outputYUVVideo, VideoWidth, VideoHeight, yBuffer, uBuffer, vBuffer, width, height);

    converter.SaveYUV(outputYUV, yBuffer, uBuffer, vBuffer, width, height);
    std::cout << "YUV файл успешно сохранен!" << std::endl;

    return 0;
}