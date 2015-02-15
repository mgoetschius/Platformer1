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
        Enemy(Shader &shader, const char *filename, float x = 650, float y = 400);
        virtual void Update(TileMap *tileMap, double dt);
        void render();
        float GetXPos() {return xPos;};
        float GetYPos() {return yPos;};
        void SetIsDead(bool b) {isDead = b;};
        bool GetIsDead() {return isDead;};
        void SetRemove(bool b) {remove = b;};
        bool GetRemove() {return remove;};
        virtual ~Enemy();
    protected:
        void SetupMesh(const char *filename);
        float xPos, yPos, xSpeed, ySpeed, gravity;
        int direction;
        bool jumping;
        bool isDead, remove;
        GLuint transUniform, projUniform;
        GLuint vao, vbo, tbo;
        glm::vec3 translation, rotation, scale;
        glm::mat4 transMatrix;
        float rotationAmount;
        Texture texture;
        std::vector<std::vector<float>> texCoords;
        Animation rightAnim, leftAnim;
        int texCoordsIndex;
    private:

};

#endif // ENEMY_H
