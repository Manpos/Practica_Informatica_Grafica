#pragma once
#include <gl\glew.h>
#include<glm.hpp>
#include <gtc\matrix_transform.hpp>
using namespace glm;
#include "shader.hpp"

enum FigureType {
	cube = 0,
};

class Object{
	
public:

	Object(glm::vec3 scale, glm::vec3 rotation, glm::vec3 position, FigureType typef);
	~Object();

	void Start();
	void Draw();
	void Move(vec3 translation);
	void Rotate(vec3 rota);
	void Scale(vec3 scal);
	void Delete();
	mat4 GetModelMatrix();
	vec3 GetPosition();

private:
	GLuint VBO, VAO, EBO;
	vec3 position;
	vec3 objScale;
	vec3 rotation;		//Degrees
	FigureType figType;
};

