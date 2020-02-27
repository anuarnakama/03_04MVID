#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 uv;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    int shininess;
};
uniform Material material;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
#define NUMBER_POINT_LIGHTS 2
uniform PointLight pointLight[NUMBER_POINT_LIGHTS];

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
#define NUMBER_SPOT_LIGHTS 1
uniform SpotLight spotLight[NUMBER_SPOT_LIGHTS];

uniform vec3 viewPos;

vec3 calcDirectionalLight(DirLight light, vec3 normal, vec3 viewDir, vec3 albedoMap, vec3 specularMap) {
    vec3 ambient = albedoMap * light.ambient;

    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * albedoMap * light.diffuse;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * specularMap * light.specular;

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos, vec3 albedoMap, vec3 specularMap) {
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant +
        light.linear * distance +
        light.quadratic * distance * distance);

    vec3 ambient = albedoMap * light.ambient;

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * albedoMap * light.diffuse;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * specularMap * light.specular;

    return (ambient + diffuse + specular) * attenuation;
}

vec3 calcSpotLight(SpotLight spotlight, vec3 normal, vec3 viewDir, vec3 fragPos, vec3 albedoMap, vec3 specularMap) {
    float distance = length(spotlight.position - fragPos);
    float attenuation = 1.0 / (spotlight.constant +
        spotlight.linear * distance +
        spotlight.quadratic * distance * distance);

    vec3 ambient = albedoMap * spotlight.ambient;

    vec3 lightDir = normalize(spotlight.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * albedoMap * spotlight.diffuse;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * specularMap * spotlight.specular;

    float theta = dot(lightDir, normalize(-spotlight.direction));
    float epsilon = spotlight.cutOff - spotlight.outerCutOff;
    float intensity = clamp(((theta - spotlight.outerCutOff) / epsilon), 0.0, 1.0);

    return ((ambient + (diffuse * intensity) + (specular * intensity)) * attenuation);
    //return (ambient + diffuse + specular) * attenuation;
}

void main() {
    vec3 albedo = vec3(texture(material.diffuse, uv));
    vec3 specular = vec3(texture(material.specular, uv));

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 finalColorDir = calcDirectionalLight(dirLight, norm, viewDir, albedo, specular);

    vec3 finalColorPoint = vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < NUMBER_POINT_LIGHTS; i++) {
     finalColorPoint += calcPointLight(pointLight[i], norm, viewDir, fragPos, albedo, specular);
	}

    vec3 finalColorSpot = vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < NUMBER_SPOT_LIGHTS; i++) {
     finalColorSpot += calcSpotLight(spotLight[i], norm, viewDir, fragPos, albedo, specular);
	}

    vec3 finalColor = finalColorDir + finalColorPoint + finalColorSpot;

    FragColor = vec4(finalColor, 1.0f);
}