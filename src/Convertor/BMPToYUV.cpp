#include "BMPToYUV.h"

#include <iostream>

BMPToYUVConverter::~BMPToYUVConverter() {
    for (auto& thread : threads) {
        thread.join();
    }
}

void BMPToYUVConverter::BgrToY(const uint8_t* bgr, size_t bgrStride, size_t startRow, size_t endRow, size_t width,
                                 uint8_t* y, size_t yStride) {
    for (size_t row = startRow; row < endRow; ++row) {
        for (size_t col = 0; col < width; ++col) {
            size_t bgrOffset = row * bgrStride + col * 3;
            uint8_t r = bgr[bgrOffset + 2];
            uint8_t g = bgr[bgrOffset + 1];
            uint8_t b = bgr[bgrOffset + 0];
            y[row * yStride + col] = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);
        }
    }
}

void BMPToYUVConverter::BgrToUV(const uint8_t* bgr, size_t bgrStride, size_t startRow, size_t endRow, size_t width, size_t height,
                                  uint8_t* u, size_t uStride, uint8_t* v, size_t vStride) {
    for (size_t row = startRow; row < endRow; row += 2) {
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

    size_t rowsPerThread = height / numThreads; // Расчет строк для каждого потока

    for (size_t i = 0; i < numThreads; ++i) {
        size_t startRow = i * rowsPerThread;
        size_t endRow = (i == numThreads - 1) ? height : (i + 1) * rowsPerThread;

        threads.emplace_back([=]() {
            BgrToY(bgr, bgrStride, startRow, endRow, width, y, yStride);
            BgrToUV(bgr, bgrStride, startRow, endRow, width, height, u, uStride, v, vStride);
        });
    }
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


