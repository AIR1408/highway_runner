#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <glm/vec3.hpp>

#include <Components/Component.h>

class Texture2D : public Component
{
public:
    Texture2D(unsigned char* pixels, glm::uvec3 properties, int unit);
    ~Texture2D();
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;
    Texture2D& operator=(Texture2D&) = delete;

    void use() override;
private:
    unsigned int id;
    int unit;
};

#endif