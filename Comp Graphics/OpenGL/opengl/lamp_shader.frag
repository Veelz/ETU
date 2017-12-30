#version 330 core
precision highp float;

in vec2 TexCoord;

out vec4 color;

uniform vec3 lampColor;
uniform sampler2D textureBox;
uniform int hasTexture = 0;

void main()
{
	if (hasTexture == 1) {
		color = texture(textureBox, TexCoord);
	}
	else {
		color = vec4(lampColor, 1.0f);
	}
}