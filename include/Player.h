#ifndef PLAYER_H
#define PLAYER_H

#include "Texture.h"
#include "TextureChangeMesh.h"
#include "Shader.h"
#include "Animation.h"
#include "TileMap.h"
#include "Yoyo.h"
#include "FireBall.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Game;

class Player
{
    public:
        Player();
        void Setup(const Shader&);
        void update(Game *game, TileMap&, double dt);
        void render();
        float GetXPos() {return xPos;};
        float GetYPos() {return yPos;};
        bool GetIsDead() {return isDead;};
        bool GetLevelOver() {return levelOver;};
        virtual ~Player();
    protected:
    private:
        float xPos, yPos, xSpeed, ySpeed;
        glm::vec2 topLeft, topRight, bottomLeft, bottomRight;
        int direction;
        float gravity;
        bool jumping = false;
        bool onLadder = false;
        bool isDead = false;
        bool doorIsOpen = false;
        bool levelOver = false;
        GLuint transUniform, projUniform;
        GLuint vao, vbo, tbo, ibo;
        std::vector<GLuint> indices;
        Shader s;
        Texture *texture;
        std::vector<std::vector<float>> texCoords;
        int texCoordsIndex;
        glm::vec3 translation, rotation, scale;
        glm::mat4 transMatrix;
        float rotationAmount;
        void SetupMesh();
        void Move(TileMap &tileMap, glm::vec2 top, glm::vec2 bottom, int direction);
        void MoveY(TileMap &tileMap);
        Animation rightAnim;
        Animation leftAnim;
        void UpdateAnimation(Animation &anim, double dt);
        Yoyo yoyo;
        bool useYoYo;
        std::vector<FireBall*> fireBalls;
        bool useFireBalls;
};

#endif // PLAYER_H
