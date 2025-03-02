#ifndef VIDEO_YUV
#define VIDEO_YUV

#include "abs/VideoProcessor.h"
#include "../cstmIncludes.h"

class VideoYUV : public VideoProcessor {
public:
    VideoYUV(const std::string& inputYUV, const std::string& outputYUV);
    ~VideoYUV();

    void ProcessVideo(int videoWidth, int videoHeight,
        const std::vector<uint8_t>& yBuffer, const std::vector<uint8_t>& uBuffer,
        const std::vector<uint8_t>& vBuffer, int overlayWidth, int overlayHeight) override;

private:
    
    bool InsertOverlay(std::vector<uint8_t>& frameData, const std::vector<uint8_t>& yBuffer,
        const std::vector<uint8_t>& uBuffer, const std::vector<uint8_t>& vBuffer,
        int frameWidth, int frameHeight, int overlayWidth, int overlayHeight);
    
};


#endif //VIDEO_YUV