#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTex0Coord;
layout (location = 3) in vec2 aTex1Coord;

out vec3 calculatedColor;
out vec2 calculatedTex0Coord;
out vec2 calculatedTex1Coord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	calculatedColor = aCol;
	calculatedTex0Coord = aTex0Coord;
	calculatedTex1Coord = aTex1Coord;
}