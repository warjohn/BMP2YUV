#ifndef SAVE_IMAGE
#define SAVE_IMAGE

#include <string>
#include <vector>
#include <stdint.h>
#include <fstream>

class SaveImage {
    /*
    Абстрактный класс для сохранения изображения разных форматов
    */

public:

    SaveImage(const std::string& filename) : filename(filename){};
    virtual ~SaveImage() = default;


protected:
    const std::string& filename;
    std::ofstream file;
    // save yuv 
    virtual void save(std::vector<uint8_t>& yBuffer, std::vector<uint8_t>& uBuffer,
        std::vector<uint8_t>& vBuffer, int width, int height) = 0;

    // save png
    // ..

};


#endif //SAVE_IMAGE