#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC

#include <memory>
#include <string>
#include <vector>

#include <extra.h>

class ShaderProgram;
class Container;
class Texture2D;
class Material;
class Mesh;

class FileLoader
{
public:
	FileLoader(std::string resourcePath);
	FileLoader(const FileLoader&) = delete;
	FileLoader& operator=(const FileLoader&) = delete;

	std::string	loadShader(std::string fileName);
	Container   loadObjFile(std::string fileName, std::vector<std::shared_ptr<Material>>& materials);
	Container   loadObjFile(std::string fileName);

	std::vector<std::shared_ptr<Material>> 	 
	loadMtlFile(std::string fileName, std::shared_ptr<ShaderProgram> shader);

	std::shared_ptr<Texture2D> loadTexture2D(std::string fileName, int unit);

	std::shared_ptr<ShaderProgram> loadShaderProgram();
	
private:	
	std::string resourcePath;
};

#endif // !RESOURCE_MANAGER_H
