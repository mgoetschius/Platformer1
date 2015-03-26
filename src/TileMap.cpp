#include "TileMap.h"
#include "Model.h"
#include "Game.h"

#include<iostream>
#include<fstream>
#include<algorithm>

TileMap::TileMap()
{
    //ctor
}

void TileMap::Setup(Shader &shader, string levelNum)
{
    string s = "./res/textures/level" + levelNum + ".png";
    texture.Setup(s.c_str());
    texCoords = texture.SetupTexCoords(Game::tileSize);
    s = "./res/maps/level" + levelNum + ".txt";
    LoadFromFile(s);
    BuildVertices(shader);
    mesh.Setup(vertices, indices, texture);

    transUniform = glGetUniformLocation(shader.program, "transMatrix");

    translation = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0, 0.0, 1.0);
    scale = glm::vec3(1.0,1.0,1.0);
    rotationAmount = 0;
    transMatrix = glm::translate(transMatrix, translation);
    transMatrix = glm::rotate(transMatrix, glm::radians(0.0f),  rotation);
    transMatrix = glm::scale(transMatrix, scale);
}

void TileMap::Update(double dt)
{
    for(int i = 0; i < enemies.size(); i++)
    {
            enemies[i]->Update(this, dt);
            if(enemies[i]->GetYPos() > tileIndices.size() * Game::tileSize + Game::tileSize)
                enemies[i]->SetRemove(true);
    }
    for(std::vector<Enemy*>::iterator i = enemies.begin(); i != enemies.end(); )
    {
        if((*i)->GetRemove())
        {
            delete *i;
            i = enemies.erase(i);
        }
        else
        {
            i++;
        }
    }
}

void TileMap::Render()
{
    if(!hasKey)
        key.render();
    door.render();
    glUniformMatrix4fv(transUniform, 1, GL_FALSE, glm::value_ptr(transMatrix));
    mesh.render();
    for(int i = 0; i < enemies.size(); i++)
    {
        enemies[i]->render();
    }
}

int TileMap::GetTileInt(int x, int y)
{
    if(x < 0 || x >= tileIndices[0].size() ||
       y < 0 || y >= tileIndices.size())
    {
        return -1;
    }
    else
    {
        return tileInts[y][x];
    }
}

bool TileMap::GetTileCollision(int x, int y)
{
    if(x < 0 || x >= tileIndices[0].size() ||
       y < 0 || y >= tileIndices.size())
    {
        return true;
    }
    if (tileInts[y][x] == 0
        || tileInts[y][x] > 32)
        return false;
    else
        return true;
}

bool TileMap::GetKeyCollision(int x, int y)
{
    if(x < 0 || x >= tileIndices[0].size() ||
       y < 0 || y >= tileIndices.size())
    {
        return false;
    }
    if (tileInts[y][x] == 63)
        return true;
    else
        return false;
}
bool TileMap::GetDoorCollision(int x, int y)
{
    if(x < 0 || x >= tileIndices[0].size() ||
       y < 0 || y >= tileIndices.size())
    {
        return false;
    }
    if (tileInts[y][x] == 62)
        return true;
    else
        return false;
}

bool TileMap::GetLadderCollision(int x, int y)
{
    if(x < 0 || x >= tileIndices[0].size() ||
       y < 0 || y >= tileIndices.size())
    {
        return false;
    }
    if (tileInts[y][x] == 64)
        return true;
    else
        return false;
}

void TileMap::LoadFromFile(std::string filename)
{
    std::ifstream in(filename, std::ios::in);

    if(!in)
        std::cerr << "Error: unable to open file" << std::endl;

    std::string line;
    while(std::getline(in, line))
    {
        tileIndices.push_back(Split(line, ',', true));
    }
    /// convert tileIndicies to tileInts
    std::vector<int> row;
    for(int y = 0; y < tileIndices.size(); y++)
    {
        row.clear();
        for(int x = 0; x < tileIndices[0].size(); x++)
        {
            int res;
            stringstream convert(tileIndices[y][x]);
            if ( !(convert >> res) )
                res = 0;
            row.push_back(res);
        }
        tileInts.push_back(row);
    }
}

