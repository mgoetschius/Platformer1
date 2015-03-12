#include "Level1.h"
#include <algorithm>
#include <GL/glfw3.h>
#include <string>
#include <sstream>
#include "Input.h"

Texture Level1::texture;

Level1::Level1()
{
    //ctor
}

void Level1::Init(int levelNum)
{
    level = levelNum;
    stringstream ss;
    ss << levelNum;

    string s = "./res/textures/level" + ss.str() + ".png";
    shader.Setup("./res/shaders/vertexshader.vs", "./res/shaders/fragmentshader.fs");
    Level1::texture.Setup(s.c_str());
    projUniform = glGetUniformLocation(shader.program, "projMatrix");

    tileMap.Setup(shader, ss.str());

    s = "./res/textures/backgroundlevel" + ss.str() + ".png";
    background.Setup(shader, s);
    background.SetScale(glm::vec3(1.0f, tileMap.GetMapHeight() * Game::tileSize, 1.0f));
    player.Setup(shader);
}

void Level1::Update(Game *game)
{
    if(player.GetIsDead())
    {
        game->ChangeState(level);

    }
    else if (player.GetLevelOver())
    {
        if(level == 1)
        {
            game->ChangeState(2);
        }
        else
        {
            game->ChangeState(1);
        }
    }
    else
    {
        curTime = glfwGetTime();
        delta = curTime - lastTime;
        lastTime = curTime;
        background.update();
        player.update(game, tileMap, delta);
        tileMap.Update(delta);
    }
}

void Level1::Render()
{
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
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
    tileMap.CleanUp();
}
