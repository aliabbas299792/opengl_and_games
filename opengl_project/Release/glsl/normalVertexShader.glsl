#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normalVector;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in mat4 instances;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
	vec2 TexCoords;
	vec3 fragPosVec;
	vec3 normalVec;
} vs_out;

void main(){
    gl_Position = projection * view * model * vec4(pos, 1.0f);

	vs_out.normalVec = mat3(transpose(inverse(model))) * normalVector;
	vs_out.fragPosVec = vec3(model * vec4(pos, 1.0f));
    vs_out.TexCoords = texCoords; //output texture coordinates
}