#include "VideoYUV.h"




VideoYUV::VideoYUV(const std::string& inputYUV, const std::string& outputYUV) : VideoProcessor(inputYUV, outputYUV) {
    inputFile.open(inputYUV, std::ios::binary);
    outputFile.open(outputYUV, std::ios::binary);

    if (!inputFile.is_open()) {
        throw std::runtime_error("Failed with open input file");
    }
    if (!outputFile.is_open()) {
        throw std::runtime_error("Failed with open output file");
    }
}

VideoYUV::~VideoYUV(){
    inputFile.close();
    outputFile.close();
}

bool VideoYUV::InsertOverlay(std::vector<uint8_t>& frameData, const std::vector<uint8_t>& yBuffer,
                             const std::vector<uint8_t>& uBuffer, const std::vector<uint8_t>& vBuffer,
                             int frameWidth, int frameHeight, int overlayWidth, int overlayHeight) {

    uint8_t* frameY = frameData.data();
    uint8_t* frameU = frameData.data() + frameWidth * frameHeight;
    uint8_t* frameV = frameU + (frameWidth / 2) * (frameHeight / 2);

    // Копирование Y-компонента
    for (int i = 0; i < overlayHeight; ++i) {
        memcpy(frameY + i * frameWidth, yBuffer.data() + i * overlayWidth, overlayWidth);
    }

    // Копирование U-компонента
    for (int i = 0; i < overlayHeight / 2; ++i) {
        memcpy(frameU + i * (frameWidth / 2), uBuffer.data() + i * (overlayWidth / 2), overlayWidth / 2);
    }

    // Копирование V-компонента
    for (int i = 0; i < overlayHeight / 2; ++i) {
        memcpy(frameV + i * (frameWidth / 2), vBuffer.data() + i * (overlayWidth / 2), overlayWidth / 2);
    }

    return true;
}

void VideoYUV::ProcessVideo(int videoWidth, int videoHeight,
                                  const std::vector<uint8_t>& yBuffer, const std::vector<uint8_t>& uBuffer,
                                  const std::vector<uint8_t>& vBuffer, int overlayWidth, int overlayHeight) {
    size_t frameSize = videoWidth * videoHeight * 3 / 2;
    std::vector<uint8_t> videoData(frameSize);

    while (inputFile.read(reinterpret_cast<char*>(videoData.data()), frameSize)) {
        if (!InsertOverlay(videoData, yBuffer, uBuffer, vBuffer, videoWidth, videoHeight, overlayWidth, overlayHeight)) {
            throw std::runtime_error("Error with overlaying image");
        }

        outputFile.write(reinterpret_cast<const char*>(videoData.data()), frameSize);
        if (!outputFile) {
            throw std::runtime_error("Error with writing output file");
        }
    }
}


