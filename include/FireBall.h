#ifndef FIREBALL_H
#define FIREBALL_H
#include "Texture.h"
#include "Shader.h"
#include "TileMap.h"
#include "Animation.h"
#include <vector>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class FireBall
{
    public:
        FireBall();
        FireBall(const Shader &shader, float x, float y, int direction);
        void Setup(const Shader &shader, float x, float y);
        void Update(TileMap tileMap, double dt, float playerXPos);
        float GetXPos() {return xPos;};
        float GetYPos() {return yPos;};
        bool GetRemove() {return remove;};
        bool SetRemove(bool r) {remove = r;};
        void render();
        virtual ~FireBall();
    protected:
    private:
        float xPos, yPos;
        float xSpeed, ySpeed;
        bool remove;
        Animation anim;
        GLuint transUniform, projUniform;
        GLuint vao, vbo, tbo;
        glm::vec3 translation, rotation, scale;
        glm::mat4 transMatrix;
        float rotationAmount;
        Texture texture;
        std::vector<std::vector<float>> texCoords;
        int texCoordsIndex;

        void SetupMesh(const char *filename);
};

#endif // FIREBALL_H
