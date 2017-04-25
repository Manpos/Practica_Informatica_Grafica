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
	vec3 upVector;
	vec3 rightVector;
	vec3 cameraDirection;

	//Camera variables
	float FOV = 60;
	float camYaw = 0, camPitch = 0;
	//vec2 lastMousePosition = vec2(WIDTH / 2, HEIGHT / 2);
	bool firstMouse = true;
	float cameraVelocity = 10.f;

	//Delta Time
	float deltaTime = 0, actualTime = 0, lastFrame = 0;

public:
	Camera();
	~Camera();
	void Update();
	void DoMovement(GLFWwindow* window);
	void MouseMove(GLFWwindow* window, double xpos, double ypos);
	void MouseScroll(GLFWwindow* window, double xoffset, double yoffset);
	mat4 LookAt();
	GLfloat GetFOV();

};