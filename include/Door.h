#ifndef DOOR_H
#define DOOR_H

#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vector"

class Door
{
    public:
        Door();
        void Setup(Shader &shader, int x, int y, int size);
        void update();
        void render();
        virtual ~Door();
    protected:
    private:
        void SetupMesh(const char *filename, int size);
        int xPos, yPos;
        GLuint transUniform;
        GLuint vao, vbo, tbo;
        glm::vec3 translation, rotation, scale;
        glm::mat4 transMatrix;
        float rotationAmount;
        Texture *texture;
        std::vector<std::vector<float>> texCoords;
};

#endif // DOOR_H
