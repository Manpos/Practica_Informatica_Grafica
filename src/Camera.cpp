#include "Camera.h"

Camera::Camera() {
	cameraPos = vec3 (0.f, 0.f, 3.f);
	cameraFront = vec3 (0.f, 0.f, -1.f);
	cameraUp = vec3(0.0, 1.0, 0.0);
}

mat4 Camera::LookAt() {
	cameraDirection = normalize(cameraPos - cameraFront);
	rightVector = normalize(cross(cameraUp, cameraDirection));
	upVector = cross(cameraDirection, rightVector);

	mat4 cameraVectors(
		rightVector.x, upVector.x, cameraDirection.x, 0.f,
		rightVector.y, upVector.y, cameraDirection.y, 0.f,
		rightVector.z, upVector.z, cameraDirection.z, 0.f,
		0.f, 0.f, 0.f, 1.f);

	mat4 cameraPosition(
		1.f, 0.f, 0.f, 0,
		0.f, 1.f, 0.f, 0,
		0.f, 0.f, 1.f, 0,
		-cameraPos.x, -cameraPos.y, -cameraPos.z, 1.f);

	return cameraVectors * cameraPosition;
}