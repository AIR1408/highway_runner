#include <Components/Mesh.h>
#include <glad/glad.h>

Mesh::Mesh(ext::Vertex* vertices, unsigned int size) : VAO(0), VBO(0), IBO(0)
{
    numberOfVertices = size;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, numberOfVertices * sizeof(ext::Vertex), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ext::Vertex), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ext::Vertex), (void*)offsetof(ext::Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ext::Vertex), (void*)offsetof(ext::Vertex, texCoord));

    glEnableVertexArrayAttrib(VAO, 0);
    glEnableVertexArrayAttrib(VAO, 1);
    glEnableVertexArrayAttrib(VAO, 2);

    glBindVertexArray(0);    
}


Mesh::~Mesh()
{
    glDeleteBuffers(1, &IBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::setIndices(unsigned int* indices, unsigned int size)
{
    numberOfIndices = size;

    glBindVertexArray(VAO);

    glGenBuffers(1, &IBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndices * sizeof(GLuint), indices, GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::use()
{
    glBindVertexArray(VAO);

    if (IBO)
        glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);

    glBindVertexArray(0);
}