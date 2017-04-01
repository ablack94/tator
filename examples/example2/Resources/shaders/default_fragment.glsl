#version 330 core
//in vec2 vtex_coord;
//in vec4 vcolor;

out vec4 color;

uniform float time;
uniform sampler2D texture1;

void main()
{
	vec4 base_color = vec4(
			((sin(time + (3.14/4)) / 2) + 0.5),
			((sin(time + (3.14/2)) / 2) + 0.5),
			((sin(time + (3.14))   / 2) + 0.5),
			1.0
		);
	//color = mix(base_color, texture(texture1, vtex_coord), 0.5);
	//color = vec4(base_color, base_color, base_color, 1.0f);
	color = base_color;
}
