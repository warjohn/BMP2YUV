#include "src/BMPToYUV.h"
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {
    if (argc != 5) {
        throw std::invalid_argument("Необходимо указать 4 аргумента. 1 - Путь до файла изображения; 2 - Путь до файла видео; 3 - Video width; 4 - Video height");
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

    std::cout << "Image shape - " << width << "x" << height << std::endl;

    size_t ySize = width * height;
    size_t uvSize = (width / 2) * (height / 2);

    std::vector<uint8_t> yBuffer(ySize);
    std::vector<uint8_t> uBuffer(uvSize);
    std::vector<uint8_t> vBuffer(uvSize);

    converter.ConvertBGRToYUV420P(pixelData, width, height, yBuffer, uBuffer, vBuffer);
    converter.InvertYUVVertically(yBuffer, uBuffer, vBuffer, width, height);

    const int VideoWidth = std::atoi(argv[3]);  // Узнал из просмоторщика vooya 352x288 - тестовое видео
    const int VideoHeight = std::atoi(argv[4]);

    processor.ProcessVideo(video, outputYUVVideo, VideoWidth, VideoHeight, yBuffer, uBuffer, vBuffer, width, height);

    converter.SaveYUV(outputYUV, yBuffer, uBuffer, vBuffer, width, height);
    std::cout << "YUV файл успешно сохранен!" << std::endl;

    return 0;
}
