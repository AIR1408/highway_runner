#include <Components/Material.h>
#include <glad/glad.h>
#include <ShaderProgram.h>
#include <memory>

Material::Material(std::shared_ptr<ShaderProgram> shader)
{
    std::string uniformNames[MATERIAL_UNIFORMS_NUMBER] = 
	{
		"material.ambient",
		"material.diffuse",
		"material.specular",
		"material.shininess"
	};

	for (int i = 0; i < MATERIAL_UNIFORMS_NUMBER; i++)
		uniformLocations[i] = shader->getUniformLocation(uniformNames[i]);
}

Material::~Material()
{
	
}

void Material::use()
{
    glUniform3f(uniformLocations[0], ambient.x, ambient.y, ambient.z);
	glUniform3f(uniformLocations[1], diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(uniformLocations[2], specular.x, specular.y, specular.z);
	glUniform1f(uniformLocations[3], shininess);
}
