#version 330 core

// Input data

in vec3 vertexColor;
in vec2 textMap;

// Ouput data
out vec4 color;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float opacity;

void main() {

	// Output color
	color = mix(texture(texture1, textMap), texture(texture2, textMap), opacity);


}