#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normalVector;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in mat4 instances;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;
out vec3 fragPosVec;
out vec3 normalVec;

void main(){
    gl_Position = projection * view * model * vec4(pos, 1.0f);

	normalVec = mat3(transpose(inverse(model))) * normalVector;
	fragPosVec = vec3(model * vec4(pos, 1.0f));
    TexCoords = texCoords; //output texture coordinates
}