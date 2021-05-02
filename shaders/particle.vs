#version 440 core
layout(location = 0) in vec4 vertex;

out vec4 particleColor;
out vec2 texCoords;

uniform mat4 projection;
uniform vec4 color;
uniform vec2 offset;

void main() {
	float scale = 10.0f;
	texCoords = vertex.zw;
	particleColor = color;
	gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0f, 1.0f);
}