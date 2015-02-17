#include "Player.h"
#include "Game.h"
#include "Input.h"

Player::Player()
{
    //ctor
}

void Player::Setup(const Shader &shader)
{
    xPos = 63;
    yPos = 8*Game::tileSize;
    xSpeed = 5;
    ySpeed = 0;
    gravity = .2;
    jumping = true;
    texCoordsIndex = 1;
    rightAnim.Setup(8, 11, 8, 0, .8);
    leftAnim.Setup(4, 7, 4);
    transUniform = glGetUniformLocation(shader.program, "transMatrix");
    translation = glm::vec3(xPos, yPos, 0.0f);
    rotation = glm::vec3(0.0, 0.0, 1.0);
    scale = glm::vec3(Game::tileSize,Game::tileSize,1.0);
    rotationAmount = 0;

    transMatrix = glm::translate(transMatrix, translation);
    transMatrix = glm::rotate(transMatrix, glm::radians(rotationAmount),  rotation);
    transMatrix = glm::scale(transMatrix, scale);

    SetupMesh();
}

void Player::update(Game *game, TileMap &tileMap, double dt)
{
    //std::cout << "x: " << (int)(xPos) / 64 << "y: " << (int)(yPos+64)/64 << std::endl;

    glm::vec2 topLeft(xPos+1, (yPos+1));
    glm::vec2 topRight((xPos+Game::tileSize-1), (yPos+1));
    glm::vec2 bottomLeft(xPos+1, (yPos+Game::tileSize-1));
    glm::vec2 bottomRight((xPos+Game::tileSize-1), (yPos+Game::tileSize-1));
    if(Input::getKey(Input::KEY_D))
    {
        direction = 1;
        glm::vec2 tr((topRight.x+xSpeed)/64, topRight.y/Game::tileSize);
        glm::vec2 br((bottomRight.x+xSpeed)/64,  (bottomRight.y)/Game::tileSize);
        Move(tileMap, tr, br, direction);
        rightAnim.Update(dt);
        texCoordsIndex = rightAnim.GetCurFrame();
    }

    else if(Input::getKey(Input::KEY_A))
    {
        direction = -1;
        glm::vec2 tl((topLeft.x-xSpeed)/64, topLeft.y/Game::tileSize);
        glm::vec2 bl((topLeft.x-xSpeed)/64,  (bottomLeft.y)/Game::tileSize);
        Move(tileMap, tl, bl, direction);
        leftAnim.Update(dt);
        texCoordsIndex = leftAnim.GetCurFrame();
    }
    else
    {
        if(direction > 0)
            texCoordsIndex = 1;
        else
            texCoordsIndex = 0;
    }
    if(Input::getKey(Input::KEY_W) && jumping == false)
    {
        ySpeed = -7.5;
        jumping = true;
        onLadder = false;
    }
    if(Input::getKey(Input::KEY_S))
    {
        if(tileMap.GetLadderCollision((int)(xPos+Game::tileSize/2)/Game::tileSize, (yPos) / Game::tileSize))
        {
            onLadder = true;
            if(!tileMap.GetTileCollision((int)(topRight.x)/Game::tileSize, (int) ((topRight.y) / Game::tileSize))
               && !tileMap.GetTileCollision((int)(topLeft.x)/Game::tileSize, (int) ((topLeft.y) / Game::tileSize)))
               {
                    yPos-=3;
               }
        }
    }
    if(!doorIsOpen
       && tileMap.GetKeyCollision((int)((xPos+Game::tileSize/2)/Game::tileSize), (int) ((yPos+Game::tileSize/2) / Game::tileSize)))
        {
            Game::audioPlayer.play(1);
            tileMap.SetHasKey(true);
            tileMap.UpdateDoor();
            doorIsOpen = true;
        }
    if(doorIsOpen
       && tileMap.GetDoorCollision((int)((xPos+Game::tileSize/2)/Game::tileSize), (int) ((yPos+Game::tileSize/2) / Game::tileSize)))
    {
        std::cout << "here\n";
        levelOver = true;
    }

    /// Gravity

        if(onLadder)
        {
            jumping = false;
            ySpeed = 0;
            if(!tileMap.GetLadderCollision((int)((xPos+Game::tileSize/2)/Game::tileSize), (int) ((yPos+Game::tileSize) / Game::tileSize)))
            {
                jumping = true;
                onLadder = false;
            }
        }
        else if(tileMap.GetTileCollision((int) ((topLeft.x) / Game::tileSize), (int) ((topLeft.y+ySpeed + gravity) / Game::tileSize))
           || tileMap.GetTileCollision((int) ((topRight.x) / Game::tileSize), (int) ((topRight.y+ySpeed + gravity) / Game::tileSize))
                )
        {
            jumping = true;
            ySpeed = 0;
        }
        else if(!tileMap.GetTileCollision((int) ((bottomLeft.x) / Game::tileSize), (int) ((bottomLeft.y+ySpeed + gravity) / Game::tileSize))
           && !tileMap.GetTileCollision((int) ((bottomRight.x) / Game::tileSize), (int) ((bottomRight.y+ySpeed + gravity) / Game::tileSize))
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

    if(jumping)
    {
        if(direction > 0)
            texCoordsIndex = 1;
        else
            texCoordsIndex = 0;
    }

    /// check collision with enemy

    std::vector<Enemy*> *enemies = tileMap.GetEnemies();
    for(std::vector<Enemy*>::iterator i = enemies->begin(); i != enemies->end(); i++ )
    {
        if(xPos + Game::tileSize > (*i)->GetXPos()
           && (*i)->GetXPos() + Game::tileSize > xPos
           && yPos + Game::tileSize > (*i)->GetYPos()
           && (*i)->GetYPos() + Game::tileSize > yPos
           && !(*i)->GetIsDead())
        {
            if(yPos + Game::tileSize/2 < (*i)->GetYPos())
            {
                std::cout << "kill\n";
                ySpeed = -5;
                (*i)->SetIsDead(true);

            }
            else
            {
                std::cout << "dead\n";
                isDead = true;
            }
        }
    }
    translation = glm::vec3(xPos, yPos, 0.0f);
    transMatrix = glm::mat4();
    transMatrix = glm::translate(transMatrix, translation);
    transMatrix = glm::rotate(transMatrix, glm::radians(0.0f),  rotation);
    transMatrix = glm::scale(transMatrix, scale);

    /// change player texCoords

    GLfloat tex[12];
    tex[0] = texCoords[texCoordsIndex][0];
    tex[1] = texCoords[texCoordsIndex][1];
    tex[2] = texCoords[texCoordsIndex][2];
    tex[3] = texCoords[texCoordsIndex][3];
    tex[4] = texCoords[texCoordsIndex][4];
    tex[5] = texCoords[texCoordsIndex][5];

    tex[6] = texCoords[texCoordsIndex][0];
    tex[7] = texCoords[texCoordsIndex][1];
    tex[8] = texCoords[texCoordsIndex][4];
    tex[9] = texCoords[texCoordsIndex][5];
    tex[10] = texCoords[texCoordsIndex][6];
    tex[11] = texCoords[texCoordsIndex][7];

    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex), tex);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Player::render()
{
    glUniformMatrix4fv(transUniform, 1, GL_FALSE, glm::value_ptr(transMatrix));
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.GetTexture());
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, 2*3);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

