#include "Player.h"
#include "Game.h"
#include "Input.h"
#include "JoyStick.h"
#include <math.h>
#include "TextureManager.h"

Player::Player()
{
    //ctor
}

void Player::Setup(const Shader &shader)
{
    s = shader;
    xPos = 63;
    yPos = 8*Game::tileSize;
    xSpeed = 5;
    ySpeed = 0;
    gravity = .2;
    jumping = true;
    direction = 1;
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

    texture = TextureManager::LoadTexture("./res/textures/playeranim.png");
    SetupMesh();

    useFireBalls = false;
    yoyo.Setup(shader, xPos, yPos);
    useYoYo = true;
}

void Player::update(Game *game, TileMap &tileMap, double dt)
{
    topLeft = glm::vec2(xPos+1, (yPos+1));
    topRight = glm::vec2((xPos+Game::tileSize-1), (yPos+1));
    bottomLeft = glm::vec2(xPos+1, (yPos+Game::tileSize-1));
    bottomRight = glm::vec2((xPos+Game::tileSize-1), (yPos+Game::tileSize-1));

    if(JoyStick::isJoyStick)
    {
        if(JoyStick::GetAxes(JoyStick::xAxis) == 1)
        {
            direction = 1;
            glm::vec2 tr((topRight.x+xSpeed)/64, topRight.y/Game::tileSize);
            glm::vec2 br((bottomRight.x+xSpeed)/64,  (bottomRight.y)/Game::tileSize);
            Move(tileMap, tr, br, direction);
            rightAnim.Update(dt);
            texCoordsIndex = rightAnim.GetCurFrame();
        }
        else if(JoyStick::GetAxes(JoyStick::xAxis) == -1)
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
        if(JoyStick::GetButtonsOnce(JoyStick::buttonA) && jumping == false)
        {
            ySpeed = -7.5;
            jumping = true;
            onLadder = false;
        }
        if(JoyStick::GetAxes(JoyStick::yAxis) == -1)
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
        if(JoyStick::GetButtonsOnce(JoyStick::buttonB) && useFireBalls)
        {
            if(fireBalls.size() < 5)
                fireBalls.push_back(new FireBall(s, xPos + Game::tileSize/32, yPos + Game::tileSize/2 - 8, direction));
        }

    }
    else
    {
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
        if(Input::getKey(Input::KEY_J) && jumping == false)
        {
            ySpeed = -7.5;
            jumping = true;
            onLadder = false;
        }
        if(Input::getKey(Input::KEY_W))
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
        if(Input::getKeyOnce(Input::KEY_K)  && useFireBalls)
        {
            if(fireBalls.size() < 5)
                fireBalls.push_back(new FireBall(s, xPos + Game::tileSize/32, yPos + Game::tileSize/2 - 8, direction));
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
        std::cout << "here";
        levelOver = true;
    }

    /// Gravity

    MoveY(tileMap);

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
                //std::cout << "kill\n";
                Game::audioPlayer.play(0);
                ySpeed = -5;
                (*i)->SetIsDead(true);

            }
            else
            {
                std::cout << "dead\n";
                isDead = true;
            }
        }
        //check yoyo collisions on each enemy
        if(useYoYo)
        {
            if(yoyo.GetXPos() + 16 > (*i)->GetXPos()
               && (*i)->GetXPos() + Game::tileSize > yoyo.GetXPos()
               && yoyo.GetYPos() + 16 > (*i)->GetYPos()
               && (*i)->GetYPos() + Game::tileSize > yoyo.GetYPos()
               && !(*i)->GetIsDead())
            {
                Game::audioPlayer.play(0);
                (*i)->SetIsDead(true);
                yoyo.SetReturning(true);
            }
        }


        /// check fireball collisions
        for(std::vector<FireBall*>::iterator j = fireBalls.begin(); j != fireBalls.end(); j++)
        {
            if((*j)->GetXPos() + 16 > (*i)->GetXPos()
           && (*i)->GetXPos() + Game::tileSize > (*j)->GetXPos()
           && (*j)->GetYPos() + 16 > (*i)->GetYPos()
           && (*i)->GetYPos() + Game::tileSize > (*j)->GetYPos()
           && !(*i)->GetIsDead())
        {
            Game::audioPlayer.play(0);
            (*i)->SetIsDead(true);
            (*j)->SetRemove(true);
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

    if(useYoYo)
        yoyo.Update(tileMap, xPos + Game::tileSize/2, yPos + Game::tileSize/2, direction, dt);
    //fireBall.Update(tileMap, dt);
    for(std::vector<FireBall*>::iterator i = fireBalls.begin(); i != fireBalls.end(); i++)
        (*i)->Update(tileMap, dt, xPos);

    for(std::vector<FireBall*>::iterator i = fireBalls.begin(); i != fireBalls.end();)
    {
        if((*i)->GetRemove())
        {
            delete *i;
            i = fireBalls.erase(i);
        }
        else
        {
            i++;
        }
    }

}

void Player::render()
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
    if(useYoYo)
        yoyo.render();
    //fireBall.render();
    for(std::vector<FireBall*>::iterator i = fireBalls.begin(); i != fireBalls.end(); i++)
        (*i)->render();

}

