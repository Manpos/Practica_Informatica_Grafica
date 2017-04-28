//void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
//
//	if (firstMouse)
//	{
//		lastMousePosition.x = xpos;
//		lastMousePosition.y = ypos;
//		firstMouse = false;
//	}
//
//	cout << xpos << endl;
//
//	vec2 offset;
//	vec3 result;
//
//	offset.x = xpos - lastMousePosition.x;
//	offset.y = lastMousePosition.y - ypos;
//
//	lastMousePosition.x = xpos;
//	lastMousePosition.y = ypos;
//
//	offset *= sensitivity;
//
//	camYaw += offset.x;
//	camPitch += offset.y;
//
//	camPitch = clamp(camPitch, -89.f, 89.f);
//	camYaw = mod(camYaw, 360.f);
//
//	result.x = cos(radians(camPitch)) * cos(radians(camYaw));
//	result.y = sin(radians(camPitch));
//	result.z = cos(radians(camPitch))* sin(radians(camYaw));
//
//	cameraFront = normalize(result);
//}
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	if (FOV >= 1.0f && FOV <= 100)
//		FOV -= yoffset;
//	if (FOV <= 1.0f)
//		FOV = 1.0f;
//	if (FOV >= 100)
//		FOV = 100;
//}

////CAMERA MOVEMENT
//void DoMovement(GLFWwindow* window) {
//	
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
//		//cameraPos.z -= cameraVelocity * deltaTime;
//		cameraPos += cameraFront * cameraVelocity * deltaTime;
//	}
//
//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
//		//cameraPos.z += cameraVelocity * deltaTime;
//		cameraPos -= cameraFront * cameraVelocity * deltaTime;
//	}
//
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
//		cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraVelocity * deltaTime;
//	}
//
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
//		cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraVelocity * deltaTime;
//	}
//}

//mat4 LookAtMatrix(vec3 position, vec3 viewDirection, vec3 worldUp) {
//	vec3 cameraDirection = normalize(position - viewDirection);
//	vec3 rightVector = normalize(cross(worldUp, cameraDirection));
//	vec3 upVector = cross(cameraDirection, rightVector);
//
//	mat4 cameraVectors(
//		rightVector.x, upVector.x, cameraDirection.x, 0.f,
//		rightVector.y, upVector.y, cameraDirection.y, 0.f,
//		rightVector.z, upVector.z, cameraDirection.z, 0.f,
//		0.f, 0.f, 0.f, 1.f);	
//
//	mat4 cameraPosition (
//		1.f, 0.f, 0.f, 0, 
//		0.f, 1.f, 0.f, 0, 
//		0.f, 0.f, 1.f, 0, 
//		-position.x, -position.y, -position.z, 1.f);
//
//	return cameraVectors * cameraPosition;
//}