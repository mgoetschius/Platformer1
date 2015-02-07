#include "Level1.h"
#include <algorithm>

Texture Level1::texture;

Level1::Level1()
{
    //ctor
}

void Level1::Init()
{
    shader.Setup("res/shaders/vertexshader.vs", "res/shaders/fragmentshader.fs");
    Level1::texture.Setup("res/textures/level1.png");
    projUniform = glGetUniformLocation(shader.program, "projMatrix");

    background.Setup(shader, string("blueblock"));
    //background.SetScale(glm::vec3(.1,.13,.1));
    background.SetScale(glm::vec3(64,64,1));
    background.SetTranslation(glm::vec3(0,0,0));

    tileMap.Setup(shader);
    tileMap.Update();

    player.Setup(shader);
}

void Level1::Update(Game *game)
{
    //background.update();
    player.update(tileMap);
}

void Level1::Render()
{
    shader.bind();
    int mapWidth = tileMap.GetMapWidth() * Game::tileSize;
    int offsetX = Game::windowWidth / 2 - (int)(player.GetXPos()) - Game::tileSize;
    offsetX = min(offsetX, 0);
    offsetX = max(offsetX, (Game::windowWidth - mapWidth));

    int offsetY = Game::windowHeight - tileMap.GetMapHeight()*Game::tileSize;
    glm::mat4 projMatrix = glm::ortho(0.0f-offsetX, (float)Game::windowWidth-offsetX, (float)Game::windowHeight-offsetY, 0.0f-offsetY, -1.0f, 1.0f);
    glUniformMatrix4fv(projUniform, 1, GL_FALSE, glm::value_ptr(projMatrix));
    //background.render();
    tileMap.Render();
    player.render();
}

Level1::~Level1()
{
    //dtor
}
