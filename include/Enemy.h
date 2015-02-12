#ifndef ENEMY_H
#define ENEMY_H
#include "Texture.h"
#include "Shader.h"
#include "Animation.h"
#include <vector>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TileMap;

class Enemy
{
    public:
        Enemy();
        void Setup(Shader &shader);
        void update(TileMap *tileMap, double dt);
        void render();
        float GetXPos() {return xPos;};
        float GetYPos() {return yPos;};
        virtual ~Enemy();
    protected:
    private:
        void SetupMesh();
        float xPos, yPos, xSpeed, ySpeed, gravity;
        int direction;
        bool jumping;
        GLuint transUniform, projUniform;
        GLuint vao, vbo, tbo;
        glm::vec3 translation, rotation, scale;
        glm::mat4 transMatrix;
        float rotationAmount;
        Texture texture;
        std::vector<std::vector<float>> texCoords;
        Animation rightAnim, leftAnim;
        int texCoordsIndex;
};

#endif // ENEMY_H
