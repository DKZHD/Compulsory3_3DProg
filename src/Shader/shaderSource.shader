#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out vec4 fragPosLight;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightMatrix;

void main()
{
	FragPos = vec3(model*vec4(aPos,1.0));
	gl_Position = proj * view * model * vec4(aPos,1.0);
	fragPosLight = lightMatrix * vec4(FragPos, 1.0);
	Normal = mat3(transpose(inverse(model))) * aNormal;
}
#shader fragment
#version 330 core
out vec3 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec4 fragPosLight;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform sampler2D shadowMap;


struct directionalLight{
	vec3 direction;
	vec3 color;
	float ambient;
};

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

float CalculateShadows(vec4 FragPosLight){
	vec3 projCoords = FragPosLight.xyz / FragPosLight.w;
	projCoords = projCoords * 0.5+0.5;

	float closestDepth = texture(shadowMap,projCoords.xy).r;
	float currentDepth = projCoords.z;

	vec3 shadowNormal = normalize(Normal);
	vec3 lightDir = normalize(lightPos-FragPos);

	float shadowBias = max(0.05*(1.0-dot(shadowNormal, -lightDir)),0.005);

	float Shadow = 0.0;
	vec2 TexelSize = 1.0 / textureSize(shadowMap, 0);
	    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * TexelSize).r; 
            Shadow += currentDepth - shadowBias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
	Shadow /= 12.0;
	if(projCoords.z > 1.0)
        Shadow = 0.0;
        
    return Shadow;
}

void main()
{
	directionalLight DL;
	DL.direction=vec3(0, 3, 6);
	vec3 lightDir = normalize(DL.direction-FragPos);
	DL.color = vec3(1,1,1);

	DL.ambient = 0.7;
	float NDL = max(dot(Normal,-lightDir),0.0);

	vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir,Normal);
    float VdR = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 Specular = material.specular * VdR * DL.color;
	vec3 Diffuse = NDL* DL.color;
	vec3 Ambient = DL.ambient * DL.color * material.ambient;

	float Shadow = CalculateShadows(fragPosLight);
	vec3 lighting = (Ambient + (1.0-Shadow)*(Diffuse+Specular))*material.diffuse;

	FragColor = lighting;
}