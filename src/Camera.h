#pragma once
#include <glm.hpp>
#include <GLFW\glfw3.h>

using namespace glm;

class Camera {
private:
	//Camera vectors
	vec3 cameraPos;
	vec3 cameraFront;
	vec3 cameraUp;

	//Camera variables
	GLfloat FOV = 60;
	GLfloat camYaw = 0, camPitch = 0;
	vec2 lastMousePosition = vec2(800 / 2, 800 / 2);
	GLboolean firstMouse = true;
	GLfloat cameraVelocity = 10.f;
	GLfloat sensitivity = 0.04;

	//Delta Time
	GLfloat deltaTime = 0, actualTime = 0, lastFrame = 0;

public:
	Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov);
	void DoMovement(GLFWwindow* window);
	void MouseMove(GLFWwindow* window, double xpos, double ypos);
	void MouseScroll(GLFWwindow* window, double xoffset, double yoffset);
	vec3 GetPosition();
	mat4 LookAt();
	GLfloat GetFOV();

};