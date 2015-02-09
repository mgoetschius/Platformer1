#include "Player.h"
#include "Game.h"
#include "Input.h"

Player::Player()
{
    //ctor
}

void Player::Setup(const Shader &shader)
{
    xPos = 128;
    yPos = 7*Game::tileSize;
    xSpeed = 5;
    ySpeed = 0;
    gravity = .1;
    jumping = true;
    transUniform = glGetUniformLocation(shader.program, "transMatrix");
    translation = glm::vec3(xPos, yPos, 0.0f);
    rotation = glm::vec3(0.0, 0.0, 1.0);
    scale = glm::vec3(Game::tileSize,Game::tileSize,1.0);
    rotationAmount = 0;

    transMatrix = glm::translate(transMatrix, translation);
    transMatrix = glm::rotate(transMatrix, glm::radians(rotationAmount),  rotation);
    transMatrix = glm::scale(transMatrix, scale);

    SetupMesh();

    //mesh.Setup(vertCoords, texCoords, ind, texture);

}

void Player::update(TileMap &tileMap)
{
    //int playerTileX = (int) (xPos / Game::tileSize);
    //int playerTileY = (int) (yPos / Game::tileSize);
    //std::cout << playerTileX << "," << playerTileY << std::endl;
    if(Input::getKey(Input::KEY_D))
        if(!tileMap.GetTileCollision((int) ((xPos+Game::tileSize+xSpeed) / Game::tileSize), (int) (yPos / Game::tileSize))
           && !tileMap.GetTileCollision((int) ((xPos+Game::tileSize+xSpeed) / Game::tileSize), (int) ((yPos + Game::tileSize) / Game::tileSize)))
            xPos += xSpeed;
    if(Input::getKey(Input::KEY_A))
        if(!tileMap.GetTileCollision((int) ((xPos-xSpeed) / Game::tileSize), (yPos / Game::tileSize))
           && !tileMap.GetTileCollision((int) ((xPos-xSpeed) / Game::tileSize), ((yPos+Game::tileSize) / Game::tileSize)))
            xPos -= xSpeed;
    if(Input::getKey(Input::KEY_W) && jumping == false)
    {
        ySpeed = -7;
        jumping = true;
        if(!tileMap.GetTileCollision((int) ((xPos) / Game::tileSize), (int) ((yPos-ySpeed) / Game::tileSize))
           && !tileMap.GetTileCollision((int) ((xPos+Game::tileSize) / Game::tileSize), (int) ((yPos-ySpeed) / Game::tileSize)))
            yPos += ySpeed;
    }

    /// Gravity

        if(tileMap.GetTileCollision((int) ((xPos) / Game::tileSize), (int) ((yPos+ySpeed + gravity) / Game::tileSize))
                || tileMap.GetTileCollision((int) ((xPos+Game::tileSize) / Game::tileSize), (int) ((yPos+ySpeed + gravity) / Game::tileSize))
                )
        {
            jumping = true;
            ySpeed = 0;
        }
        if(!tileMap.GetTileCollision((int) ((xPos) / Game::tileSize), (int) ((yPos+Game::tileSize+ySpeed + gravity) / Game::tileSize))
           && !tileMap.GetTileCollision((int) ((xPos+Game::tileSize) / Game::tileSize), (int) ((yPos+Game::tileSize+ySpeed + gravity) / Game::tileSize))
           )
        {
            jumping = true;
            ySpeed += gravity;
            yPos += ySpeed;
        }


        else
        {
            jumping = false;
            ySpeed = 0;
        }


    translation = glm::vec3(xPos, yPos, 0.0f);
    transMatrix = glm::mat4();
    transMatrix = glm::translate(transMatrix, translation);
    transMatrix = glm::rotate(transMatrix, glm::radians(0.0f),  rotation);
    transMatrix = glm::scale(transMatrix, scale);
    transMatrix = transMatrix;
}

void Player::render()
{
    glUniformMatrix4fv(transUniform, 1, GL_FALSE, glm::value_ptr(transMatrix));
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.GetTexture());
    glDrawArrays(GL_TRIANGLES, 0, 2*3);
    //mesh.render();
}

Player::~Player()
{
    //dtor
}

void Player::SetupMesh()
{
    GLfloat vertCoords[] =
    {
        1.0, 1.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0
    };
    GLfloat texCoords[] =
    {
        0.5, 0.5,
        0.5, 0.0,
        0.0, 0.5,
        0.5, 0.0,
        0.0, 0.0,
        0.0, 0.5
    };

    texture.Setup("res/textures/player.png");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertCoords), vertCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glBindVertexArray(0);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}
