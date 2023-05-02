#type vertex
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texPos;

uniform mat4 u_viewproj;

out vec4 v_color;
out vec2 v_texPos;

void main() {
	gl_Position = u_viewproj * vec4(a_position, 1.0);	
	v_color = a_color;
	v_texPos = a_texPos;
}

#type fragment
#version 460 core

in vec4 v_color;
in vec2 v_texPos;

layout(location = 0) out vec4 color;

uniform sampler2D u_texture;
uniform vec4 u_color;
uniform float u_tilingFactor;

void main() {
	color = v_color;
}