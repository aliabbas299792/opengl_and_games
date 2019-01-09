#version 330 core

in vec3 fragColour;
in vec2 texCoordinates;

out vec4 color;

uniform sampler2D rectTexture;

//uniform vec4 fragColour; //literally a simple uniform, look in main.cpp

void main(){
  color = texture(rectTexture, texCoordinates); //the output colour of all fragments will be this
}