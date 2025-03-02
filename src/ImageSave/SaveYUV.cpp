#include "SaveYUV.h"

SaveYUV::SaveYUV(const std::string& filename) : SaveImage(filename) {
    file.open(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("failed with open file");
    }
}

SaveYUV::~SaveYUV() { 
    if (file.is_open()) {
        file.close();
    }
}


void SaveYUV::save(std::vector<uint8_t>& yBuffer, std::vector<uint8_t>& uBuffer,
    std::vector<uint8_t>& vBuffer, int width, int height) {
    
    size_t ySize = width * height;
    size_t uvSize = (width / 2) * (height / 2);

    file.write(reinterpret_cast<const char*>(yBuffer.data()), ySize);
    file.write(reinterpret_cast<const char*>(uBuffer.data()), uvSize);
    file.write(reinterpret_cast<const char*>(vBuffer.data()), uvSize);
    yBuffer.clear();
    uBuffer.clear();
    vBuffer.clear();
}