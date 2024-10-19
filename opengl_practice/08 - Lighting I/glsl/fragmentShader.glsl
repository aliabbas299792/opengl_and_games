#version 330 core

in vec3 fragColour;
in vec2 texCoordinates;

//each material may have different properties and different ways it may interact with light, to use this, we can make this struct, and accept it as a uniform
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

//the light intensity and such
struct Light{
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

//by accepting these as uniforms, it helps keep things a lot more organised, and allow us a lot more control over the look of our object
uniform Material material;
uniform Light light;

uniform vec3 objectColour;
uniform vec3 lightColour;
uniform vec3 cameraPos;

in vec3 fragPosVec;
in vec3 normalVec;

out vec4 color;

void main(){
  vec3 ambient = light.ambient * material.ambient;

  vec3 normal = normalize(normalVec); //normalises the normal, to be safe
  vec3 lightDir = normalize(light.position - fragPosVec); //the normalised vector is the direction of the light
  
  float diff = max(dot(normal, lightDir), 0.0f); //recall that the dot product outputs 0 when vectors are perpendicular, and gets closer to 1 as the angle between them is smaller
  //however when the angle between them is more than 90 degrees, the dot product becomes negative, and so we use the max(vector, 0.0f), to clamp it to a minimum of 0.0f
  vec3 diffuse = diff * light.diffuse * material.diffuse;

  vec3 viewDir = normalize(cameraPos - fragPosVec); //gets the direction towards the light
  vec3 reflectDir = reflect(-lightDir, normal); //inputs the vector towards the point, so from the light source, and returns the reflected ray vector, with the normal being the specified normal (2nd parameter)
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess); //the closer the viewer is to the angle of reflection, the brighter the highlight, more so as it's raised to a high power
  //raising it to a higher power results in a more focused highlight
  vec3 specular = spec * light.specular * material.specular; //tones down the highlight and makes it the colour of the light

  vec3 result = (ambient + diffuse + specular) * objectColour; //the phong model is applied
  //if the phong calculations are done in the vertex shader (where it is more efficient), it is of a lower quality, as there are many more fragments than there are verticies, so more calculations are carried out

  color.xyz = result;
}