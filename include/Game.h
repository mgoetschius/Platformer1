#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "GameState.h"
#include "Level1.h"
#include "AudioPlayer.h"

enum GameStates
{
    NO_STATE,
    INTRO,
    PLAY
};


class Game
{
    public:
        Game();
        void Init();
        void Update();
        void Render();
        void ChangeState(int state); // make some sort of enum type thing
        virtual ~Game();
        static int windowWidth;
        static int windowHeight;
        static AudioPlayer audioPlayer;
        const static int tileSize;
    protected:
    private:
        bool isRunning;
        GameState *gameState;
};

#endif // GAME_H
