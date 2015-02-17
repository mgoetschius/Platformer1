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
    string s = "res/textures/level" + levelNum + ".png";
    texture.Setup(s.c_str());
    texCoords = texture.SetupTexCoords(Game::tileSize);
    s = "res/maps/level" + levelNum + ".txt";
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

    for(int i = 0; i < 1; i++)
    {
        //Enemy e;
        //e.Setup(shader, "res/textures/orangieanim.png", i * 300 + 600, 450);
        enemies.push_back(new OrangeEnemy(shader, "res/textures/orangieanim.png", i * 300 + 600, 450));
    }
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
    glUniformMatrix4fv(transUniform, 1, GL_FALSE, glm::value_ptr(transMatrix));
    mesh.render();
    for(int i = 0; i < enemies.size(); i++)
    {
        enemies[i]->render();
    }
    if(!hasKey)
        key.render();
    door.render();
}

bool TileMap::GetTileCollision(int x, int y)
{
    if(x < 0 || x >= tileIndices[0].size() ||
       y < 0 || y >= tileIndices.size())
    {
        return true;
    }
    if (tileIndices[y][x] == "0"
        || tileIndices[y][x] == "3"
        || tileIndices[y][x] == "A"
        || tileIndices[y][x] == "B"
        || tileIndices[y][x] == "D"
        || tileIndices[y][x] == "K")
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
    if (tileIndices[y][x] == "K")
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
    if (tileIndices[y][x] == "D")
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
    if (tileIndices[y][x] == "3")
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
        tileIndices.push_back(Split(line, ',', false));
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
            if(tileIndices[y][x] != "0"
               && tileIndices[y][x] != "A"
               && tileIndices[y][x] != "B"
               && tileIndices[y][x] != "D"
               && tileIndices[y][x] != "K")
            {
                Vertex v1(glm::vec3(x * Game::tileSize, y * Game::tileSize, 0.0f), glm::vec2(texCoords[atoi(tileIndices[y][x].c_str())][0], texCoords[atoi(tileIndices[y][x].c_str())][1]));
                Vertex v2(glm::vec3(x * Game::tileSize + Game::tileSize, y * Game::tileSize, 0.0f), glm::vec2(texCoords[atoi(tileIndices[y][x].c_str())][2], texCoords[atoi(tileIndices[y][x].c_str())][3]));
                Vertex v3(glm::vec3(x * Game::tileSize + Game::tileSize, y * Game::tileSize + Game::tileSize, 0.0f), glm::vec2(texCoords[atoi(tileIndices[y][x].c_str())][4], texCoords[atoi(tileIndices[y][x].c_str())][5]));
                Vertex v4(glm::vec3(x * Game::tileSize, y * Game::tileSize + Game::tileSize, 0.0f), glm::vec2(texCoords[atoi(tileIndices[y][x].c_str())][6], texCoords[atoi(tileIndices[y][x].c_str())][7]));
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
            if(tileIndices[y][x] == "A")
            {
                enemies.push_back(new OrangeEnemy(shader, "res/textures/orangieanim.png", x * Game::tileSize, y * Game::tileSize));
            }

            if(tileIndices[y][x] == "B")
            {
                enemies.push_back(new PurpleEnemy(shader, "res/textures/purpleie.png", x * Game::tileSize, y * Game::tileSize));
            }
            if(tileIndices[y][x] == "K")
            {
                key.Setup(shader, x * Game::tileSize, y * Game::tileSize, Game::tileSize);
            }
            if(tileIndices[y][x] == "D")
            {
                door.Setup(shader, x * Game::tileSize, y * Game::tileSize, Game::tileSize);
            }

        }
    }
}

void TileMap::CleanUp()
{
    std::cout << "Cleaning up TileMap\n";
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
