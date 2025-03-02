#include "src/ImageReader/BMPreader.h"
#include "src/Convertor/BMPToYUV.h"
#include "src/VideoProcess/VideoYUV.h"
#include "src/ImageSave/SaveYUV.h"

#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {
    if (argc < 5) {
        throw std::invalid_argument("Необходимо указать как минимум 4 аргумента: <путь_изображения> <путь_видео> <ширина_видео> <высота_видео>");
    } 
    // consts 
    const std::string csinputBMP = argv[1];
    const std::string csvideo = argv[2];
    const int csVideoWidth = std::atoi(argv[3]);
    const int csVideoHeight = std::atoi(argv[4]);
  
    // vars
    int width, height;
    std::vector<uint8_t> pixelData;
    std::vector<uint8_t> yBuffer;
    std::vector<uint8_t> uBuffer;
    std::vector<uint8_t> vBuffer;
    const std::string outputYUV = "outputImage.yuv"; // Путь к YUV выходному файлу изображнию
    const std::string outputYUVVideo = "outputVideo.yuv"; // путь до файла видео  


    // classes 
    BMPreader reader(csinputBMP, width, height, pixelData);
    BMPToYUVConverter converter;
    VideoYUV videoYUV(csvideo, outputYUVVideo);
    SaveYUV saveImage(outputYUV);

    // process
    reader.read();
    if (width > csVideoWidth || height > csVideoHeight) { 
        throw std::runtime_error("Pls use images with small or same size like a video. Resize function will be add in future addons");
    } 
    converter.ConvertBGRToYUV420P(pixelData, width, height, yBuffer, uBuffer, vBuffer);
    videoYUV.ProcessVideo(csVideoWidth, csVideoHeight, yBuffer, uBuffer, vBuffer, width, height);
    saveImage.save(yBuffer, uBuffer, vBuffer, width, height);
    std::cout << "YUV файл успешно сохранен!" << std::endl;

    return 0;
}
