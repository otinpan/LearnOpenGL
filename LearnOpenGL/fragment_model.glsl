#version 330 core
out vec4 FragColor;


struct DirectionalLight{
    vec3 direction;
    vec3 ambient,diffuse,specular;
};

struct PointLight{
    vec3 position;  
    vec3 ambient,diffuse,specular;
    float constant,linear,quadratic;
};

struct Spotlight{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 ambient,diffuse, specular;
    float constant, linear, quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirectionalLight dirLight;
uniform PointLight pointLight;
uniform Spotlight spotlight;
uniform sampler2D texture_diffuse1;
uniform float shininess;
uniform vec3 specularColor;

vec3 calcDirectionalLight(DirectionalLight light, vec3 norm,vec3 viewDir){
    vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * spec * specularColor;
    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light,vec3 norm,vec3 viewDir){
    vec3 ambient=light.ambient*texture(texture_diffuse1,TexCoords).rgb;
    vec3 lightDir=normalize(light.position-FragPos);
    float diff=max(dot(norm,lightDir),0.0);
    vec3 diffuse = light.diffuse*diff*texture(texture_diffuse1,TexCoords).rgb;
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),shininess);
    vec3 specular=light.specular*spec*specularColor;
    float distance=length(light.position-FragPos);
    float attenuation=1.0/(light.constant+light.linear*distance+light.quadratic*distance*distance);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return ambient + diffuse + specular;
}

vec3 calcSpotlight(Spotlight light,vec3 norm, vec3 viewDir){
    vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;  
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * spec * specularColor;
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon   = light.cutOff - light.outerCutOff;            // inner - outer Ç≈ê≥ÇÃíl
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    diffuse  *= intensity;
    specular *= intensity;
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;
    return ambient + diffuse + specular;
}

void main(){
    vec3 norm=normalize(Normal);
    vec3 viewDir=normalize(viewPos-FragPos);

    vec3 result=vec3(0.0);
    result+=calcDirectionalLight(dirLight,norm,viewDir);
    result+=calcPointLight(pointLight,norm,viewDir);
    result+=calcSpotlight(spotlight,norm,viewDir);

    FragColor=vec4(result,1.0);
}

