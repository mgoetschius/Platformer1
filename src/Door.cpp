#include "Door.h"
#include "TextureManager.h"

Door::Door()
{
    //ctor
}

void Door::Setup(Shader & shader, int x, int y, int size)
{
    xPos = x;
    yPos = y;
    texture = TextureManager::LoadTexture("./res/textures/door.png");
    SetupMesh("./res/textures/door.png", size);

    transUniform = glGetUniformLocation(shader.program, "transMatrix");
    translation = glm::vec3(xPos, yPos, 0.0f);
    rotation = glm::vec3(0.0, 0.0, 1.0);
    scale = glm::vec3(size,size,1.0);
    rotationAmount = 0;

    transMatrix = glm::translate(transMatrix, translation);
    transMatrix = glm::rotate(transMatrix, glm::radians(rotationAmount),  rotation);
    transMatrix = glm::scale(transMatrix, scale);
}

void Door::update()
{
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

    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex), tex);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Door::render()
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

void Door::SetupMesh(const char *filename, int size)
{
    texCoords = texture->SetupTexCoords(size);

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glBindVertexArray(0);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}


Door::~Door()
{
    //dtor
}
