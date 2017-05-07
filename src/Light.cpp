#include "Light.h"

Light::Light(vec3 pos, vec3 dir, vec3 ambient, vec3 diffuse, vec3 specular, LType lType, int number) {
	//Light position and direction
	Lpos = pos;
	Ldirection = dir;

	//Light intensity value
	Lambient = ambient;
	Ldiffuse = diffuse;
	Lspecular = specular;

	//Light type and number
	LightType = lType;
	lightNumber = number;

	switch (lType) {
	case DIRECTIONAL:
		break;
	case POINT:
		break;
	case SPOT:
		break;
	default:
		break;
	}
}


void Light::SetAtt(float constant, float lineal, float quadratic) {
	c1 = constant, c2 = lineal, c3 = quadratic;
}

void Light::SetAperture(float min, float max) {
	MinAperture = min;
	MaxAperture = max;
}

void Light::SetLight(Shader *shad, vec3 CamPos) {
	std::string variable;
	vec3 result;
	glUniform3f(glGetUniformLocation(shad->Program, "viewPos"), CamPos.x, CamPos.y, CamPos.z);
	switch (LightType){
	case DIRECTIONAL:
		//Light Direction
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.direction"), Ldirection.x, Ldirection.y, Ldirection.z);

		//Ambiental light
		result = Lambient * Lcolor;
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.ambient"), result.x, result.y, result.z);
		result = vec3(0.0);

		//Diffuse light
		result = Ldiffuse * Lcolor;
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.diffuse"), result.x, result.y, result.z);
		result = vec3(0.0);

		//Specular light
		result = Lspecular * Lcolor;
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.specular"), result.x, result.y, result.z);
		result = vec3(0.0);

		break;
	case POINT:		
		variable = "plight[" + std::to_string(lightNumber) + "]";

		//Light Position
		glUniform3f(glGetUniformLocation(shad->Program, std::string(variable + ".ambient").c_str()), Lpos.x, Lpos.y, Lpos.z);

		//Ambiental light
		result = Lambient * Lcolor;
		glUniform3f(glGetUniformLocation(shad->Program, std::string(variable + ".ambient").c_str()), result.x, result.y, result.z);
		result = vec3(0.0);

		//Diffuse light
		result = Ldiffuse * Lcolor;
		glUniform3f(glGetUniformLocation(shad->Program, std::string(variable + ".diffuse").c_str()), result.x, result.y, result.z);
		result = vec3(0.0);

		//Specular light
		result = Lspecular * Lcolor;
		glUniform3f(glGetUniformLocation(shad->Program, std::string(variable + ".specular").c_str()), result.x, result.y, result.z);
		result = vec3(0.0);

		//Constant, linear and quadratic attenuation
		glUniform1f(glGetUniformLocation(shad->Program, std::string(variable + ".constant").c_str()), c1);
		glUniform1f(glGetUniformLocation(shad->Program, std::string(variable + ".linear").c_str()), c2);
		glUniform1f(glGetUniformLocation(shad->Program, std::string(variable + ".quadratic").c_str()), c3);

		break;
	case SPOT:
		variable = "slight[" + std::to_string(lightNumber) + "]";

		//Light Position
		glUniform3f(glGetUniformLocation(shad->Program, std::string(variable + ".position").c_str()), Lpos.x, Lpos.y, Lpos.z);

		//Light Direction
		glUniform3f(glGetUniformLocation(shad->Program, std::string(variable + ".direction").c_str()), Lpos.x, Lpos.y, Lpos.z);

		//Ambiental light
		result = Lambient * Lcolor;
		glUniform3f(glGetUniformLocation(shad->Program, std::string(variable + ".ambient").c_str()), result.x, result.y, result.z);
		result = vec3(0.0);

		//Diffuse light
		result = Ldiffuse * Lcolor;
		glUniform3f(glGetUniformLocation(shad->Program, std::string(variable + ".diffuse").c_str()), result.x, result.y, result.z);
		result = vec3(0.0);

		//Specular light
		result = Lspecular * Lcolor;
		glUniform3f(glGetUniformLocation(shad->Program, std::string(variable + ".specular").c_str()), result.x, result.y, result.z);
		result = vec3(0.0);

		//Min and max aperture
		glUniform1f(glGetUniformLocation(shad->Program, std::string(variable + ".minAperture").c_str()), MinAperture);
		glUniform1f(glGetUniformLocation(shad->Program, std::string(variable + ".maxAperture").c_str()), MaxAperture);

		//Constant, linear and quadratic attenuation
		glUniform1f(glGetUniformLocation(shad->Program, std::string(variable + ".constant").c_str()), c1);
		glUniform1f(glGetUniformLocation(shad->Program, std::string(variable + ".linear").c_str()), c2);
		glUniform1f(glGetUniformLocation(shad->Program, std::string(variable + ".quadratic").c_str()), c3);

		break;
	default:
		break;
	}
}

void Light::Rotate(vec3 rotation) {	
//opcional
}

void Light::SetDirection(vec3 dir) {
	Ldirection = dir;
}

void Light::SetPosition(vec3 pos) {
	Lpos = pos;
}

void Light::SetColor(vec3 color) {
	Lcolor = color;
}

vec3 Light::GetColor() {
	return Lcolor;
}