void Player::Move(TileMap tileMap, glm::vec2 top, glm::vec2 bottom, int direction)
{
   if(!tileMap.GetTileCollision(top.x, top.y)
      && !tileMap.GetTileCollision(bottom.x, bottom.y))
   {
       xPos += xSpeed * direction;
   }
}

Player::~Player()
{
    //dtor
}

void Player::SetupMesh()
{
    texture.Setup("res/textures/playeranim.png");
    texCoords = texture.SetupTexCoords(Game::tileSize);

    GLfloat vertCoords[] =
    {
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 1.0, 0.0,
        0.0, 0.0, 0.0,
        1.0, 1.0, 0.0,
        0.0, 1.0, 0.0
    };
    GLfloat tex[12];
    tex[0] = texCoords[1][0];
    tex[1] = texCoords[1][1];
    tex[2] = texCoords[1][2];
    tex[3] = texCoords[1][3];
    tex[4] = texCoords[1][4];
    tex[5] = texCoords[1][5];

    tex[6] = texCoords[1][0];
    tex[7] = texCoords[1][1];
    tex[8] = texCoords[1][4];
    tex[9] = texCoords[1][5];
    tex[10] = texCoords[1][6];
    tex[11] = texCoords[1][7];



    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertCoords), vertCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STREAM_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glBindVertexArray(0);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}
