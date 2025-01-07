#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct DirectionalLight
{
    vec3 direction;
    vec3 color;
};

struct PointLight
{
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 color;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

out vec4 PixelColor;
  
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 viewPos;

uniform Material material;

uniform int numDirLights;
uniform DirectionalLight directionalLights[10];
uniform int numPointLights;
uniform PointLight pointLights[10];
uniform int numSpotLights;
uniform SpotLight spotLights[10];

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   
    // combine results
    vec3 ambient  = (light.color * 0.1)  * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.color  * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.color * spec * vec3(texture(material.specular, TexCoord));

    return (ambient + diffuse + specular);
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    // combine results
    vec3 ambient  = (light.color * 0.1)  * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.color  * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.color * spec * vec3(texture(material.specular, TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Calculate spotlight angle
    float theta = dot(lightDir, normalize(-light.direction));
    
    // Spotlight intensity calculation based on angle
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Calculate diffuse and specular contributions
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   
    // Diffuse, and specular components
    vec3 diffuse  = light.color * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.color * spec * vec3(texture(material.specular, TexCoord));

    // Apply spotlight fading to diffuse and specular
    diffuse *= intensity;
    specular *= intensity;

    return diffuse + specular;
}

void main()
{
    // Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0, 0.0, 0.0);

    // Directional lighting
    for(int i = 0; i < numDirLights; i++)
    {
        //result += CalcDirLight(directionalLights[i], norm, viewDir);
    }

    // Point lighting
    for(int i = 0; i < numPointLights; i++)
    {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    } 

    // Spot lighting
    for(int i = 0; i < numSpotLights; i++)
    {
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
    } 

    PixelColor = vec4(result, 1.0f);
}