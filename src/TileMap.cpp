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

void TileMap::Setup(Shader &shader)
{
    texture.Setup("res/textures/level1.png");
    texCoords = texture.SetupTexCoords(Game::tileSize);
    LoadFromFile(std::string("res/maps/map.txt"));
    BuildVertices();
    mesh.Setup(vertices, indices, texture);

    transUniform = glGetUniformLocation(shader.program, "transMatrix");

    translation = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0, 0.0, 1.0);
    scale = glm::vec3(1.0,1.0,1.0);
    rotationAmount = 0;
    transMatrix = glm::translate(transMatrix, translation);
    transMatrix = glm::rotate(transMatrix, glm::radians(0.0f),  rotation);
    transMatrix = glm::scale(transMatrix, scale);

    Enemy e;
    e.Setup(shader);
    enemies.push_back(e);
}

void TileMap::Update()
{
    //mesh.update();
    for(int i = 0; i < enemies.size(); i++)
    {
        enemies[i].update(this);
    }
}

void TileMap::Render()
{
    glUniformMatrix4fv(transUniform, 1, GL_FALSE, glm::value_ptr(transMatrix));
    mesh.render();
    for(int i = 0; i < enemies.size(); i++)
    {
        enemies[i].render();
    }
}

bool TileMap::GetTileCollision(int x, int y)
{
    if(x < 0 || x >= tileIndices[0].size() ||
       y < 0 || y >= tileIndices.size())
    {
        return true;
    }
    if (tileIndices[y][x] == "0")
        return false;
    else
        return true;
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

void TileMap::BuildVertices()
{
    int indicesPlacer = 0;
    for(unsigned int y = 0; y < tileIndices.size(); y++)
    {
        for(unsigned int x = 0; x < tileIndices[0].size(); x++)
        {
            if(tileIndices[y][x] != "0")
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
        }
    }
}



TileMap::~TileMap()
{
    //dtor
}
