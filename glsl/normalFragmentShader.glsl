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
uniform samplerCube depthMap;

uniform float far_plane;

float ShadowCalculation(vec3 fragPos)
{
    vec3 fragToLight = fragPos - light.position;
    // ise the fragment to light vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= 100;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // test for shadows
    float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, far_plane] range
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;      
    // display closestDepth as debug (to visualize depth cubemap)
    //FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    
        
    return shadow;
}

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

	float shadow = ShadowCalculation(fs_out.fragPosVec);

	vec3 color = (light.ambient + (1.0 - shadow) *attenuation + (spec * light.specular * attenuation) + diffuse) * texture(texture_diffuse1, fs_out.TexCoords).xyz;

	vec3 color2 = diffuse*shadow;

	FragColor.xyz = color;

	
    vec3 fragToLight = fs_out.fragPosVec - light.position;
    // ise the fragment to light vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;

	//FragColor = vec4(vec3(shadow), 1.0);
}