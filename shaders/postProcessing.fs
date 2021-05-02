#version 440 core

out vec4 Color;
in vec2 texCoords;

uniform sampler2D scene;
uniform vec2 offsets[9];
uniform int kernal[9];
uniform float blur[9];

uniform bool chaos;
uniform bool confuse;
uniform bool shake;

void main() {
	Color = vec4(0.0f);
	vec3 arr[9];
	if(chaos || shake){
		for(int i = 0; i < 9; i++) {
			arr[i] = vec3(texture(scene, texCoords.st + offsets[i]));
		}
	}

	if(chaos) {
		for(int i = 0; i < 9; i++) {
			Color += vec4(arr[i] * kernal[i], 0.0f);
		}
		Color.a = 1.0f;
	}
	else if(confuse) {
		Color = vec4(1.0 - texture(scene, texCoords).rgb, 1.0);
	}
	else if(shake) {
		for(int i = 0; i < 9; i++) {
			Color += vec4(arr[i] * blur[i], 0.0f);
		}
		Color.a = 1.0f;
	} 
	else {
		Color = texture(scene, texCoords);
	}
}