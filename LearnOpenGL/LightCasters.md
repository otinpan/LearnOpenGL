# Light casters
## Directional light
無限遠からの光を扱うときは、光の方向、光の強さ、反射強度などを設定するだけでよい。  
![](https://learnopengl.com/img/lighting/light_casters_directional.png)
`fragment_directional_light.glsl`
```glsl
#version 330 core

out vec4 FragColor;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main(){
	//ambient
	vec3 ambient=light.ambient*texture(material.diffuse,TexCoords).rgb;

	//diffuse
	vec3 norm=normalize(Normal);
	vec3 lightDir=normalize(-light.direction);
	float diff=max(dot(norm,lightDir),0.0);
	vec3 diffuse=light.diffuse*diff*texture(material.diffuse,TexCoords).rgb;

	//specular
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 reflectDir=reflect(-lightDir,norm);
	float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	vec3 specular=light.specular*spec*texture(material.specular,TexCoords).rgb;

	vec3 result=ambient + diffuse + specular;
	FragColor=vec4(result,1.0);
}
```

## Point light
光源が無限遠にあるとみなせない場合は、光源とオブジェクトの距離を考慮する必要がある。
![](https://learnopengl.com/img/lighting/light_casters_point.png)

* `constant`: 光源からの距離に関係なく一定の強さを持つ光
* `linear`: 光源からの距離に応じて減衰する光
* `quadratic`: 光源からの距離の二乗に応じて減衰する光

`fragment_point_light.glsl`
```glsl
#version 330 core
out vec4 FragColor;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main(){
	// ambient
	vec3 ambient=light.ambient*texture(material.diffuse,TexCoords).rgb;

	// diffuse
	vec3 norm=normalize(Normal);
	vec3 lightDir=normalize(light.position-FragPos);
	float diff=max(dot(norm,lightDir),0.0);
	vec3 diffuse=light.diffuse*diff*texture(material.diffuse,TexCoords).rgb;

	// specular
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 reflectDir=reflect(-lightDir,norm);
	float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	vec3 specular=light.specular*spec*texture(material.specular,TexCoords).rgb;

	// attenuation
	float distance=length(light.position-FragPos);
	float attenuation=1.0/(light.constant+light.linear*distance+light.quadratic*distance*distance);

	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;

	vec3 result=ambient+diffuse+specular;
	FragColor=vec4(result,1.0);
}
```


## Spot light
splotlightはある特定の範囲のみを照らす。
![](https://learnopengl.com/img/lighting/light_casters_spotlight_angles.png)
* LightDir: 光源からフラグメントへのベクトル
* SpotDir: スポットライトの方向ベクトル
* Phi: スポットライトが照らす範囲の角度
* Theta: スポットライトの中心からのフラグメントまでの角度  

さらにそこに`cutOff`と`outerCutOff`を設定することで、スポットライトの照らす範囲を制限することができる。`cutOff`から
`outercutOff`に近づくにつれて、明るさを減衰させる。  
`fragment_spotlight.glsl`
```glsl
#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
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

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
    
    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 
```


### Combined
これらのライトを1つのオブジェクトに影響させるには、1つのシェーダにまとめる必要がある。  
`fragment_combined_light.glsl`
```glsl
#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
};

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
uniform Material material;
uniform DirectionalLight dirLight;
uniform PointLight pointLight;
uniform Spotlight spotlight;

vec3 calcDirectionalLight(DirectionalLight light, vec3 norm,vec3 viewDir){
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light,vec3 norm,vec3 viewDir){
    vec3 ambient=light.ambient*texture(material.diffuse,TexCoords).rgb;
    vec3 lightDir=normalize(light.position-FragPos);
    float diff=max(dot(norm,lightDir),0.0);
    vec3 diffuse = light.diffuse*diff*texture(material.diffuse,TexCoords).rgb;
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular=light.specular*spec*texture(material.specular,TexCoords).rgb;
    float distance=length(light.position-FragPos);
    float attenuation=1.0/(light.constant+light.linear*distance+light.quadratic*distance*distance);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return ambient + diffuse + specular;
}

vec3 calcSpotlight(Spotlight light,vec3 norm, vec3 viewDir){
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.outerCutOff - light.cutOff;
    float intensity = clamp((theta - light.cutOff) / epsilon, 0.0, 1.0);
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
```
