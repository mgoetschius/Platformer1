#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <vector>
#include <string>
#include "Vertex.h"
#include "Texture.h"


class Mesh
{
    public:
        Mesh();
        void Setup(std::vector<Vertex> vert, std::vector<GLuint> ind, Texture *&tex);
        void update();
        void render();
        virtual ~Mesh();
    protected:
    private:
        GLuint vao, vbo, ibo;
        Texture *texture;
        int totalIndices;
};

#endif // MESH_H
