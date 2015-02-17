#ifndef LEVEL1_H
#define LEVEL1_H

#include "GameState.h"
#include "Game.h"
#include "Shader.h"
#include "Model.h"
#include "TileMap.h"
#include "Texture.h"
#include "Player.h"
#include "Background.h"

class Level1 : public GameState
{
    public:
        Level1();
        void Init(int levelNum);
        void Update(Game *game);
        void Render();
        virtual ~Level1();
        static Texture texture;
    protected:
    private:
        Shader shader;
        GLuint projUniform;
        Background background;
        TileMap tileMap;
        Player player;
        double delta, curTime, lastTime;
        int level;
};

#endif // LEVEL1_H
