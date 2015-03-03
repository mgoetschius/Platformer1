#include "Model.h"
#include "Vertex.h"
#include "Texture.h"
#include "Game.h"
#include <vector>
#include <string>

Model::Model()
{
    //ctor
}

void Model::Setup(Shader &shader, std::string name)
{
    getMesh(name);
    transUniform = glGetUniformLocation(shader.program, "transMatrix");

    translation = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0, 0.0, 1.0);
    scale = glm::vec3(0.0,0.0,0.0);
    rotationAmount = 0;

    transMatrix = glm::translate(transMatrix, translation);
    //transMatrix = glm::rotate(transMatrix, glm::radians(rotationAmount),  rotation);
    transMatrix = glm::scale(transMatrix, scale);
}



void Model::update()
{
    transMatrix = glm::mat4();
    transMatrix = glm::translate(transMatrix, translation);
    transMatrix = glm::rotate(transMatrix, glm::radians(0.0f),  rotation);
    transMatrix = glm::scale(transMatrix, scale);
    transMatrix = transMatrix;

    //mesh.update();
}

void Model::render()
{
    glUniformMatrix4fv(transUniform, 1, GL_FALSE, glm::value_ptr(transMatrix));
    mesh.render();
}

void Model::getMesh(std::string name)
{
    std::vector<Vertex> vertexArray;
    std::vector<GLuint> indices;

        if(name == "1")
        {
            Vertex v1(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(0.5f, 0.5f));
            Vertex v2(glm::vec3(1.0f, 0.0f,0.0f), glm::vec2(0.5f, 0.0f));
            Vertex v3(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
            Vertex v4(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.5f));
            vertexArray.push_back(v1);
            vertexArray.push_back(v2);
            vertexArray.push_back(v3);
            vertexArray.push_back(v4);
        }
        else
        {
            Vertex v1(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.5f));
            Vertex v2(glm::vec3(1.0f, 0.0f,0.0f), glm::vec2(1.0f, 0.0f));
            Vertex v3(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f, 0.0f));
            Vertex v4(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 0.5f));
            vertexArray.push_back(v1);
            vertexArray.push_back(v2);
            vertexArray.push_back(v3);
            vertexArray.push_back(v4);
        }

        GLuint ind[] = {
        0, 1, 3,
        1, 2, 3,
        };
        for(int i = 0; i < 6; i++)
        indices.push_back(ind[i]);

        //Texture texture;
        //std::string path = "./res/textures/" + name + ".png";
        //texture.Setup(path.c_str());
        //mesh.Setup(vertexArray, indices, texture);
        mesh.Setup(vertexArray, indices, Level1::texture);
}

Model::~Model()
{
    //dtor
}
