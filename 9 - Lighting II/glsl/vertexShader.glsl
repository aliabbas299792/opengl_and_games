#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normalVector;
layout(location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normalVec;
out vec3 fragPosVec;
out vec2 TexCoords;

void main(){
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    
    //whenever we apply a non-uniform scale the normal vectors are not perpendicular to the corresponding surface anymore which distorts the lighting
    //to fix this you take the transpose of the inverse of the model matrix, which applies a similar transform to the normal, but without making it become not perpendicular
    vec3 normal = mat3(transpose(inverse(model))) * normalVector;
    //though you shouldn't do this normally, as this is an expensive operation, instead do it on the CPU and then send it through a uniform to the shader

    TexCoords = texCoords; //output texture coordinates
    normalVec = normal;
    fragPosVec = vec3(model * vec4(pos, 1.0f)); //this gives the position in world space, which we will use to calculate lighting and such;
}