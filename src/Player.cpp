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

void Player::update(TileMap &tileMap, double dt)
{
    if(Input::getKey(Input::KEY_D))
    {
        direction = 1;
        Move(tileMap, direction);
        rightAnim.Update(dt);
        texCoordsIndex = rightAnim.GetCurFrame();
    }

    else if(Input::getKey(Input::KEY_A))
    {
        direction = -1;
        Move(tileMap, direction);
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

    if(jumping)
    {
        if(direction > 0)
            texCoordsIndex = 1;
        else
            texCoordsIndex = 0;
    }

    /// check collision with enemy

    std::vector<Enemy> *enemies = tileMap.GetEnemies();
    for(std::vector<Enemy>::iterator i = enemies->begin(); i != enemies->end(); i++ )
    {
        if(xPos + Game::tileSize > (*i).GetXPos()
           && (*i).GetXPos() > xPos
           && yPos + Game::tileSize > (*i).GetYPos()
           && (*i).GetYPos() + Game::tileSize > yPos)
        {
            std::cout << "hit\n";
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



void Player::Move(TileMap &tileMap, int dx)
{
    float speed = xSpeed * dx;
    if(speed > 0)
    {
        if  (!tileMap.GetTileCollision((int) ((xPos+Game::tileSize+speed) / Game::tileSize), (int) (yPos / Game::tileSize))
            && !tileMap.GetTileCollision((int) ((xPos+Game::tileSize+speed) / Game::tileSize), (int) ((yPos + Game::tileSize) / Game::tileSize)))
            {
                xPos += speed;
            }
        else if  (!tileMap.GetTileCollision((int) ((xPos+Game::tileSize+1) / Game::tileSize), (int) (yPos / Game::tileSize))
            && !tileMap.GetTileCollision((int) ((xPos+Game::tileSize+1) / Game::tileSize), (int) ((yPos + Game::tileSize) / Game::tileSize)))
            {
                xPos += 1;
            }
    }
    else
    {
        if(!tileMap.GetTileCollision((int) ((xPos+speed) / Game::tileSize), (yPos / Game::tileSize))
           && !tileMap.GetTileCollision((int) ((xPos+speed) / Game::tileSize), ((yPos+Game::tileSize) / Game::tileSize)))
           {
                xPos += speed;
           }
        else if(!tileMap.GetTileCollision((int) ((xPos-1) / Game::tileSize), (yPos / Game::tileSize))
           && !tileMap.GetTileCollision((int) ((xPos-1) / Game::tileSize), ((yPos+Game::tileSize) / Game::tileSize)))
           {
                xPos -= 1;
           }
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
