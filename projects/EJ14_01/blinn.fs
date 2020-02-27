#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 uv;
in vec4 fragPosLighSpace;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    int shininess;
};
uniform Material material;

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
uniform SpotLight light;

uniform vec3 viewPos;

uniform sampler2D depthMap;

float ShadowCalculation(vec4 fragPosLighSpace, float bias) {
    vec3 projCoords = fragPosLighSpace.xyz / fragPosLighSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    //float closestDepth = texture(depthMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for (int y = -1; y <=1; ++y) {
            float pcf = texture(depthMap, projCoords.xy + vec2(x,y) * texelSize).r;
            shadow += currentDepth -bias > pcf ? 1.0 : 0.0;
        }
	}
    shadow /= 9.0;

    if (projCoords.z > 1.0) {
     shadow = 0.0;
	}

    return shadow;
}

vec3 calcSpotLight(SpotLight spotlight, vec3 normal, vec3 viewDir, vec3 fragPos, vec3 albedoMap, vec3 specularMap, float shadow) {
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

    return ((ambient + (diffuse * intensity) + (specular * intensity)) * attenuation * (1.0 - shadow));
    //return (ambient + diffuse + specular) * attenuation;
}

void main() {
    vec3 albedo = vec3(texture(material.diffuse, uv));
    vec3 ambient = albedo * light.ambient;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * albedo * light.diffuse;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * vec3(texture(material.specular, uv)) * light.specular;

    float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);
    float shadow = ShadowCalculation(fragPosLighSpace, bias);

    //vec3 phong = ambient + ((1.0 - shadow) * (diffuse + specular));
    FragColor = vec4(calcSpotLight(light, norm, viewDir, fragPos, albedo, specular, shadow), 1.0f);
}