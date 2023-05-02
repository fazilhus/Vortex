#type vertex
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texPos;

uniform mat4 u_viewproj;
uniform mat4 u_transform;

out vec2 v_texPos;

void main() {
	v_texPos = a_texPos;
	gl_Position = u_viewproj * u_transform * vec4(a_position, 1.0);	
}

#type fragment
#version 460 core

in vec2 v_texPos;

layout(location = 0) out vec4 color;

uniform sampler2D u_texture;
uniform vec4 u_color;
uniform float u_tilingFactor;

void main() {
	color = texture(u_texture, v_texPos * u_tilingFactor) * u_color;
}