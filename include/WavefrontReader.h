#ifndef WAVEFRONT_READER_H
#define WAVEFRONT_READER_H

#include <string>
#include <fstream>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class WavefrontReader
{
public:
    WavefrontReader(std::string fileName);
    ~WavefrontReader();

    bool eof();
    void readLine();
    
    std::string getString();
    float       getFloat();
    glm::vec3   getVec3();
    glm::vec2   getVec2();

private:
    std::stringstream* _line;
    std::ifstream _fin;
};

#endif 