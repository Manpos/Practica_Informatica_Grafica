#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <glm.hpp>
#include "shader.hpp"
#include<glm.hpp>
#include <gtc\matrix_transform.hpp>
using namespace glm;

class Light {
public:
	enum LType {
		DIRECTIONAL = 0,
		POINT = 1,
		SPOT = 2
	};
	Light(vec3 pos, vec3 dir, vec3 ambient, vec3 diffuse, vec3 specular, LType lType, int number);
	void SetAtt(float constant, float lineal, float quadratic);
	void SetLight(Shader *shad, vec3 CamPos);
	void Rotate(vec3 rotation);
	void SetDirection(vec3 dir);
	void SetAperture(float min, float max);
	vec3 GetPosition();
	void SetPosition(vec3 pos);
	void SetColor(vec3 color);
	vec3 GetColor();

private:
	vec3 Lcolor = vec3(1.0);
	vec3 Ldirection;
	vec3 Lpos;
	vec3 Lambient;
	vec3 Ldiffuse;
	vec3 Lspecular;
	LType LightType;
	float MinAperture;
	float MaxAperture;
	float c1, c2, c3;
	int lightNumber;
};

