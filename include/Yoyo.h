#ifndef YOYO_H
#define YOYO_H
#include "Texture.h"
#include "Shader.h"
#include "TileMap.h"
#include "Animation.h"
#include <vector>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Yoyo
{
    public:
        Yoyo();
        void Setup(const Shader &shader, float x, float y);
        void Update(TileMap tileMap, float x, float y, int direction, double dt);
        void render();
        float GetXPos() {return xPos;};
        float GetYPos() {return yPos;};
        void SetReturning(bool value) {returning = value;};
        virtual ~Yoyo();
    protected:
    private:
        void SetupMesh(const char *filename);
        float xPos, yPos;
        float length, maxLength, minLength;
        float outSpeed;
        bool isOut;
        bool returning;
        Animation anim;
        GLuint transUniform, projUniform;
        GLuint vao, vbo, tbo;
        glm::vec3 translation, rotation, scale;
        glm::mat4 transMatrix;
        float rotationAmount;
        Texture *texture;
        std::vector<std::vector<float>> texCoords;
        int texCoordsIndex;
};

#endif // YOYO_H
