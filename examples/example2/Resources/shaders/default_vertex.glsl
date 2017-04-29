#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 vcolor;
layout (location = 3) in vec2 tex_coord;

out vec2 vtex_coord;
out vec4 ovcolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//gl_Position = vec4(position, 1.0f);
	gl_Position = projection * view * model * vec4(position, 1.0f);
	ovcolor = vcolor; // patss-through
	vtex_coord = tex_coord;
	//vcolor = vec4(color, 1.0); // pass through
	//vtex_coord = vec2(tex_coord.s, (1 - tex_coord.t)); // pass through
}
