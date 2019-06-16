#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

//the light intensity and such
struct Light{
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

uniform Light light;
uniform vec3 tint;

in vec3 fragPosVec;
in vec3 normalVec;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

vec3 calcAttenuatedLight(vec3 lightPos){
	vec3 norm = normalize(normalVec);
	vec3 lightDir = normalize(lightPos - fragPosVec);

	float distance = length(lightPos - fragPosVec);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse * attenuation;

	vec3 viewDir = normalize(lightPos - fragPosVec);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);

	return (light.ambient*attenuation + (spec * light.specular * attenuation) + diffuse) * texture(texture_diffuse1, TexCoords).xyz;
}

vec3 calcInfiniteLight(vec3 lightPos){
	vec3 norm = normalize(normalVec);
	vec3 lightDir = normalize(-fragPosVec);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse;

	vec3 viewDir = normalize(-fragPosVec);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);

	return (light.ambient + (spec * light.specular) + diffuse) * texture(texture_diffuse1, TexCoords).xyz;
}

void main()
{
	FragColor.xyz = calcAttenuatedLight(light.position) * tint;
}