void Player::Move(TileMap &tileMap, glm::vec2 top, glm::vec2 bottom, int direction)
{
    int t = tileMap.GetTileInt(top.x, top.y);
    int b = tileMap.GetTileInt(bottom.x, bottom.y);

    if((t == 0 || t >= 33)
       &&(b == 0 || b >= 33))
    {
        xPos += xSpeed * direction;
    }
   /*
   if(!tileMap.GetTileCollision(top.x, top.y)
      && !tileMap.GetTileCollision(bottom.x, bottom.y))
   {
       xPos += xSpeed * direction;
   }
   */

}

void Player::MoveY(TileMap &tileMap)
{
        //std::cout << (int)xPos % 64 << std::endl;
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
	///**********************************************************************************************************************
    else if (tileMap.GetTileInt((bottomRight.x-31) / Game::tileSize, bottomRight.y / Game::tileSize) == 33)
    {
        if(jumping)
        {
            if (bottomRight.y + ySpeed + gravity < ((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * ((int)(bottomRight.x-31) % 64))
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
        }
        else
        {
            yPos = ((int)(bottomRight.y) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * ((int)(bottomRight.x-31) % 64);
            jumping = false;
        }
    }
    else if (tileMap.GetTileInt((bottomRight.x-31) / Game::tileSize, (bottomRight.y + ySpeed + gravity) / Game::tileSize) == 33)
    {
        if(jumping)
        {
            if (yPos + ySpeed + gravity < ((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * ((int)(bottomRight.x-31) % 64))
            {
                jumping = true;
				ySpeed += gravity;
				yPos += ySpeed;
			}
			else
			{
				jumping = false;
				ySpeed = 0;
				yPos = ((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * ((int)(bottomRight.x-31) % 64);
			}
        }
        else
        {
            yPos = ((int)(bottomRight.y) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * ((int)(bottomRight.x-31) % 64);
            jumping = false;
        }
    }
    ///***********************************************************************************************************
    else if (tileMap.GetTileInt((bottomRight.x-31) / Game::tileSize, bottomRight.y / Game::tileSize) == 34)
    {
        if(jumping)
        {
            if (bottomRight.y + ySpeed + gravity < ((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * ((int)(bottomRight.x-31) % 64) - 22)
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
        }
        else
        {
            yPos = ((int)(bottomRight.y) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * ((int)(bottomRight.x-31) % 64) - 22;
            jumping = false;
        }
    }
    else if (tileMap.GetTileInt((bottomRight.x-31) / Game::tileSize, (bottomRight.y + ySpeed + gravity) / Game::tileSize) == 34)
    {
        if(jumping)
        {
            if (yPos + ySpeed + gravity < ((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * ((int)(bottomRight.x-31) % 64) - 22)
            {
                jumping = true;
				ySpeed += gravity;
				yPos += ySpeed;
			}
			else
			{
				jumping = false;
				ySpeed = 0;
				yPos = ((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * ((int)(bottomRight.x-31) % 64) - 22;
			}
        }
        else
        {
            yPos = ((int)(bottomRight.y) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * ((int)(bottomRight.x-31) % 64) - 22;
            jumping = false;
        }
    }
    ///**********************************************************************************************************************
    else if (tileMap.GetTileInt((bottomRight.x-31) / Game::tileSize, bottomRight.y / Game::tileSize) == 35)
    {
        if(jumping)
        {
            if (bottomRight.y + ySpeed + gravity < ((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * ((int)(bottomRight.x-31) % 64) - 43.1)
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
        }
        else
        {
            yPos = ((int)(bottomRight.y) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * ((int)(bottomRight.x-31) % 64) - 43.1;
            jumping = false;
        }
    }
    else if (tileMap.GetTileInt((bottomRight.x-31) / Game::tileSize, (bottomRight.y + ySpeed + gravity) / Game::tileSize) == 35)
    {
        if(jumping)
        {
            if (yPos + ySpeed + gravity < ((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * ((int)(bottomRight.x-31) % 64) - 43.1)
            {
                jumping = true;
				ySpeed += gravity;
				yPos += ySpeed;
			}
			else
			{
				jumping = false;
				ySpeed = 0;
				yPos = ((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * ((int)(bottomRight.x-31) % 64) - 43.1;
			}
        }
        else
        {
            yPos = ((int)(bottomRight.y + 64) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * ((int)(bottomRight.x-31) % 64) - 43.1;
            jumping = false;
        }
    }
    ///**********************************************************************************************************************
    else if (tileMap.GetTileInt((bottomRight.x) / Game::tileSize, (bottomRight.y + ySpeed + gravity) / Game::tileSize) == 33)
    {
        if(jumping)
        {
            if (yPos + ySpeed + gravity < ((int)(bottomRight.y + ySpeed + gravity) / Game::tileSize * Game::tileSize))//((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize))
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
        }

        else
        {
            if(tileMap.GetTileInt((bottomLeft.x+31) / Game::tileSize, (bottomLeft.y + 64) / Game::tileSize) == 35)
            {
                yPos = ((int)(bottomLeft.y + 32) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (((int)(bottomLeft.x+31) % 64)) -44;
                jumping = false;
            }
            else
            {
                yPos = ((int)(bottomRight.y) / Game::tileSize * Game::tileSize);
                jumping = false;
            }

        }

    }

    else if (tileMap.GetTileInt((bottomRight.x) / Game::tileSize, bottomRight.y / Game::tileSize) == 33)
    {
        if(jumping)
        {
            if (yPos + ySpeed + gravity < ((int)(yPos+64) / Game::tileSize * Game::tileSize))//((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize))
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
        }
        else
        {
            jumping = false;
            ySpeed = 0;
        }
    }

    ///************************************************************************************************************
    //
    /// Going down starts here
    //
    ///***************************************************************************************************************

    else if (tileMap.GetTileInt((bottomRight.x-31) / Game::tileSize, bottomRight.y / Game::tileSize) == 38)
    {
        if(jumping)
        {
            if (bottomLeft.y + ySpeed + gravity < ((int)(bottomLeft.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (64 - ((int)(bottomRight.x-31) % 64)) - 43.1)
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
        }
        else
        {
            yPos = ((int)(bottomRight.y) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (64 - ((int)(bottomRight.x-31) % 64)) - 43.1;
            jumping = false;
        }
    }
    else if (tileMap.GetTileInt((bottomRight.x-31) / Game::tileSize, (bottomRight.y + ySpeed + gravity) / Game::tileSize) == 38)
    {
        if(jumping)
        {
            if (yPos + ySpeed + gravity < ((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (64 - ((int)(bottomRight.x-31) % 64)) - 43.1)
            {
                jumping = true;
				ySpeed += gravity;
				yPos += ySpeed;
			}
			else
			{
				jumping = false;
				ySpeed = 0;
				yPos = ((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (64 - ((int)(bottomRight.x-31) % 64)) - 43.1;
			}
        }
        else
        {
            yPos = ((int)(bottomRight.y + 64) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (64 - ((int)(bottomRight.x-31) % 64)) - 43.1;
            jumping = false;
        }
    }
    ///**********************************************************************************************************************
    else if (tileMap.GetTileInt((bottomRight.x-31) / Game::tileSize, bottomRight.y / Game::tileSize) == 39)
    {
        if(jumping)
        {
            if (bottomLeft.y + ySpeed + gravity < ((int)(bottomLeft.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (64 - ((int)(bottomRight.x-31) % 64)) - 22)
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
        }
        else
        {
            yPos = ((int)(bottomRight.y) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (64 - ((int)(bottomRight.x-31) % 64)) - 22;
            jumping = false;
        }
    }
    else if (tileMap.GetTileInt((bottomRight.x-31) / Game::tileSize, (bottomRight.y + ySpeed + gravity) / Game::tileSize) == 39)
    {
        if(jumping)
        {
            if (yPos + ySpeed + gravity < ((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (64 - ((int)(bottomRight.x-31) % 64)) - 22)
            {
                jumping = true;
				ySpeed += gravity;
				yPos += ySpeed;
			}
			else
			{
				jumping = false;
				ySpeed = 0;
				yPos = ((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (64 - ((int)(bottomRight.x-31) % 64)) - 22;
			}
        }
        else
        {
            yPos = ((int)(bottomRight.y + 64) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (64 - ((int)(bottomRight.x-31) % 64)) - 22;
            jumping = false;
        }
    }
    ///**********************************************************************************************************************
    else if (tileMap.GetTileInt((bottomRight.x-31) / Game::tileSize, bottomRight.y / Game::tileSize) == 40)
    {
        if(jumping)
        {
            if (bottomLeft.y + ySpeed + gravity < ((int)(bottomLeft.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (64 - ((int)(bottomRight.x-31) % 64)))
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
        }
        else
        {
            yPos = ((int)(bottomRight.y) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (64 - ((int)(bottomRight.x-31) % 64));
            jumping = false;
        }
    }
    else if (tileMap.GetTileInt((bottomRight.x-31) / Game::tileSize, (bottomRight.y + ySpeed + gravity) / Game::tileSize) == 40)
    {
        if(jumping)
        {
            if (yPos + ySpeed + gravity < ((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (64 - ((int)(bottomRight.x-31) % 64)))
            {
                jumping = true;
				ySpeed += gravity;
				yPos += ySpeed;
			}
			else
			{
				jumping = false;
				ySpeed = 0;
				yPos = ((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (64 - ((int)(bottomRight.x-31) % 64));
			}
        }
        else
        {
            yPos = ((int)(bottomRight.y + 64) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (64 - ((int)(bottomRight.x-31) % 64));
            jumping = false;
        }
    }
    ///**********************************************************************************************************************
    else if (tileMap.GetTileInt((bottomLeft.x) / Game::tileSize, (bottomLeft.y + ySpeed + gravity) / Game::tileSize) == 40)
    {
        if(jumping)
        {
            if (yPos + ySpeed + gravity < ((int)(bottomLeft.y + ySpeed + gravity) / Game::tileSize * Game::tileSize))//((int)(bottomRight.y + Game::tileSize) / Game::tileSize * Game::tileSize))
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
        }

        else
        {
            if(tileMap.GetTileInt((bottomLeft.x+31) / Game::tileSize, (bottomLeft.y + 64) / Game::tileSize) == 38)
            {
                yPos = ((int)(bottomLeft.y + 32) / Game::tileSize * Game::tileSize) - tan(atan(64.0/192.0)) * (64 - ((int)(bottomLeft.x+31) % 64)) -44;
                jumping = false;
            }
            else
            {
                yPos = ((int)(bottomLeft.y) / Game::tileSize * Game::tileSize);
                jumping = false;
            }
        }

    }

    else if (tileMap.GetTileInt((bottomLeft.x) / Game::tileSize, bottomLeft.y / Game::tileSize) == 40)
    {
        if(jumping)
        {
            if (yPos + ySpeed + gravity < ((int)(yPos+64) / Game::tileSize * Game::tileSize))
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
        }
        else
        {
            jumping = false;
            ySpeed = 0;
        }
    }
    ///*****************************************************************************************************
	else if(
			(tileMap.GetTileInt(topLeft.x / Game::tileSize, (int)(topLeft.y + ySpeed + gravity) / Game::tileSize) > 0
				&& tileMap.GetTileInt(topLeft.x / Game::tileSize, (int)(topLeft.y + ySpeed + gravity) / Game::tileSize) <= 48)
			||
			(tileMap.GetTileInt(topRight.x / Game::tileSize, (int)(topRight.y + ySpeed + gravity) / Game::tileSize) > 0
				&& tileMap.GetTileInt(topRight.x / Game::tileSize, (int)(topRight.y + ySpeed + gravity) / Game::tileSize) <= 48)
			)
        {
            jumping = true;
            ySpeed = 0;
        }
	else if(
			(tileMap.GetTileInt(bottomLeft.x / Game::tileSize, (int)(bottomLeft.y + ySpeed + gravity) / Game::tileSize) == 0
				|| tileMap.GetTileInt(bottomLeft.x / Game::tileSize, (int)(bottomLeft.y + ySpeed + gravity) / Game::tileSize) >= 48)
			&&
			(tileMap.GetTileInt(bottomRight.x / Game::tileSize, (int)(bottomRight.y + ySpeed + gravity) / Game::tileSize) == 0
				|| tileMap.GetTileInt(bottomRight.x / Game::tileSize, (int)(bottomRight.y + ySpeed + gravity) / Game::tileSize) >= 48)
			)
	{
		jumping = true;
		ySpeed += gravity;
		yPos += ySpeed;
	}
    /*****************************************
    // say else if (collision does occurr)
    // then add a loop to make sure player
    // is on the ground
    *****************************************/
	else
	{
        while(true)
        {
            if((tileMap.GetTileInt(bottomLeft.x / Game::tileSize, (int)(bottomLeft.y + gravity) / Game::tileSize) == 0
                    || tileMap.GetTileInt(bottomLeft.x / Game::tileSize, (int)(bottomLeft.y + gravity) / Game::tileSize) >= 48)
                &&
                (tileMap.GetTileInt(bottomRight.x / Game::tileSize, (int)(bottomRight.y + gravity) / Game::tileSize) == 0
                    || tileMap.GetTileInt(bottomRight.x / Game::tileSize, (int)(bottomRight.y + gravity) / Game::tileSize) >= 48)
                )
            {
                yPos += gravity;
                bottomLeft = glm::vec2(xPos+1, (yPos+Game::tileSize-1));
                bottomRight = glm::vec2((xPos+Game::tileSize-1), (yPos+Game::tileSize-1));
            }
            else
            {
                jumping = false;
                ySpeed = 0;
                break;
            }
        }
	}

	if(jumping)
	{
		if(direction > 0)
		texCoordsIndex = 1;
		else
		texCoordsIndex = 0;
	}
}

Player::~Player()
{
    //dtor
}

void Player::SetupMesh()
{
    texCoords = texture->SetupTexCoords(Game::tileSize);

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
