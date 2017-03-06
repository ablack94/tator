#version 330 core
in vec2 texCoord;
in vec4 vcolor;

out vec4 color;

uniform float time;

void main()
{
	color.r = (sin(time + (3.14/4)) / 2) + 0.5;
	color.g = (sin(time + (3.14/2)) / 2) + 0.5;
	color.b = (sin(time + (3.14))   / 2) + 0.5;
	color.a = 1.0;
}
