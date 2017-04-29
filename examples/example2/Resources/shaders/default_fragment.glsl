#version 330 core
in vec2 vtex_coord;
in vec4 ovcolor;

out vec4 color;

uniform float time;
uniform sampler2D texture1;

void main()
{
	color = vec4(
			((sin(time + (3.14/4)) / 2) + 0.5),
			((sin(time + (3.14/2)) / 2) + 0.5),
			((sin(time + (3.14))   / 2) + 0.5),
			1.0
		);
	//color = vec4(1.0, 1.0, 0.0, 1.0);
}
