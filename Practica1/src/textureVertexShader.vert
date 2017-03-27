#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texture;

uniform float downOffset;
uniform mat4 trans;

out vec3 vertexColor;
out vec2 textMap;

void main(){

	vertexColor = color;
	textMap = texture;
	gl_Position = trans * vec4(position, 1.0);

}