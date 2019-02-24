#version 330 core

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct spotLight{
  vec3 position;
  vec3 direction;
  float cutOff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

struct directionLight{
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct pointLight{
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  
  float constant;
  float linear;
  float quadratic;
};

vec3 calcSpotLight(spotLight light, Material material, vec3 cameraPos, vec2 TexCoords, vec3 fragPosVec, vec3 normal);
vec3 calcPointLight(pointLight light, Material material, vec3 cameraPos, vec2 TexCoords, vec3 fragPosVec, vec3 normal);
vec3 calcDirLight(directionLight light, Material material, vec3 cameraPos, vec2 TexCoords, vec3 fragPosVec, vec3 normal);

in vec3 fragColour;

#define NR_POINT_LIGHTS 4  

uniform Material material;

uniform spotLight torch;
uniform directionLight dirLight;
uniform pointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 cameraPos;

in vec3 fragPosVec;
in vec3 normalVec;
in vec2 TexCoords;

out vec4 color;

void main(){
    vec3 normal = normalize(normalVec);
    
    vec3 outputVar = calcSpotLight(torch, material, cameraPos, TexCoords, fragPosVec, normal);
    //first param is the light itself
    //second param is the material it's using
    //third param is the position of the camera
    //fourth param is the texture of the current fragment, from the vertex shader
    //fifth param is the the position of this fragment
    //sixth param is the normal perpendicular to the fragment

    outputVar += calcDirLight(dirLight, material, cameraPos, TexCoords, fragPosVec, normal) * 0.5;
    //first param is the light itself
    //second param is the material it's using
    //third param is the position of the camera
    //fourth param is the texture of the current fragment, from the vertex shader
    //fifth param is the the position of this fragment
    //sixth param is the normal perpendicular to the fragment

    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        outputVar += calcPointLight(pointLights[i], material, cameraPos, TexCoords, fragPosVec, normal);
        //first param is the light itself
        //second param is the material it's using
        //third param is the position of the camera
        //fourth param is the texture of the current fragment, from the vertex shader
        //fifth param is the the position of this fragment
        //sixth param is the normal perpendicular to the fragment
    }

    color = vec4(outputVar, 1.0f);
}

vec3 calcSpotLight(spotLight light, Material material, vec3 cameraPos, vec2 TexCoords, vec3 fragPosVec, vec3 normal){
    vec3 lightDirToSourceFromFragment = normalize(light.position - fragPosVec);

    //calculating the intensity in the inside of the viewing cone of the spot light torch thing
    float outerCutOff = light.cutOff - 0.05;
    float cosThetaBetweenFragmentAndLight = dot(lightDirToSourceFromFragment, normalize(-light.direction));
    float epsilon = light.cutOff - outerCutOff;
    float intensity = (cosThetaBetweenFragmentAndLight - outerCutOff) / epsilon;

    //calculates the falloff of the brightness (not for ambient)
    float distance = length(light.position - fragPosVec);
    float attenuation = 1.0 / (light.constant + (light.linear * distance) + light.quadratic * (distance * distance));

    //calculates ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)) * 0.2;

    //calculates diffuse lighting
    float diff = max(dot(normal, lightDirToSourceFromFragment), 0.0f);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

    //calculates specular lighting
    vec3 viewDir = normalize(cameraPos - fragPosVec);
    vec3 reflectDir = reflect(-lightDirToSourceFromFragment, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

    //returns the effect of the specular, diffuse and ambient lightings, coupled with attenuation and change
    return (((specular + diffuse) * attenuation * intensity) + ambient);
}

vec3 calcPointLight(pointLight light, Material material, vec3 cameraPos, vec2 TexCoords, vec3 fragPosVec, vec3 normal){
    vec3 lightDirToSourceFromFragment = normalize(light.position - fragPosVec);

    //calculates the falloff of the brightness (not for ambient)
    float distance = length(light.position - fragPosVec);
    float attenuation = 1.0 / (light.constant + (light.linear * distance) + light.quadratic * (distance * distance));

    //calculates ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)) * 0.1;

    //calculates diffuse lighting
    float diff = max(dot(normal, lightDirToSourceFromFragment), 0.0f);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

    //calculates specular lighting
    vec3 viewDir = normalize(cameraPos - fragPosVec);
    vec3 reflectDir = reflect(-lightDirToSourceFromFragment, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

    //returns the effect of the specular, diffuse and ambient lightings, coupled with attenuation and change in intensity
    return (((specular + diffuse) * attenuation) + ambient);
}

vec3 calcDirLight(directionLight light, Material material, vec3 cameraPos, vec2 TexCoords, vec3 fragPosVec, vec3 normal){
    vec3 lightDirToSourceFromFragment = normalize(-light.direction); //light is infinitely far away, so essentially just a single direction

    //calculates ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)) * 0.1;

    //calculates diffuse lighting
    float diff = max(dot(normal, lightDirToSourceFromFragment), 0.0f);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

    //calculates specular lighting
    vec3 viewDir = normalize(cameraPos - fragPosVec);
    vec3 reflectDir = reflect(-lightDirToSourceFromFragment, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

    return (specular + diffuse + ambient);
}