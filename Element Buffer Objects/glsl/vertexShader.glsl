#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 inputColour;

out vec3 fragColour;

void main(){
    gl_Position.xyz = vertexPosition_modelspace; //no transformation will be applied to any input vertices, just passed on to next stage
    gl_Position.w = 1.0;

    fragColour = inputColour;
}