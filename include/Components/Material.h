#ifndef MATERIAL_H
#define MATERIAL_H

#define MATERIAL_UNIFORMS_NUMBER 4

#include <memory>
#include <string>

#include <glm/vec3.hpp>

#include <Components/Component.h>

class ShaderProgram;

class Material : public Component
{
public:
    Material(std::shared_ptr<ShaderProgram> shader);
    ~Material();

    void use() override;

    glm::vec3 ambient, diffuse, specular;
	float shininess;
    std::string name;
private:
	int uniformLocations[MATERIAL_UNIFORMS_NUMBER];

};

#endif