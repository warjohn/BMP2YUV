#ifndef VIDEO_PROCESSOR
#define VIDEO_PROCESSOR

#include <string>
#include <vector>
#include <stdint.h>
#include <fstream>

class VideoProcessor {
    /*
        Абстрактный класс для обработки видео любых форматов 
    */
public:

    VideoProcessor(const std::string& inputYUV, const std::string& outputYUV) : inputYUV(inputYUV), outputYUV(outputYUV){};
    virtual ~VideoProcessor() = default;


protected:
    const std::string& inputYUV;
    const std::string& outputYUV;
    std::ifstream inputFile;
    std::ofstream outputFile;

    virtual void ProcessVideo(int videoWidth, int videoHeight,
        const std::vector<uint8_t>& yBuffer, const std::vector<uint8_t>& uBuffer,
        const std::vector<uint8_t>& vBuffer, int overlayWidth, int overlayHeight) = 0;

};
#endif //VIDEO_PROCESSOR