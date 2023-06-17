#type vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texPos;
layout(location = 3) in float a_texInd;
layout(location = 4) in float a_tilingFactor;
layout(location = 5) in int a_entityID;

layout(std140, binding = 0) uniform Camera {
	mat4 u_viewproj;
};

struct Vertex {
	vec4 color;
	vec2 texPos;
	float tilingFactor;
};

layout(location = 0) out Vertex Output;
layout(location = 3) out flat float v_texInd;
layout(location = 4) out flat int v_entityID;

void main() {
	gl_Position = u_viewproj * vec4(a_position, 1.0);	
	Output.color = a_color;
	Output.texPos = a_texPos;
	Output.tilingFactor = a_tilingFactor;
	v_texInd = a_texInd;
	v_entityID = a_entityID;
}

#type fragment
#version 450 core

struct Vertex {
	vec4 color;
	vec2 texPos;
	float tilingFactor;
};

layout(location = 0) in Vertex Input;
layout(location = 3) in flat float v_texInd;
layout(location = 4) in flat int v_entityID;

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_entityID;

layout(binding = 0) uniform sampler2D u_textures[32];

void main() {
	vec4 texColor = Input.color;

	switch(int(v_texInd)) {
		case  0: texColor *= texture(u_textures[ 0], Input.texPos * Input.tilingFactor); break;
		case  1: texColor *= texture(u_textures[ 1], Input.texPos * Input.tilingFactor); break;
		case  2: texColor *= texture(u_textures[ 2], Input.texPos * Input.tilingFactor); break;
		case  3: texColor *= texture(u_textures[ 3], Input.texPos * Input.tilingFactor); break;
		case  4: texColor *= texture(u_textures[ 4], Input.texPos * Input.tilingFactor); break;
		case  5: texColor *= texture(u_textures[ 5], Input.texPos * Input.tilingFactor); break;
		case  6: texColor *= texture(u_textures[ 6], Input.texPos * Input.tilingFactor); break;
		case  7: texColor *= texture(u_textures[ 7], Input.texPos * Input.tilingFactor); break;
		case  8: texColor *= texture(u_textures[ 8], Input.texPos * Input.tilingFactor); break;
		case  9: texColor *= texture(u_textures[ 9], Input.texPos * Input.tilingFactor); break;
		case 10: texColor *= texture(u_textures[10], Input.texPos * Input.tilingFactor); break;
		case 11: texColor *= texture(u_textures[11], Input.texPos * Input.tilingFactor); break;
		case 12: texColor *= texture(u_textures[12], Input.texPos * Input.tilingFactor); break;
		case 13: texColor *= texture(u_textures[13], Input.texPos * Input.tilingFactor); break;
		case 14: texColor *= texture(u_textures[14], Input.texPos * Input.tilingFactor); break;
		case 15: texColor *= texture(u_textures[15], Input.texPos * Input.tilingFactor); break;
		case 16: texColor *= texture(u_textures[16], Input.texPos * Input.tilingFactor); break;
		case 17: texColor *= texture(u_textures[17], Input.texPos * Input.tilingFactor); break;
		case 18: texColor *= texture(u_textures[18], Input.texPos * Input.tilingFactor); break;
		case 19: texColor *= texture(u_textures[19], Input.texPos * Input.tilingFactor); break;
		case 20: texColor *= texture(u_textures[20], Input.texPos * Input.tilingFactor); break;
		case 21: texColor *= texture(u_textures[21], Input.texPos * Input.tilingFactor); break;
		case 22: texColor *= texture(u_textures[22], Input.texPos * Input.tilingFactor); break;
		case 23: texColor *= texture(u_textures[23], Input.texPos * Input.tilingFactor); break;
		case 24: texColor *= texture(u_textures[24], Input.texPos * Input.tilingFactor); break;
		case 25: texColor *= texture(u_textures[25], Input.texPos * Input.tilingFactor); break;
		case 26: texColor *= texture(u_textures[26], Input.texPos * Input.tilingFactor); break;
		case 27: texColor *= texture(u_textures[27], Input.texPos * Input.tilingFactor); break;
		case 28: texColor *= texture(u_textures[28], Input.texPos * Input.tilingFactor); break;
		case 29: texColor *= texture(u_textures[29], Input.texPos * Input.tilingFactor); break;
		case 30: texColor *= texture(u_textures[30], Input.texPos * Input.tilingFactor); break;
		case 31: texColor *= texture(u_textures[31], Input.texPos * Input.tilingFactor); break;
	}

	o_color = texColor;
	o_entityID = v_entityID;
}
