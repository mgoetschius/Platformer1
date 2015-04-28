#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <GL/glew.h>
#include <iostream>
#include <map>
#include "Texture.h"

class TextureManager
{
    public:
        TextureManager();
        static Texture* LoadTexture(const char *filename);
        static void Clear();
        virtual ~TextureManager();
    protected:
    private:
        static std::map<const char*, Texture*> textures;
};

#endif // TEXTUREMANAGER_H
