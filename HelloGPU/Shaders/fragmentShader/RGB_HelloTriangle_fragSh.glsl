#version 330 core

out vec4 FragmentColor;

in vec3 calculatedColor;
in vec2 calculatedTex0Coord;
in vec2 calculatedTex1Coord;

uniform sampler2D myTexture_0;
uniform sampler2D myTexture_1;

uniform float mix_intensity;

void main()
{
	FragmentColor = mix( texture(myTexture_0, calculatedTex0Coord), texture(myTexture_1, calculatedTex1Coord), mix_intensity ) * vec4(calculatedColor, 1.0);
}