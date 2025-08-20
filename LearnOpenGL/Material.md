# Material
�ޗ��̐��� (Material) �͕��̂��ǂ̂悤�Ɍ��𔽎˂��邩���`����B
* ambient: �������ǂ̐F�Ŕ��˂��邩
* diffuse: �g�U�����ǂ̐F�Ŕ��˂��邩
* specular: ���ʔ��˂̐F
* shininess: �n�C���C�g�̉s�� (�傫���قǌ��򂪉s���A�������قǂڂ₯��)

���̑����͌������̂��̂������̐������`����B
* ambient: �����Ƃ��āA���͑S�̂��ϓ��ɏƂ炷���� (�����S�̖̂��邳)
* diffuse: �ꌾ���畨�̕\�ʂɓ�����g�U���˂Ɋ�^������̋��� (�����v�̖��邳)
* specular: ���ʔ̎ЂɊ�^������̋��� (�X�|�b�g���C�g�̉s��)

���ʂ̐F = �����̐��� * �ޗ��̐���  

�ŕ\�����B��{�I�ɂ͂ǂ̃I�u�W�F�N�g�ɑ΂��Ă��������̑������g�p����B

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