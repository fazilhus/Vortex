#type vertex
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;

uniform mat4 u_viewproj;
uniform mat4 u_transform;

out vec3 v_position;
out vec4 v_color;

void main() {
	v_position = a_position;
	v_color = a_color;
	gl_Position = u_viewproj * u_transform * vec4(a_position, 1.0);	
}

#type fragment
#version 460 core

in vec3 v_position;
in vec4 v_color;

layout(location = 0) out vec4 color;

void main() {
	color = vec4(v_position * 0.5 + 0.5, 1.0);
	color = v_color;
}