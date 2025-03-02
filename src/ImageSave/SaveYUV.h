#ifndef SAVE_YUV
#define SAVE_YUV

#include "abs/SaveImage.h"
#include "../cstmIncludes.h"



class SaveYUV : public SaveImage {

public:
    SaveYUV(const std::string& filename);
    ~SaveYUV();

    void save(std::vector<uint8_t>& yBuffer, std::vector<uint8_t>& uBuffer,
        std::vector<uint8_t>& vBuffer, int width, int height) override;

};


#endif //SAVE_YUV