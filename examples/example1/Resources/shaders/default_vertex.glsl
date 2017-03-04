#version 330 core
# Inputs
layout (location = 0) in vec3 iPosition;
layout (location = 2) in vec2 iTexCoord;
# Outputs
out vec2 oTexCoord;
# Globals
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(iPosition, 1.0f);
	oTexCoord = vec2(iTexCoord.x, iTexCoord.y);
}