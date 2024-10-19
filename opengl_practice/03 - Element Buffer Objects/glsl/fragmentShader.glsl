#version 330 core

in vec3 fragColour;

out vec4 color;

//uniform vec4 fragColour; //literally a simple uniform, look in main.cpp

void main(){
  color.xyz = fragColour; //the output colour of all fragments will be this
}