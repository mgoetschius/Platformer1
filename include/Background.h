#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Texture.h"
#include "Shader.h"
#include <string>
#include "Mesh.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Background
{
    public:
        Background();
        void update();
        void render();
        void SetScale(glm::vec3 s) {scale = s;};
        void SetTranslation(glm::vec3 trans) {translation = trans;};
        void Setup(Shader shader, const std::string &filepath);
        int GetWidth() {return texture.GetWidth();};
        virtual ~Background();
    protected:
    private:
        Texture texture;
        Mesh mesh;
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 transMatrix;
        GLuint transUniform;
        float rotationAmount;
};

#endif // BACKGROUND_H
