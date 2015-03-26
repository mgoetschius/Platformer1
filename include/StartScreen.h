#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "GameState.h"
#include "Game.h"
#include "Shader.h"
#include "Model.h"
#include "TileMap.h"
#include "Texture.h"
#include "Background.h"



class StartScreen : public GameState
{
    public:
        StartScreen();
        void Init(int levelNum);
        void Update(Game *game);
        void Render();
        virtual ~StartScreen();
    protected:
    private:
        Shader shader;
        GLuint transUniform, projUniform;
        GLuint vao, vbo, tbo;
        glm::vec3 translation, rotation, scale;
        glm::mat4 transMatrix;
        float rotationAmount;
        Texture texture;
        std::vector<std::vector<float>> texCoords;
};

#endif // STARTSCREEN_H
