#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class ShaderProgram
{
public:
	ShaderProgram(std::string& vertex_shader_source, std::string& fragment_shader_source);
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;
	~ShaderProgram();

	int getUniformLocation(std::string name);

	void setInt(std::string name, unsigned int value);
	void setFloat(std::string name, float value);

	void setVector(std::string name, glm::vec3 vector);
	void setMatrix(std::string name, glm::mat4 matrix);
	bool isCompiled() { return success; };
	void Use();


private:
	unsigned int id;
	int success;
	char infoLog[512];

	void attachShader(std::string& shader_source, unsigned int shader_type);
};

#endif // !SHADER_PROGRAM_H
