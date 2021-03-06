#include "PurpleEnemy.h"
#include "Game.h"
#include "TextureManager.h"

PurpleEnemy::PurpleEnemy(Shader &shader, const char *filename, float x, float y)
{
    texture = TextureManager::LoadTexture(filename);
    SetupMesh(filename);
    xPos = x;
    yPos = y;
    xSpeed = -2;
    ySpeed = 0;
    gravity = .2f;
    isDead = false;
    remove = false;

    texCoordsIndex = 0;
    rightAnim.Setup(2, 3, 2);
    leftAnim.Setup(0, 1, 0);

    transUniform = glGetUniformLocation(shader.program, "transMatrix");
    translation = glm::vec3(xPos, yPos, 0.0f);
    rotation = glm::vec3(0.0, 0.0, 1.0);
    scale = glm::vec3(Game::tileSize,Game::tileSize,1.0);
    rotationAmount = 0;

    transMatrix = glm::translate(transMatrix, translation);
    transMatrix = glm::rotate(transMatrix, glm::radians(rotationAmount),  rotation);
    transMatrix = glm::scale(transMatrix, scale);
}

void PurpleEnemy::Update(TileMap *tileMap, double dt)
{
    if(!isDead)
    {
        xPos += xSpeed;

        if (xSpeed < 0)
        {
            leftAnim.Update(dt);
            texCoordsIndex = leftAnim.GetCurFrame();

           if(xPos + xSpeed < 0)
           {
                xSpeed *= -1;
                direction = -1;
           }
           if(tileMap->GetTileCollision((int) ((xPos+xSpeed) / Game::tileSize), (int) ((yPos-1+Game::tileSize) / Game::tileSize))
              || tileMap->GetTileCollision((int) ((xPos+xSpeed) / Game::tileSize), (int) ((yPos+1) / Game::tileSize))
              || !tileMap->GetTileCollision((int)(xPos+xSpeed)/Game::tileSize, (int) (yPos+1+Game::tileSize) / Game::tileSize))
            {
                xSpeed *= -1;
                direction = -1;
            }
        }

        else
        {
            rightAnim.Update(dt);
            texCoordsIndex = rightAnim.GetCurFrame();

            if (xPos + xSpeed > tileMap->GetMapWidth()*Game::tileSize)
            {
                xSpeed *= -1;
                direction = 1;
            }
            if(tileMap->GetTileCollision((int) ((xPos+Game::tileSize+xSpeed) / Game::tileSize), (int) ((yPos-1+Game::tileSize) / Game::tileSize))
               || tileMap->GetTileCollision((int) ((xPos+Game::tileSize+xSpeed) / Game::tileSize), (int) ((yPos+1) / Game::tileSize))
               || !tileMap->GetTileCollision((int)(xPos+Game::tileSize+xSpeed)/Game::tileSize, (int) (yPos+1+Game::tileSize) / Game::tileSize))
            {
                xSpeed *= -1;
                direction = 1;
            }
        }

        /// Gravity

            if(tileMap->GetTileCollision((int) ((xPos) / Game::tileSize), (int) ((yPos+ySpeed + gravity) / Game::tileSize))
                    || tileMap->GetTileCollision((int) ((xPos+Game::tileSize) / Game::tileSize), (int) ((yPos+ySpeed + gravity) / Game::tileSize))
                    )
            {
                jumping = true;
                ySpeed = 0;
            }
            if(!tileMap->GetTileCollision((int) ((xPos) / Game::tileSize), (int) ((yPos+Game::tileSize+ySpeed + gravity) / Game::tileSize))
               && !tileMap->GetTileCollision((int) ((xPos+Game::tileSize) / Game::tileSize), (int) ((yPos+Game::tileSize+ySpeed + gravity) / Game::tileSize))
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

        /// update animation

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
    } // end if(!isDead)

    else // isDead
    {
        xSpeed = 1;
        ySpeed = 1.5;
        xPos+= xSpeed;
        yPos+= ySpeed;
        rotationAmount+= -1;
    }

    translation = glm::vec3(xPos, yPos, 0.0f);
    transMatrix = glm::mat4();
    transMatrix = glm::translate(transMatrix, translation);
    transMatrix = glm::rotate(transMatrix, glm::radians(rotationAmount),  rotation);
    transMatrix = glm::scale(transMatrix, scale);
}

PurpleEnemy::~PurpleEnemy()
{
    //dtor
}
