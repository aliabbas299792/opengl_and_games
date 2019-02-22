#version 330 core

in vec3 fragColour;

//each material may have different properties and different ways it may interact with light, to use this, we can make this struct, and accept it as a uniform
struct Material{
    sampler2D diffuse; //the ambient is almost always equal to the diffuse colour of the material, so no need for it really, and we have made the diffuse into a sampler2D,
    //which basically will hold the diffuse map, which is a texture, which will allow for the pixels inside of a single texture to be treated differently
    sampler2D specular; //the specular map, which means that the metallic bits can be made to be more shiny than wooden bits for example
    sampler2D emission; //the emission map, which allows you to add some sort of extra highlighting if you want
    float shininess; //the power that the specular highlight is raised to
};

//the light intensity and such
struct Light{
  vec3 position; //position of the light, not needed when directional light
  //vec3 direction; //for directional light

  vec3 ambient; //the ambient colour
  vec3 diffuse; //the diffuse colour
  vec3 specular; //the specular colour
  //all of these colours obviously have intensity mixed in with them

  //the attenuation values for the point light
  float constant;
  float linear;
  float quadratic;
};

//by accepting these as uniforms, it helps keep things a lot more organised, and allow us a lot more control over the look of our object
uniform Material material;
uniform Light light;

uniform vec3 objectColour;
uniform vec3 lightColour;
uniform vec3 cameraPos;
uniform float time;

in vec3 fragPosVec;
in vec3 normalVec;
in vec2 TexCoords;

out vec4 color;

void main(){
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)); //replaces the ambient colour with the colour sampled from the texture

  float distance    = length(light.position - fragPosVec);
  float attenuation = 1.0 / (light.constant + (light.linear * distance) + light.quadratic * (distance * distance));    

  vec3 normal = normalize(normalVec); //normalises the normal, to be safe
  //the below is not used in directional lights
  vec3 lightDir = normalize(light.position - fragPosVec); //the normalised vector is the direction of the light, from the fragment to the source, using the coordinates from there as well
  //vec3 lightDir = normalize(-light.direction); //the direction, provided by the directional light

  float diff = max(dot(normal, lightDir), 0.0f); //recall that the dot product outputs 0 when vectors are perpendicular, and gets closer to 1 as the angle between them is smaller
  //however when the angle between them is more than 90 degrees, the dot product becomes negative, and so we use the max(vector, 0.0f), to clamp it to a minimum of 0.0f
  vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords)); //replaces the diffuse colour with the colour sampled from the texture, using the coordinates from there as well

  vec3 viewDir = normalize(cameraPos - fragPosVec); //gets the direction towards the light
  vec3 reflectDir = reflect(-lightDir, normal); //inputs the vector towards the point, so from the light source, and returns the reflected ray vector, with the normal being the specified normal (2nd parameter)
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess); //the closer the viewer is to the angle of reflection, the brighter the highlight, more so as it's raised to a high power
  //raising it to a higher power results in a more focused highlight
  vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords)); //tones down the highlight and makes it the colour of the light

  vec3 emission = vec3(0.0f); //initialise the emission vector to 0
  if(texture(material.specular, TexCoords).r < 0.07f){ //checks if the specular map's red component is less than 0.07f, which indicates it's probably the black bit
    emission = texture(material.emission, TexCoords + vec2(0.0f, time)).xyz; //if it is, add the emission map to the resultant colour, and shift it according to the time
  }

  vec3 result = ( (ambient + diffuse + specular) * attenuation /*+ emission it's too bright, doesn't help with testing*/ ) * objectColour; //the phong reflection model is applied
  //if the phong calculations are done in the vertex shader (where it is more efficient), it is of a lower quality, as there are many more fragments than there are verticies, so more calculations are carried out
  //as you can see, I have multiplied by attenuation, which will result in things further from a point light source being dimmer

  color.xyz = result;
}