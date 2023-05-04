#type vertex
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texPos;
layout(location = 3) in float a_texInd;
layout(location = 4) in float a_tilingFactor;

uniform mat4 u_viewproj;

out vec4 v_color;
out vec2 v_texPos;
out float v_texInd;
out float v_tilingFactor;

void main() {
	gl_Position = u_viewproj * vec4(a_position, 1.0);	
	v_color = a_color;
	v_texPos = a_texPos;
	v_texInd = a_texInd;
	v_tilingFactor = a_tilingFactor;
}

#type fragment
#version 460 core

in vec4 v_color;
in vec2 v_texPos;
in float v_texInd;
in float v_tilingFactor;

out vec4 color;

uniform sampler2D u_textures[32];

void main() {
	int ind = int(v_texInd);
	if (ind == 0) {
		color = texture(u_textures[0], v_texPos * v_tilingFactor) * v_color;
	}
	else if (ind == 1) {
		color = texture(u_textures[1], v_texPos * v_tilingFactor) * v_color;
	}
	else if (ind == 2) {
		color = texture(u_textures[2], v_texPos * v_tilingFactor) * v_color;
	}
	else {
		color = vec4(1.0, 1.0, 1.0, 1.0);
	}
}