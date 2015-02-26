#include "Game.h"

int Game::windowWidth = 720;
int Game::windowHeight = 680;
const int Game::tileSize = 64;
AudioPlayer Game::audioPlayer;

Game::Game()
{
    //ctor
}

void Game::Init()
{
    std::cout << "Initializing Game\n";
    gameState = new Level1();
    gameState->Init(1);
}

void Game::Update()
{
    gameState->Update(this);
}

void Game::Render()
{
    gameState->Render();
}


void Game::ChangeState(int state)
{
    delete gameState;
    gameState = new Level1();
    gameState->Init(state);
}

Game::~Game()
{
    delete gameState;
}
