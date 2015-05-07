#include "Yoyo.h"
#include "Game.h"
#include "Input.h"
#include "JoyStick.h"
#include "TextureManager.h"

Yoyo::Yoyo()
{

}

void Yoyo::Setup(const Shader &shader, float x, float y)
{
    texture = TextureManager::LoadTexture("./res/textures/yoyo.png");
    SetupMesh("./res/textures/yoyo.png");
    xPos = x;
    yPos = y;
    length = 0;
    maxLength = 128;
    minLength = 0;
    outSpeed = 3.0;
    isOut = false;
    returning = false;
    anim.Setup(1,4, 1, 0, .1);

    texCoordsIndex = 0;

    transUniform = glGetUniformLocation(shader.program, "transMatrix");
    translation = glm::vec3(xPos, yPos, 0.0f);
    rotation = glm::vec3(0.0, 0.0, 1.0);
    scale = glm::vec3(16,16,1.0);
    rotationAmount = 0;

    transMatrix = glm::translate(transMatrix, translation);
    transMatrix = glm::rotate(transMatrix, glm::radians(rotationAmount),  rotation);
    transMatrix = glm::scale(transMatrix, scale);
}

void Yoyo::Update(TileMap tileMap, float x, float y, int direction, double dt)
{
    if(!isOut)
    {
        texCoordsIndex = 0;
        if(JoyStick::isJoyStick)
        {
            if(JoyStick::GetButtons(JoyStick::buttonB))
            {
                isOut = true;
                maxLength *= direction;
            }

        }
        else
        {
            if(Input::getKey(Input::KEY_K))
            {
                isOut = true;
                maxLength *= direction;
            }
        }
        xPos = x;
    }
    else
    {
        anim.Update(dt);
        texCoordsIndex = anim.GetCurFrame();
        if(maxLength > 0)
        {
            if(JoyStick::isJoyStick)
            {
                if(JoyStick::GetButtons(JoyStick::buttonB))
                {
                       if(! returning && length < maxLength
                          && !tileMap.GetTileCollision((xPos + 16 + outSpeed)/64, (yPos+8)/64))
                       {
                            length += outSpeed;
                            xPos = x + length;
                       }
                       else
                       {
                           if(xPos < x)
                            {
                                isOut = false;
                                returning = false;
                                length = 0;
                                maxLength = 128;
                                maxX = tileMap.GetMapWidth()*64;
                                xPos = x;
                            }
                           else
                           {
                                maxX = xPos;
                                returning = true;
                                length -= outSpeed + 5;
                                xPos = min(maxX, x + length);
                           }
                       }
                }
                else
                {
                    if(xPos < x)
                    {
                        isOut = false;
                        returning = false;
                        length = 0;
                        maxLength = 128;
                        maxX = tileMap.GetMapWidth()*64;
                        xPos = x;
                    }
                   else
                   {
                        maxX = xPos;
                        returning = true;
                        length -= outSpeed + 5;
                        xPos = x + length;
                   }
                }
                if(length <= 0)
                {
                    isOut = false;
                    returning = false;
                    maxLength = 128;
                    maxX = tileMap.GetMapWidth()*64;
                    xPos = x;
                }

            }
            else
            {
                if(Input::getKey(Input::KEY_K))
                {
                       if(! returning && length < maxLength
                          && !tileMap.GetTileCollision((xPos + 16 + outSpeed)/64, (yPos+8)/64))
                       {
                            length += outSpeed;
                            xPos = x + length;
                       }
                       else
                       {
                           if(xPos < x)
                            {
                                isOut = false;
                                returning = false;
                                length = 0;
                                maxLength = 128;
                                maxX = tileMap.GetMapWidth()*64;
                                xPos = x;
                            }
                           else
                           {
                                maxX = xPos;
                                returning = true;
                                length -= outSpeed + 5;
                                xPos = min(maxX, x + length);
                           }
                       }
                }
                else
                {
                    if(xPos < x)
                    {
                        isOut = false;
                        returning = false;
                        length = 0;
                        maxLength = 128;
                        maxX = tileMap.GetMapWidth()*64;
                        xPos = x;
                    }
                   else
                   {
                        maxX = xPos;
                        returning = true;
                        length -= outSpeed + 5;
                        xPos = x + length;
                   }
                }
                if(length <= 0)
                {
                    isOut = false;
                    returning = false;
                    length = 0;
                    maxLength = 128;
                    maxX = tileMap.GetMapWidth()*64;
                    xPos = x;
                }
            }
        }
        else /// maxLength < 0
        {
            if(JoyStick::isJoyStick)
            {
                if(JoyStick::GetButtons(JoyStick::buttonB))
                {

                       if(! returning && length > maxLength
                          && !tileMap.GetTileCollision((xPos - outSpeed)/64, (yPos+8)/64))
                       {
                           length -= outSpeed;
                           xPos = x + length;
                       }
                       else
                       {
                           if(xPos > x)
                            minX = 0;
                           else
                            minX = xPos;
                           returning = true;
                           length += outSpeed;
                           xPos = max(minX, x + length);
                       }
                }
                else
                {
                    if(xPos > x)
                        minX = 0;
                    else
                        minX = xPos;
                    returning = true;
                    length += outSpeed;
                    xPos = max(minX, x + length);
                }
                if(length >= 0)
                {
                    isOut = false;
                    returning = false;
                    maxLength = 128;
                    minX = 0;
                    xPos = x;
                }
            }
            else
            {
                if(Input::getKey(Input::KEY_K))
                {

                       if(! returning && length > maxLength
                          && !tileMap.GetTileCollision((xPos - outSpeed)/64, (yPos+8)/64))
                       {
                           length -= outSpeed;
                           xPos = x + length;
                       }
                       else
                       {
                           if(xPos > x)
                            {
                                isOut = false;
                                returning = false;
                                length = 0;
                                maxLength = 128;
                                minX = 0;
                                xPos = x;
                            }
                           else
                           {
                                minX = xPos;
                                returning = true;
                                length += outSpeed + 5;
                                xPos = max(minX, x + length);
                           }
                       }
                }
                else
                {
                    if(xPos > x)
                    {
                        isOut = false;
                        returning = false;
                        length = 0;
                        maxLength = 128;
                        minX = 0;
                        xPos = x;
                    }
                   else
                   {
                        minX = xPos;
                        returning = true;
                        length += outSpeed + 5;
                        xPos = max(minX, x + length);
                   }
                }
                if(length >= 0)
                {
                    isOut = false;
                    returning = false;
                    maxLength = 128;
                    minX = 0;
                    xPos = x;
                }
            }
        }


    }

    yPos = y;

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
    translation = glm::vec3(xPos, yPos, 0.0f);
    transMatrix = glm::mat4();
    transMatrix = glm::translate(transMatrix, translation);
    transMatrix = glm::rotate(transMatrix, glm::radians(rotationAmount),  rotation);
    transMatrix = glm::scale(transMatrix, scale);

}

void Yoyo::render()
{
    glUniformMatrix4fv(transUniform, 1, GL_FALSE, glm::value_ptr(transMatrix));
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTexture());
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, 2*3);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

Yoyo::~Yoyo()
{
    //dtor
}

void Yoyo::SetupMesh(const char *filename)
{
    texCoords = texture->SetupTexCoords(16);

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
    tex[0] = texCoords[0][0];
    tex[1] = texCoords[0][1];
    tex[2] = texCoords[0][2];
    tex[3] = texCoords[0][3];
    tex[4] = texCoords[0][4];
    tex[5] = texCoords[0][5];

    tex[6] = texCoords[0][0];
    tex[7] = texCoords[0][1];
    tex[8] = texCoords[0][4];
    tex[9] = texCoords[0][5];
    tex[10] = texCoords[0][6];
    tex[11] = texCoords[0][7];



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
