#include "Background.h"

#include "Vertex.h"
#include "TextureManager.h"
#include <vector>

Background::Background()
{
    //ctor
}

void Background::Setup(Shader shader, const std::string &filepath)
{
    texture = TextureManager::LoadTexture(filepath.c_str());

    std::vector<Vertex> vertexArray;
    std::vector<GLuint> indices;

    Vertex v1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
    Vertex v2(glm::vec3(texture->GetWidth(), 0.0f,0.0f), glm::vec2(1.0f, 0.0f));
    Vertex v3(glm::vec3(texture->GetWidth(), 1.0f, 0.0f), glm::vec2(1.0f, 1.0f));
    Vertex v4(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
    vertexArray.push_back(v1);
    vertexArray.push_back(v2);
    vertexArray.push_back(v3);
    vertexArray.push_back(v4);
    GLuint ind[] =
    {
        0, 1, 3,
        1, 2, 3,
    };
    for(int i = 0; i < 6; i++)
        indices.push_back(ind[i]);

    mesh.Setup(vertexArray, indices, texture);

    transUniform = glGetUniformLocation(shader.program, "transMatrix");

    translation = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0, 0.0, 1.0);
    scale = glm::vec3(1.0,1.0,1.0);
    rotationAmount = 0;
}

void Background::update()
{
    //std::cout << scale.y;
    transMatrix = glm::mat4();
    transMatrix = glm::translate(transMatrix, translation);
    transMatrix = glm::rotate(transMatrix, glm::radians(0.0f),  rotation);
    transMatrix = glm::scale(transMatrix, scale);
}

void Background::render()
{
    glUniformMatrix4fv(transUniform, 1, GL_FALSE, glm::value_ptr(transMatrix));
    mesh.render();
}

Background::~Background()
{
    //dtor
}
