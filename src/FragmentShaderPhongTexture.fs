#version 330 core

#define NUM_MAX_PLIGHTS 4
#define NUM_MAX_SLIGHTS 4

struct Material {
	sampler2D diffuse;
    sampler2D specular;
    float shininess;
};  

struct PLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant, linear, quadratic;
};

struct DLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SLight {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float minAperture, maxAperture;
	float constant, linear, quadratic;
};

in vec3 fragPos;  
in vec3 normalVec;  
in vec2 texCoords;
  
out vec4 color;
  
uniform int n;
uniform vec3 viewPos;
uniform Material material;
uniform DLight dlight;
uniform PLight plight[NUM_MAX_PLIGHTS];
uniform SLight slight[NUM_MAX_SLIGHTS];

vec3 DirectionalLight(DLight light, vec3 Normal, vec3 viewPosition);
vec3 PointLight(PLight light, vec3 Normal, vec3 viewPosition);
vec3 SpotLight(SLight light, vec3 Normal, vec3 viewPosition);

void main(){
	vec3 pLight1 = PointLight(plight[0], normalVec, viewPos), pLight2 = PointLight(plight[1], normalVec, viewPos);
	vec3 sLight1 = SpotLight(slight[0], normalVec, viewPos), sLight2 = SpotLight(slight[1], normalVec, viewPos);
	vec3 dLight1 = DirectionalLight(dlight, normalVec, viewPos);
	//Output color
	color = vec4((pLight1 + pLight2 + sLight1 + sLight2 + dLight1), 1.0);
	
} 

vec3 DirectionalLight(DLight light, vec3 Normal, vec3 viewPosition){
	//Other variables
	vec3 L = normalize(-light.direction);
	vec3 N = normalize(Normal);
	vec3 R = reflect(-L,N);
	vec3 V = normalize(viewPosition - fragPos);

	//Difuse Light - Checked
	vec3 diffuse = light.diffuse * (max(dot(N, L), 0.0)) * material.diffuse;

	//Specular Light
	vec3 specular =  light.specular * pow(max(dot(R,V), 0.0), n) * material.specular;

	return light.ambient * material.diffuse + diffuse + specular;
}

vec3 PointLight(PLight light, vec3 Normal, vec3 viewPosition){
	//Attenuation factor variables
	float d = length(light.position - fragPos);
	float attFact = 1 / (light.constant + light.linear * d + light.quadratic * (d * d));
	
	//Other variables
	vec3 L = normalize(lightPos - fragPos);
	vec3 N = normalize(Normal);
	vec3 R = reflect(-L, N);
	vec3 V = normalize(viewPosition - fragPos);

	//Ambient Light
	vec3 ambient = attFact * light.ambient * material.diffuse;

	//Difuse Light
	vec3 diffuse = attFact * light.diffuse * (max(dot(N, L), 0.0)) * material.diffuse;

	//Specular Light
	vec3 specular = attFact * light.specular * pow(max(dot(R,V), 0.0), n) * material.specular;

	return ambient + diffuse + specular;
}

vec3 SpotLight(SLight light, vec3 Normal, vec3 viewPosition){
	
	//Attenuation factor variables
	float d = length(light.position - fragPos);
	float attFact = 1 / (light.constant + light.linear * d + light.quadratic * (d * d));

	//Other variables
	vec3 L = normalize(lightPos - fragPos);
	vec3 N = normalize(Normal);
	vec3 R = reflect(-L, N);
	vec3 V = normalize(viewPosition - fragPos);

	//Light types
	vec3 specular;
	vec3 ambiental;
	vec3 diffuse;

	//Soften focal light
	float theta = dot(L, normalize(-light.direction));
	float epsilon = light.minAperture - light.maxAperture;
	float inte = clamp((theta - light.maxAperture)/epsilon, 0, 1);

	if(theta > light.maxAperture){

		//Ambiental Light
		ambiental = attFact * light.ambient * material.diffuse;

		//Difuse Light
		difuse = attFact * light.diffuse * (max(dot(N, L), 0.0)) * material.diffuse;

		//Specular Light
		specular = attFact * light.specular * pow(max(dot(R,V), 0.0), n) * material.specular;

		//Output color
		return ambiental + inte * diffuse + inte * specular;

	}

	else{

		//Ambiental Light
		ambiental = attFact * light.ambient * material.diffuse;

		return ambiental;

	}

}