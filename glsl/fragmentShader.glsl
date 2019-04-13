#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

//the light intensity and such
struct Light{
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Light light;

uniform vec3 cameraPos;

in vec3 fragPosVec;
in vec3 normalVec;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

void main()
{
  vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).xyz;

  vec3 normal = normalize(normalVec); //normalises the normal, to be safe
  vec3 lightDir = normalize(light.position - fragPosVec); //the normalised vector is the direction of the light
  
  vec3 viewDir = normalize(cameraPos - fragPosVec); //gets the direction towards the light
  vec3 reflectDir = reflect(-lightDir, normal); //inputs the vector towards the point, so from the light source, and returns the reflected ray vector, with the normal being the specified normal (2nd parameter)
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 16); //the closer the viewer is to the angle of reflection, the brighter the highlight, more so as it's raised to a high power
  //raising it to a higher power results in a more focused highlight
  vec3 specular = spec * light.specular * 0.15; //tones down the highlight and makes it the colour of the light

  float diff = max(dot(normal, lightDir), 0.0f);
  vec3 diffuse = diff * light.diffuse * texture(texture_diffuse1, TexCoords).xyz;

  vec3 result = ambient + specular + diffuse; //the phong model is applied
  FragColor.xyz = result;
}