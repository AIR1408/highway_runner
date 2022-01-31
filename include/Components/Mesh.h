#ifndef MESH_H
#define MESH_H

#include <Components/Component.h>

#include <extra.h>

class Mesh : public Component
{
public:
	//Mesh(std::vector<ext::Vertex>& vertices);
	Mesh(ext::Vertex* vertices, unsigned int size);
    ~Mesh() override;
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&) = delete;
    
    void setIndices(unsigned int* indices, unsigned int size);
    
    void use() override;
private:
	unsigned int VAO, VBO, IBO;
    unsigned int numberOfVertices, numberOfIndices;
};

#endif