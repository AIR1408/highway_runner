#include <WavefrontReader.h>
#include <sstream>
#include <string>

WavefrontReader::WavefrontReader(std::string fileName)
{
    _fin.open(fileName);
    if (!_fin.is_open())
        throw "ERROR::FILE_LOADER Cannot open file " + fileName;
    _line = new std::stringstream;
}

WavefrontReader::~WavefrontReader()
{
    delete _line;
    _fin.close();
}

bool WavefrontReader::eof()
{
    return _fin.eof();
}

void WavefrontReader::readLine()
{
    char buffer[256];
    _fin.getline(buffer, 256);
    _line->str(buffer);
    _line->clear();
}

std::string WavefrontReader::getString()
{
    char buffer[64] = "";
    *_line >> buffer;
    return buffer;
}

float WavefrontReader::getFloat()
{
    float temp;
    *_line >> temp;
    return temp;
}

glm::vec2 WavefrontReader::getVec2()
{
    glm::vec2 temp;
    *_line >> temp.x >> temp.y;
    return temp;
}

glm::vec3 WavefrontReader::getVec3()
{
    glm::vec3 temp;
    *_line >> temp.x >> temp.y >> temp.z;
    return temp;
}
