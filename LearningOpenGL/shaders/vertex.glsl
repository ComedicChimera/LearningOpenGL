#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;

void main() {
	gl_Position = u_Proj * u_View * u_Model * position;
	v_TexCoord = texCoord;
};