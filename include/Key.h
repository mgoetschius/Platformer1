#ifndef KEY_HH
#define KEY_HH

#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vector"



class Key
{
    public:
        Key();
        void Setup(Shader &shader, int x, int y, int size);
        void render();
        virtual ~Key();
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

#endif // KEY_HH