bool isEmptyString(std::string s) {return s.length() == 0;}
std::vector<std::string> TileMap::Split(std::string& s, char c, bool removeEmpties)
{

    std::vector<std::string> vstr;
    std::string::size_type i = 0;
    std::string::size_type j = s.find(c);
    while(j != std::string::npos)
    {
        vstr.push_back(s.substr(i, j-i));
        i = ++j;
        j = s.find(c, j);

        if (j == std::string::npos)
         vstr.push_back(s.substr(i, s.length()));
    }
    if(removeEmpties)
        vstr.erase(std::remove_if(vstr.begin(), vstr.end(), isEmptyString), vstr.end());

    return vstr;
}

void TileMap::BuildVertices(Shader &shader)
{
    int indicesPlacer = 0;
    for(unsigned int y = 0; y < tileIndices.size(); y++)
    {
        for(unsigned int x = 0; x < tileIndices[0].size(); x++)
        {
            if((tileInts[y][x] > 0
               && tileInts[y][x] <= 56)
               || tileInts[y][x] == 64)
            {
                Vertex v1(glm::vec3(x * Game::tileSize, y * Game::tileSize, 0.0f), glm::vec2(texCoords[atoi(tileIndices[y][x].c_str()) - 1][0], texCoords[atoi(tileIndices[y][x].c_str()) - 1][1]));
                Vertex v2(glm::vec3(x * Game::tileSize + Game::tileSize, y * Game::tileSize, 0.0f), glm::vec2(texCoords[atoi(tileIndices[y][x].c_str()) - 1][2], texCoords[atoi(tileIndices[y][x].c_str()) - 1][3]));
                Vertex v3(glm::vec3(x * Game::tileSize + Game::tileSize, y * Game::tileSize + Game::tileSize, 0.0f), glm::vec2(texCoords[atoi(tileIndices[y][x].c_str()) - 1][4], texCoords[atoi(tileIndices[y][x].c_str()) - 1][5]));
                Vertex v4(glm::vec3(x * Game::tileSize, y * Game::tileSize + Game::tileSize, 0.0f), glm::vec2(texCoords[atoi(tileIndices[y][x].c_str()) - 1][6], texCoords[atoi(tileIndices[y][x].c_str()) - 1][7]));
                vertices.push_back(v1);
                vertices.push_back(v2);
                vertices.push_back(v3);
                vertices.push_back(v4);
                indices.push_back(indicesPlacer++);
                indices.push_back(indicesPlacer++);
                indices.push_back(indicesPlacer);
                indices.push_back(indicesPlacer-2);
                indices.push_back(indicesPlacer);
                indices.push_back(++indicesPlacer);
                indicesPlacer++;
            }
            if(tileInts[y][x] == 57)
            {
                enemies.push_back(new OrangeEnemy(shader, "./res/textures/orangieanim.png", x * Game::tileSize, y * Game::tileSize));
            }

            if(tileInts[y][x] == 58)
            {
                enemies.push_back(new PurpleEnemy(shader, "./res/textures/purpleie.png", x * Game::tileSize, y * Game::tileSize));
            }
            if(tileInts[y][x] == 63)
            {
                key.Setup(shader, x * Game::tileSize, y * Game::tileSize, Game::tileSize);
            }
            if(tileInts[y][x] == 62)
            {
                door.Setup(shader, x * Game::tileSize, y * Game::tileSize, Game::tileSize);
            }

        }
    }
}

void TileMap::CleanUp()
{
    for(std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); it++)
    {
        delete (*it);
    }
    enemies.clear();
}


TileMap::~TileMap()
{
    /*

    */
}
