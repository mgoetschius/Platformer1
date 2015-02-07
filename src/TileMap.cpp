#include "TileMap.h"
#include "Model.h"

#include<iostream>
#include<fstream>
#include<algorithm>

TileMap::TileMap()
{
    //ctor
}

void TileMap::Setup(Shader &shader)
{
    LoadFromFile(std::string("res/maps/map.txt"));
    tiles.resize(tileIndices.size());
    for(unsigned int y = 0; y < tileIndices.size(); y++)
    {
        for(unsigned int x = 0; x < tileIndices[0].size(); x++)
        {
            Model m;
            m.Setup(shader, tileIndices[y][x]);
            m.SetTranslation(glm::vec3(x*64, y*64, 0));
            m.SetScale(glm::vec3(64,64,1));
            tiles[y].push_back(m);
        }
    }
}

void TileMap::Update()
{
    for(unsigned int y = 0; y < tiles.size(); y++)
    {
        for(unsigned int x = 0; x < tiles[0].size(); x++)
        {
            tiles[y][x].update();
        }
    }
}

void TileMap::Render()
{
   for(unsigned int y = 0; y < tiles.size(); y++)
    {
        for(unsigned int x = 0; x < tiles[0].size(); x++)
        {
            tiles[y][x].render();
        }
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

TileMap::~TileMap()
{
    //dtor
}
