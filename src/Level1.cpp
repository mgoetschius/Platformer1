#include "Level1.h"
#include <algorithm>
#include <GL/glfw3.h>

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

    tileMap.Setup(shader);

    background.Setup(shader, string("res/textures/background.png"));
    background.SetScale(glm::vec3(1.0f, tileMap.GetMapHeight() * Game::tileSize, 1.0f));
    std::cout << tileMap.GetMapHeight() << std::endl;
    player.Setup(shader);
}

void Level1::Update(Game *game)
{
    curTime = glfwGetTime();
    delta = curTime - lastTime;
    lastTime = curTime;
    background.update();
    player.update(tileMap, delta);
    tileMap.Update(delta);
}

void Level1::Render()
{
    shader.bind();
    int mapWidth = tileMap.GetMapWidth() * Game::tileSize;
    int offsetX = Game::windowWidth / 2 - (int)(player.GetXPos()) - Game::tileSize;
    offsetX = min(offsetX, 0);
    offsetX = max(offsetX, (Game::windowWidth - mapWidth));

    int offsetY = Game::windowHeight - tileMap.GetMapHeight()*Game::tileSize;
    float temp = (float)(Game::windowWidth - mapWidth) / (mapWidth - background.GetWidth());
    int backgroundX = offsetX/temp;
    background.SetTranslation(glm::vec3((float)backgroundX, 0.0f, 0.0f));
    background.update();
    background.render();

    glm::mat4 projMatrix = glm::ortho(0.0f-offsetX, (float)Game::windowWidth-offsetX, (float)Game::windowHeight-offsetY, 0.0f-offsetY, -1.0f, 1.0f);
    glUniformMatrix4fv(projUniform, 1, GL_FALSE, glm::value_ptr(projMatrix));
    tileMap.Render();
    player.render();
}

Level1::~Level1()
{
    //dtor
}
