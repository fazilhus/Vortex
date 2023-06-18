#type vertex
#version 450 core

layout(location = 0) in vec3 a_worldPosition;
layout(location = 1) in vec3 a_localPosition;
layout(location = 2) in vec4 a_color;
layout(location = 3) in float a_thickness;
layout(location = 4) in float a_fade;
layout(location = 5) in int a_entityID;

layout(std140, binding = 0) uniform Camera {
	mat4 u_viewproj;
};

struct VertexOutput {
	vec3 localPosition;
	vec4 color;
	float thickness;
	float fade;
};

layout (location = 0) out VertexOutput Output;
layout (location = 4) out flat int v_entityID;

void main() {
	Output.localPosition = a_localPosition;
	Output.color = a_color;
	Output.thickness = a_thickness;
	Output.fade = a_fade;

	v_entityID = a_entityID;

	gl_Position = u_viewproj * vec4(a_worldPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_entityID;

struct VertexOutput {
	vec3 localPosition;
	vec4 color;
	float thickness;
	float fade;
};

layout (location = 0) in VertexOutput Input;
layout (location = 4) in flat int v_entityID;

void main() {
    float distance = 1.0 - length(Input.localPosition);
    float circle = smoothstep(0.0, Input.fade, distance);
    circle *= smoothstep(Input.thickness, Input.thickness - Input.fade, distance);

	if (circle == 0.0)
		discard;

    o_color = Input.color;
	o_color.a *= circle;

	o_entityID = v_entityID;
}
