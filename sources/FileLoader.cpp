#include <FileLoader.h>
#include <glad/glad.h>
#include <WavefrontReader.h>
#include <stb_image.h>
#include <Components/Texture2D.h>
#include <Components/Material.h>
#include <Components/Mesh.h>
#include <Container.h>
#include <ShaderProgram.h>
#include <sstream>

FileLoader::FileLoader(std::string resourcePath)
{
    this->resourcePath = resourcePath;
}

std::string FileLoader::loadShader(std::string fileName)
{
    std::ifstream fin(resourcePath + "/shaders/" + fileName);
    if (!fin.is_open())
        throw "ERROR::FILE_LOADER Can't open file " + fileName;
    std::ostringstream buf;
    buf << fin.rdbuf();
    fin.close();
    return buf.str();
}

std::shared_ptr<Texture2D> FileLoader::loadTexture2D(std::string fileName, int unit)
{
    std::string path = resourcePath + "/textures/" + fileName;
    int width = 0, height = 0, channels = 0;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!pixels)
        throw "ERROR::FILE_LOADER Can't load image: " + fileName;

    std::shared_ptr<Texture2D> p_texture = 
        std::make_shared<Texture2D>(pixels, glm::uvec3(width, height, channels), unit);
        
    stbi_image_free(pixels);
    return p_texture;
}

std::shared_ptr<ShaderProgram> FileLoader::loadShaderProgram()
{
    std::string vertex_shader   = loadShader("/vertex.txt");
    std::string fragment_shader = loadShader("/fragment.txt");

    auto ID = std::make_shared<ShaderProgram>(vertex_shader, fragment_shader);
    return ID;
}

Container
FileLoader::loadObjFile(std::string fileName, std::vector<std::shared_ptr<Material>>& materials)
{
    WavefrontReader wavefrontReader(resourcePath + "/models/" + fileName);
    std::string type, buffer, name;
    bool isFirst = true;

    Container container;

    std::vector<glm::vec3> positions, normals;
    std::vector<glm::vec2> texCoords;

    positions.emplace_back(glm::vec3(0));
    texCoords.emplace_back(glm::vec2(0));
    normals.emplace_back(glm::vec3(0));
    
    std::vector<ext::Vertex> vertices;

    while (!wavefrontReader.eof())
    {
        wavefrontReader.readLine();
        type = wavefrontReader.getString();
        if      (type == "v")  positions.push_back(wavefrontReader.getVec3());
        else if (type == "vt") texCoords.push_back(wavefrontReader.getVec2());
        else if (type == "vn") normals.push_back(wavefrontReader.getVec3());
        else if (type == "f")
        {
            if (vertices.empty())
                vertices.reserve(positions.size() * 3);

            unsigned int i = 3;
            while (i--)
            {
                buffer = wavefrontReader.getString();

                GLuint indices[3] = { 0, 0, 0 }, *pIndex = indices;
                
                for (const auto& sym : buffer)
                {
                    if (sym >= '0' && sym <= '9')
                        *pIndex = 10 * (*pIndex) + sym - '0';
                    else if (sym == '/') 
                        pIndex++;
                }

                vertices.push_back({
                    positions[indices[0]],
                    texCoords[indices[1]],
                    normals  [indices[2]]   
                });
            }
        }
        else if (type == "o" || type == "g")
        {
            if (!vertices.empty())
            {
                container.addComponentBack(std::make_shared<Mesh>(vertices.data(), vertices.size()));        
                vertices.clear();
            }
        }
        else if (type == "usemtl")
        {
            buffer = wavefrontReader.getString();
            bool isFound = name == buffer;
            name = buffer;
            for (unsigned int i = 0; i < materials.size() && !isFound; i++)
                if (materials[i]->name == name)
                {
                    container.addComponentBack(materials[i]);
                    isFound = true;
                }
            //if (!isFound)
                //throw "ERROR::FILE_LOADER wrong .mtl file!";
        }
    }
    if (!vertices.empty())
    {
        container.addComponentBack(std::make_shared<Mesh>(vertices.data(), vertices.size()));        
        vertices.clear();
    }

    return container;
}

Container FileLoader::loadObjFile(std::string fileName)
{
    std::vector<std::shared_ptr<Material>> emptyVector;
    return loadObjFile(fileName, emptyVector);
}


std::vector<std::shared_ptr<Material>> 
FileLoader::loadMtlFile(std::string fileName, std::shared_ptr<ShaderProgram> shader)
{
    WavefrontReader wavefrontReader(resourcePath + "/models/" + fileName);
    std::string type;

    std::vector<std::shared_ptr<Material>> materials;
    std::shared_ptr<Material> material = std::make_shared<Material>(shader);
    bool isFirst = true;

    while (!wavefrontReader.eof())
    {
        wavefrontReader.readLine();
        type = wavefrontReader.getString();
        
        if (type == "newmtl")
        {
            if (isFirst)
                isFirst = false;
            else
            {
                materials.push_back(material);
                material = nullptr;
                material = std::make_shared<Material>(shader);
            }
            material->name = wavefrontReader.getString();
        }
        else if (type == "Ka")     material->ambient = wavefrontReader.getVec3();
        else if (type == "Kd")     material->diffuse = wavefrontReader.getVec3();
        else if (type == "Ks")     material->specular = wavefrontReader.getVec3();
        else if (type == "Ns")     material->shininess = wavefrontReader.getFloat();
        // else if (type == "map_Ka") material.addMap(loadTexture(line.getString());
        // else if (type == "map_Kd") material.addMap(loadTexture(line.getString());
        // else if (type == "map_Ks") material.addMap(loadTexture(line.getString());
    }
    materials.push_back(material);
    return materials;
}