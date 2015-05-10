#include "StartScreen.h"
#include "Input.h"
#include "JoyStick.h"
#include "TextureManager.h"
#include <iostream>

StartScreen::StartScreen()
{
    //ctor
}
void StartScreen::Init(int levelNum)
{
    shader.Setup("./res/shaders/vertexshader.vs", "./res/shaders/fragmentshader.fs");
    texture = TextureManager::LoadTexture("./res/textures/backgroundlevel0.png");

    projUniform = glGetUniformLocation(shader.program, "projMatrix");
    transUniform = glGetUniformLocation(shader.program, "transMatrix");

    tileMap.Setup(shader, "0");
    tileMap.SetHasKey(true);
    tileMap.SetRenderDoor(false);

    translation = glm::vec3(0.0, 0.0, 0.0f);
    rotation = glm::vec3(0.0, 0.0, 1.0);
    scale = glm::vec3(Game::windowWidth,Game::windowHeight,1.0);
    rotationAmount = 0;

    transMatrix = glm::translate(transMatrix, translation);
    transMatrix = glm::rotate(transMatrix, glm::radians(rotationAmount),  rotation);
    transMatrix = glm::scale(transMatrix, scale);

    GLfloat vertCoords[] =
    {
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 1.0, 0.0,
        0.0, 0.0, 0.0,
        1.0, 1.0, 0.0,
        0.0, 1.0, 0.0
    };
    GLfloat tex[] =
    {
        0, 0,
        1, 0,
        1, 1,

        0, 0,
        1, 1,
        0, 1
    };



    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertCoords), vertCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glBindVertexArray(0);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void StartScreen::Update(Game *game)
{
    if(JoyStick::isJoyStick || Input::getKey(Input::KEY_S))
    {
            game->ChangeState(1);
    }
    else
    {
        curTime = glfwGetTime();
        delta = curTime - lastTime;
        lastTime = curTime;
        tileMap.Update(delta);

        scale = glm::vec3(Game::windowWidth,Game::windowHeight,1.0);
        transMatrix = glm::mat4();
        transMatrix = glm::translate(transMatrix, translation);
        transMatrix = glm::rotate(transMatrix, glm::radians(rotationAmount),  rotation);
        transMatrix = glm::scale(transMatrix, scale);
    }
}

void StartScreen::Render()
{
    shader.bind();
    glm::mat4 projMatrix = glm::ortho(0.0f, (float)Game::windowWidth, (float)Game::windowHeight, 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(projUniform, 1, GL_FALSE, glm::value_ptr(projMatrix));
    glUniformMatrix4fv(transUniform, 1, GL_FALSE, glm::value_ptr(transMatrix));
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTexture());
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glDrawArrays(GL_TRIANGLES, 0, 2*3);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    int mapWidth = tileMap.GetMapWidth() * Game::tileSize;
    int offsetX = Game::windowWidth / 2 - Game::tileSize;
    offsetX = min(offsetX, 0);
    offsetX = max(offsetX, (Game::windowWidth/2 - mapWidth/2));

    int offsetY = Game::windowHeight - tileMap.GetMapHeight()*Game::tileSize;

    projMatrix = glm::ortho(0.0f-offsetX, (float)Game::windowWidth-offsetX, (float)Game::windowHeight-offsetY, 0.0f-offsetY, -1.0f, 1.0f);
    glUniformMatrix4fv(projUniform, 1, GL_FALSE, glm::value_ptr(projMatrix));
    tileMap.Render();
}

StartScreen::~StartScreen()
{

}
