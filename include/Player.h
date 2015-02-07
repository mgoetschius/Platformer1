#ifndef PLAYER_H
#define PLAYER_H

#include "Texture.h"
#include "TextureChangeMesh.h"
#include "Shader.h"
#include "TileMap.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Player
{
    public:
        Player();
        void Setup(const Shader&);
        void update(TileMap&);
        void render();
        float GetXPos() {return xPos;};
        float GetYPos() {return yPos;};
        virtual ~Player();
    protected:
    private:
        float xPos, yPos, xSpeed, ySpeed;
        float gravity;
        bool jumping = false;
        GLuint transUniform, projUniform;
        GLuint vao, vbo, tbo, ibo;
        std::vector<GLuint> indices;
        Shader shader;
        Texture texture;
        TextureChangeMesh mesh;
        glm::vec3 translation, rotation, scale;
        glm::mat4 transMatrix;
        float rotationAmount;
        void SetupMesh();
};

#endif // PLAYER_H
