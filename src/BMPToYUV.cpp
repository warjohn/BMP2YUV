#include "BMPToYUV.h"
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

void BMPToYUVConverter::BgrToY(const uint8_t* bgr, size_t bgrStride, size_t width, size_t height,
                               uint8_t* y, size_t yStride) {
    for (size_t row = 0; row < height; ++row) {
        for (size_t col = 0; col < width; ++col) {
            size_t bgrOffset = row * bgrStride + col * 3;
            uint8_t r = bgr[bgrOffset + 2];
            uint8_t g = bgr[bgrOffset + 1];
            uint8_t b = bgr[bgrOffset + 0];
            y[row * yStride + col] = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);
        }
    }
}

void BMPToYUVConverter::BgrToUV(const uint8_t* bgr, size_t bgrStride, size_t width, size_t height,
                                uint8_t* u, size_t uStride, uint8_t* v, size_t vStride) {
    for (size_t row = 0; row < height; row += 2) {
        for (size_t col = 0; col < width; col += 2) {
            int sumU = 0, sumV = 0;
            int count = 0;
            for (int dy = 0; dy < 2; ++dy) {
                for (int dx = 0; dx < 2; ++dx) {
                    if (row + dy >= height || col + dx >= width) continue;
                    size_t bgrOffset = (row + dy) * bgrStride + (col + dx) * 3;
                    uint8_t r = bgr[bgrOffset + 2];
                    uint8_t g = bgr[bgrOffset + 1];
                    uint8_t b = bgr[bgrOffset + 0];
                    int uVal = -0.169 * r - 0.331 * g + 0.499 * b + 128;
                    int vVal = 0.499 * r - 0.419 * g - 0.081 * b + 128;
                    sumU += std::min(255, std::max(0, uVal));
                    sumV += std::min(255, std::max(0, vVal));
                    count++;
                }
            }
            if (count > 0) {
                u[(row / 2) * uStride + (col / 2)] = static_cast<uint8_t>(sumU / count);
                v[(row / 2) * vStride + (col / 2)] = static_cast<uint8_t>(sumV / count);
            }
        }
    }
}

void BMPToYUVConverter::BgrToYuv420pParallel(const uint8_t* bgr, size_t bgrStride, size_t width, size_t height,
                                             uint8_t* y, size_t yStride, uint8_t* u, size_t uStride, uint8_t* v, size_t vStride) {
    if (width % 2 != 0) width++;
    if (height % 2 != 0) height++;

    std::thread threadY(&BMPToYUVConverter::BgrToY, this, bgr, bgrStride, width, height, y, yStride);
    std::thread threadUV(&BMPToYUVConverter::BgrToUV, this, bgr, bgrStride, width, height, u, uStride, v, vStride);

    threadY.join();
    threadUV.join();
}

bool BMPToYUVConverter::ReadBMP(const std::string& filename, std::vector<uint8_t>& pixelData, int& width, int& height) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Ошибка при открытии BMP файла!" << std::endl;
        return false;
    }

    BMPHeader bmpHeader;
    file.read(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));
    if (bmpHeader.bfType != 0x4D42) {
        std::cerr << "Неизвестный формат BMP!" << std::endl;
        return false;
    }

    DIBHeader dibHeader;
    file.read(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader));
    width = dibHeader.biWidth;
    height = dibHeader.biHeight;

    if (dibHeader.biBitCount != 24) {
        std::cerr << "Только 24-битные BMP изображения поддерживаются!" << std::endl;
        return false;
    }

    size_t imageSize = dibHeader.biSizeImage;
    pixelData.resize(imageSize);
    file.seekg(bmpHeader.bfOffBits, std::ios::beg);
    file.read(reinterpret_cast<char*>(pixelData.data()), imageSize);

    return true;
}

void BMPToYUVConverter::ConvertBGRToYUV420P(const std::vector<uint8_t>& bgrData, int width, int height,
                                            std::vector<uint8_t>& yBuffer, std::vector<uint8_t>& uBuffer, std::vector<uint8_t>& vBuffer) {
    size_t ySize = width * height;
    size_t uvSize = (width / 2) * (height / 2);

    yBuffer.resize(ySize);
    uBuffer.resize(uvSize);
    vBuffer.resize(uvSize);

    BgrToYuv420pParallel(bgrData.data(), width * 3, width, height, yBuffer.data(), width,
                         uBuffer.data(), width / 2, vBuffer.data(), width / 2);
}

