# Material
材料の性質 (Material) は物体がどのように光を反射するかを定義する。
* ambient: 環境光をどの色で反射するか
* diffuse: 拡散光をどの色で反射するか
* specular: 鏡面反射の色
* shininess: ハイライトの鋭さ (大きいほど光沢が鋭く、小さいほどぼやける)

光の属性は光源そのものが放つ光の成分を定義する。
* ambient: 環境光として、周囲全体を均等に照らす強さ (部屋全体の明るさ)
* diffuse: 苦言から物体表面に当たる拡散反射に寄与する光の強さ (ランプの明るさ)
* specular: 鏡面販社に寄与する光の強さ (スポットライトの鋭さ)

結果の色 = 光源の成分 * 材料の成分  

で表される。基本的にはどのオブジェクトに対しても同じ光の属性を使用する。

`vertex_material.glsl`
```glsl
#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;



void main(){
	gl_Position =projection * view * model * vec4(aPos, 1.0);
	Normal = normalMatrix * aNormal; 
	FragPos= vec3(view * model * vec4(aPos, 1.0));
}
```

`fragment_material.glsl`
```glsl
#version 330 core
struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light{
	vec3 viewPosition;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;


uniform Material material;
uniform Light light;


void main(){
	//ambient;
	vec3 ambient=light.ambient * material.ambient;

	//difuse
	vec3 norm=normalize(Normal);
	vec3 lightDir=normalize(light.viewPosition-FragPos);

	float diff=max(dot(norm,lightDir),0.0);
	vec3 diffuse=light.diffuse * (diff * material.diffuse);


	//specular
	vec3 viewDir=normalize(-FragPos);
	vec3 reflectDir=reflect(-lightDir,norm);
	float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	vec3 specular=light.specular * (spec * material.specular);

	//result
	vec3 result=ambient + diffuse + specular;
	FragColor=vec4(result,1.0);
}
```