#version 330 core
out vec4 FragColor;

in FS_OUT {
	vec2 TexCoords;
	vec3 fragPosVec;
	vec3 normalVec;
} fs_out;

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

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

void main()
{
	vec3 norm = normalize(fs_out.normalVec);
	vec3 lightDir = normalize(light.position - fs_out.fragPosVec);

	float distance = length(light.position - fs_out.fragPosVec);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse * attenuation;

	vec3 viewDir = normalize(light.position - fs_out.fragPosVec);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	vec3 halfwayDir = normalize(lightDir + viewDir);

	float spec = pow(max(dot(norm, halfwayDir), 0.0), 16);

	vec3 color = (light.ambient *attenuation + (spec * light.specular * attenuation) + diffuse) * texture(texture_diffuse1, fs_out.TexCoords).xyz;

	FragColor.xyz = color * tint;
}