void BMPToYUVConverter::InvertYUVVertically(std::vector<uint8_t>& yBuffer, std::vector<uint8_t>& uBuffer, std::vector<uint8_t>& vBuffer,
                                            int width, int height) {
    size_t ySize = width * height;
    size_t uvSize = (width / 2) * (height / 2);

    for (size_t row = 0; row < height / 2; ++row) {
        size_t topRowOffset = row * width;
        size_t bottomRowOffset = (height - row - 1) * width;
        for (size_t col = 0; col < width; ++col) {
            std::swap(yBuffer[topRowOffset + col], yBuffer[bottomRowOffset + col]);
        }
    }

    for (size_t row = 0; row < (height / 2) / 2; ++row) {
        size_t topRowOffsetU = row * (width / 2);
        size_t bottomRowOffsetU = ((height / 2) - row - 1) * (width / 2);
        size_t topRowOffsetV = row * (width / 2);
        size_t bottomRowOffsetV = ((height / 2) - row - 1) * (width / 2);

        for (size_t col = 0; col < width / 2; ++col) {
            std::swap(uBuffer[topRowOffsetU + col], uBuffer[bottomRowOffsetU + col]);
            std::swap(vBuffer[topRowOffsetV + col], vBuffer[bottomRowOffsetV + col]);
        }
    }
}

void BMPToYUVConverter::SaveYUV(const std::string& filename, const std::vector<uint8_t>& yBuffer, const std::vector<uint8_t>& uBuffer,
                                const std::vector<uint8_t>& vBuffer, int width, int height) {
    size_t ySize = width * height;
    size_t uvSize = (width / 2) * (height / 2);

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Ошибка при открытии YUV файла для записи!" << std::endl;
        return;
    }

    file.write(reinterpret_cast<const char*>(yBuffer.data()), ySize);
    file.write(reinterpret_cast<const char*>(uBuffer.data()), uvSize);
    file.write(reinterpret_cast<const char*>(vBuffer.data()), uvSize);
}

bool VideoProcessor::InsertOverlay(std::vector<uint8_t>& frameData, const std::vector<uint8_t>& yBuffer,
                                   const std::vector<uint8_t>& uBuffer, const std::vector<uint8_t>& vBuffer,
                                   int frameWidth, int frameHeight, int overlayWidth, int overlayHeight) {
    size_t overlaySizeY = overlayWidth * overlayHeight;
    size_t overlaySizeUV = (overlayWidth / 2) * (overlayHeight / 2);

    uint8_t* frameY = frameData.data();
    uint8_t* frameU = frameData.data() + frameWidth * frameHeight;
    uint8_t* frameV = frameU + (frameWidth / 2) * (frameHeight / 2);

    for (int i = 0; i < overlayHeight; ++i) {
        for (int j = 0; j < overlayWidth; ++j) {
            size_t yIndex = i * overlayWidth + j;
            size_t frameYIndex = i * frameWidth + j;
            frameY[frameYIndex] = yBuffer[yIndex];
        }
    }

    for (int i = 0; i < overlayHeight / 2; ++i) {
        for (int j = 0; j < overlayWidth / 2; ++j) {
            size_t uvIndex = i * (overlayWidth / 2) + j;
            size_t frameUIndex = (i * (frameWidth / 2)) + j;
            size_t frameVIndex = frameUIndex;

            frameU[frameUIndex] = uBuffer[uvIndex];
            frameV[frameVIndex] = vBuffer[uvIndex];
        }
    }

    return true;
}

bool VideoProcessor::ProcessVideo(const std::string& inputYUV, const std::string& outputYUV, int videoWidth, int videoHeight,
                                  const std::vector<uint8_t>& yBuffer, const std::vector<uint8_t>& uBuffer,
                                  const std::vector<uint8_t>& vBuffer, int overlayWidth, int overlayHeight) {
    std::ifstream inputFile(inputYUV, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Ошибка открытия входного файла!" << std::endl;
        return false;
    }

    std::ofstream outputFile(outputYUV, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Ошибка открытия выходного файла!" << std::endl;
        return false;
    }

    size_t frameSize = videoWidth * videoHeight * 3 / 2;
    std::vector<uint8_t> videoData(frameSize);

    while (inputFile.read(reinterpret_cast<char*>(videoData.data()), frameSize)) {
        if (!InsertOverlay(videoData, yBuffer, uBuffer, vBuffer, videoWidth, videoHeight, overlayWidth, overlayHeight)) {
            std::cerr << "Ошибка вставки изображения!" << std::endl;
            return false;
        }

        outputFile.write(reinterpret_cast<const char*>(videoData.data()), frameSize);
        if (!outputFile) {
            std::cerr << "Ошибка записи в выходной файл!" << std::endl;
            return false;
        }
    }

    if (inputFile.eof()) {
        std::cout << "Видео обработано успешно." << std::endl;
    } else {
        std::cerr << "Ошибка при чтении входного файла." << std::endl;
        return false;
    }

    return true;
}