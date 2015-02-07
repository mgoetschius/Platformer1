#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

uniform mat4 transMatrix;
uniform mat4 projMatrix;

out vec2 TexCoord;

void main()
{
    gl_Position = projMatrix * transMatrix * vec4(position.x, position.y, 0.0, 1.0);
	TexCoord = texCoord;
}
