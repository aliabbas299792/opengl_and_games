#version 330 core
//the fragment shader, which is unique to the second shader, which deals only with the lamp cube thing
in vec3 fragColour;

out vec4 color;

void main(){
  color = vec4(1.0f);
}