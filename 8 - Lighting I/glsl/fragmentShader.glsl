#version 330 core

 //the position of the light set in the progShader object

in vec3 fragColour;
in vec2 texCoordinates;

in vec3 objectColourFrag;
in vec3 normalVec;
in vec3 fragPosVec;
in vec3 cameraPosVec;
in vec3 lightPosVec;
in vec3 lightColourVec;

out vec4 color;

void main(){
  float ambientStrength = 0.2; //basic environmental ambient lighting
  float specularStrength = 1.5; //specular lighting strength

  vec3 ambient = ambientStrength * lightColourVec;

  vec3 normal = normalize(normalVec); //normalises the normal, to be safe
  vec3 lightDir = normalize(lightPosVec - fragPosVec); //the normalised vector is the direction of the light
  
  float diff = max(dot(normal, lightDir), 0.0f); //recall that the dot product outputs 0 when vectors are perpendicular, and gets closer to 1 as the angle between them is smaller
  //however when the angle between them is more than 90 degrees, the dot product becomes negative, and so we use the max(vector, 0.0f), to clamp it to a minimum of 0.0f
  vec3 diffuse = diff * lightColourVec;

  vec3 viewDir = normalize(cameraPosVec - fragPosVec); //gets the direction towards the light
  vec3 reflectDir = reflect(-lightDir, normal); //inputs the vector towards the point, so from the light source, and returns the reflected ray vector, with the normal being the specified normal (2nd parameter)
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32); //the closer the viewer is to the angle of reflection, the brighter the highlight, more so as it's raised to a high power
  //raising it to a higher power results in a more focused highlight
  vec3 specular = specularStrength * spec * lightColourVec; //tones down the highlight and makes it the colour of the light

  vec3 result = (ambient + diffuse + specular) * objectColourFrag; //the phong model is applied
  //if the phong calculations are done in the vertex shader (where it is more efficient), it is of a lower quality, as there are many more fragments than there are verticies, so more calculations are carried out

  color.xyz = result;
}