#include <ShaderProgram.h>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

ShaderProgram::ShaderProgram(std::string& vertexShaderSource, std::string& fragmentShaderSource) : id(0)
{
    id = glCreateProgram();

    attachShader(vertexShaderSource, GL_VERTEX_SHADER);
    attachShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        throw "ERROR::SHADER::PROGRAM::LINKING_FAILED " + std::string(infoLog);
    }
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(id);
}

GLint ShaderProgram::getUniformLocation(std::string name)
{
    return glGetUniformLocation(id, name.c_str());
}

void ShaderProgram::setInt(std::string name, unsigned int value)
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::setFloat(std::string name, float value)
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::setVector(std::string name, glm::vec3 vector)
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), vector.x, vector.y, vector.z);
}

void ShaderProgram::setMatrix(std::string name, glm::mat4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::Use()
{
    glUseProgram(id);
}

void ShaderProgram::attachShader(std::string& shader_source, unsigned int shader_type)
{
    GLuint shader_ID = glCreateShader(shader_type);
    const GLchar* code = shader_source.c_str();
    glShaderSource(shader_ID, 1, &code, nullptr);
    glCompileShader(shader_ID);
    glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &success);
    if (success)
    {
        glAttachShader(id, shader_ID);
        glDeleteShader(shader_ID);
    }
    else 
    {
        glGetShaderInfoLog(shader_ID, 512, nullptr, infoLog);
        throw "ERROR::SHADER::COMPILATION_FAILED " + std::string(infoLog);
    }
}
