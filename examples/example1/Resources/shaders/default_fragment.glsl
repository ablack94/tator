#version 330 core
# Inputs
in vec2 iTexCoord;
# Outputs
out vec4 oColor;
# Globals
uniform sampler2D texture1;

void main() {
	oColor = texture(texture1, iTexCoord);
}
