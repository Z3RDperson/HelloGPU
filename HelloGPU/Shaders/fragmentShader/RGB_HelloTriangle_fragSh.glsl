#version 330 core

out vec4 FragmentColor;

in vec3 calculatedColor;

void main()
{
	FragmentColor = vec4(calculatedColor, 1.0);
}