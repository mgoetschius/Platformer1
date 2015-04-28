#include "TextureManager.h"
#include "soil.h"

std::map<const char*, Texture*> TextureManager::textures;

TextureManager::TextureManager()
{
    //ctor
}

Texture* TextureManager::LoadTexture(const char* filename)
{
    if(textures.find(filename) != textures.end())
    {
        return textures[filename];
    }
    else
    {
        textures[filename] = new Texture(filename);
        return textures[filename];
    }
}

void TextureManager::Clear()
{
    for(std::map<const char*, Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
    {
        GLuint id = it->second->GetTexture();
        glDeleteTextures(1, &id);
        delete it->second;
    }
    textures.clear();
}

TextureManager::~TextureManager()
{
    for(std::map<const char*, Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
    {
        GLuint id = it->second->GetTexture();
        glDeleteTextures(1, &id);
        delete it->second;
    }
    textures.clear();
}
