#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

uniform mat4 gWorldViewProjec;
uniform mat4 gWorld;

uniform vec2 scaleTex;
uniform vec2 transTex;
uniform vec2 flipScale;
uniform vec2 flipTrans;

out vec2 TexCoord0;
out vec3 Normal0;
out vec3 WorldPos0;

void main()
{
    gl_Position = gWorldViewProjec * vec4(Position, 1.0);
	TexCoord0   = flipScale * (scaleTex * TexCoord + transTex) + flipTrans;
	Normal0     = (gWorld * vec4(Normal, 0.0)).xyz;
	WorldPos0   = (gWorld * vec4(Position, 1.0)).xyz;
}
