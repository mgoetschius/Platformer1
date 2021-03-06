#include "Mesh.h"
#include "Vertex.h"
#include <iostream>

Mesh::Mesh()
{

}

void Mesh::Setup(std::vector<Vertex> vert, std::vector<GLuint> ind, Texture *&tex)
{
    totalIndices = ind.size();
    texture = tex;

    int arraySize = 5 * ind.size();
    GLfloat vertices[arraySize];

    int j = 0;
    for(int i = 0; i < arraySize; i+=5 )
    {
        vertices[i] = vert[j].GetPosition().x;
        vertices[i+1] = vert[j].GetPosition().y;
        vertices[i+2] = vert[j].GetPosition().z;
        vertices[i+3] = vert[j].GetTexCoord().x;
        vertices[i+4] = vert[j].GetTexCoord().y;
        j++;
        if(j > ind.size())
        std::cout << "size: " << ind.size() << " j: " << j << std::endl;
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(GLuint), &ind[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void Mesh::update()
{

}

void Mesh::render()
{
    glBindVertexArray(vao);
    //make sure soil is loading RGBA image
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTexture());
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawElements(GL_TRIANGLES, totalIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

Mesh::~Mesh()
{
}
