//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include <shader.hpp>

#include <SOIL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

#pragma region Variables

const GLint WIDTH = 800, HEIGHT = 800;
bool WIDEFRAME = false;
float textOpacity = 0;
float deg = 0;
int degX = 0, degY = 0;
float FOV = 60;
bool view2D = false;

//Camera vectors
vec3 cameraPos(0.f, 0.f, 3.f);
vec3 direction(0.f,0.f,0.f);
vec3 upVector;
vec3 rightVector;
vec3 cameraDirection;

//Camera variables
double posX, posY;
float camYaw = 0, camPitch = 0;
vec2 offset;
vec2 mousePosition, lastMousePosition;
float sensibility = 0.04;

float cameraVelocity = 10.f;

//Delta Time
float deltaTime = 0, actualTime = 0, lastFrame = 0;

#pragma endregion

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//cuando se pulsa una tecla escape cerramos la aplicacion
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		WIDEFRAME = !WIDEFRAME;
	}
	if (key == GLFW_KEY_1) {
		if (textOpacity < 1) {
			textOpacity += 0.1;
		}
		else textOpacity = 1;
	}
	if (key == GLFW_KEY_2) {
		if (textOpacity > 0) {
			textOpacity -= 0.1;
		}
		else textOpacity = 0;
	}
	if (key == GLFW_KEY_RIGHT) {
		degY -= 5;
		degY = degY % 360;
	}
	if (key == GLFW_KEY_LEFT) {
		degY += 5;
		degY = degY % 360;
	}
	if (key == GLFW_KEY_UP) {
		degX -= 5;
		degX = degX % 360;
	}
	if (key == GLFW_KEY_DOWN) {
		degX += 5;
		degX = degX % 360;
	}

}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

//CAMERA MOVEMENT
void DoMovement(GLFWwindow* window) {

	//MOUSE INPUT
	glfwSetCursorPosCallback(window, cursor_position_callback);
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//cameraPos.z -= cameraVelocity * deltaTime;
		cameraPos += cameraDirection * cameraVelocity * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//cameraPos.z += cameraVelocity * deltaTime;
		cameraPos -= cameraDirection * cameraVelocity * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += rightVector * cameraVelocity * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= rightVector * cameraVelocity * deltaTime;
	}
}

mat4 LookAtMatrix(vec3 position, vec3 viewDirection, vec3 worldUp) {
	cameraDirection = normalize(viewDirection - position);
	rightVector = normalize(cross(cameraDirection, worldUp));
	upVector = cross(rightVector, cameraDirection);

	mat4 cameraVectors(
		rightVector.x, upVector.x, -cameraDirection.x, 0.f,
		rightVector.y, upVector.y, -cameraDirection.y, 0.f,
		rightVector.z, upVector.z, -cameraDirection.z, 0.f,
		0.f, 0.f, 0.f, 1.f);	

	mat4 cameraPosition (
		1.f, 0.f, 0.f, 0, 
		0.f, 1.f, 0.f, 0, 
		0.f, 0.f, 1.f, 0, 
		-position.x, -position.y, -position.z, 1.f);

	return cameraVectors * cameraPosition;
}

vec3 cameraRotation(float camyaw, float campitch, vec3 cameraPosition) {
	vec3 result;

	float YAW = mod(camyaw, 360.f);
	float PITCH = clamp(campitch, -89.f, 89.f);

	result.x = cos(radians(YAW)) * cos(radians(PITCH));
	result.y = sin(radians(PITCH));
	result.z = cos(radians(YAW))* sin(radians(PITCH));

	return result;
}

int main() {

#pragma region OpenGL starters

	GLFWwindow* window;
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//create a window
	window = glfwCreateWindow(WIDTH, HEIGHT, "Finestra del projecte", nullptr, nullptr);

	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	//set GLEW and inicializate
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		cout << "Error al iniciar glew" << endl;
		glfwTerminate();
		return NULL;
	}

	//set function when callback
	glfwSetKeyCallback(window, key_callback);

	//set windows and viewport
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//SHADERS
	//Shader object("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");
	//Shader object("./src/textureVertexShader.vert", "./src/textureFragmentShader.frag");
	Shader object("./src/vertexShader3D.txt", "./src/fragmentShader3D.txt");


	//VBO

	//Plane vertices
	GLfloat VertexBufferObject[] = {
		0.5f,  0.5f, 0.0f,		0.f, 1.f, 1.f,		1.0f, 0.0f, // Top Right
		0.5f, -0.5f, 0.0f,		0.f, 1.f, 0.f,		1.0f, 1.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,		0.f, 1.f, 0.f,		0.0f, 1.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,		0.f, 1.f, 1.f,		0.0f, 0.0f  // Top Left 

	};

	//Cube vertices
	GLfloat VertexBufferCube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	vec3 CubesPositionBuffer[] = {
		vec3(0.0f ,  0.0f,  0.0f),
		vec3(2.0f ,  5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f , -0.4f, -3.5f),
		vec3(-1.7f,  3.0f, -7.5f),
		vec3(1.3f , -2.0f, -2.5f),
		vec3(1.5f ,  2.0f, -2.5f),
		vec3(1.5f ,  0.2f, -1.5f),
		vec3(-1.3f,  1.0f, -1.5f)
	};

	//EBO
	GLuint index[]{
		0, 1, 2,
		0, 2, 3
	};

	// Crear los VBO, VAO y EBO
	GLuint VBO, VAO, EBO;

	//reservar memoria para el VAO, VBO y EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Enlazar el buffer con openGL
	glBindVertexArray(VAO);

	//VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferObject), VertexBufferObject, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferCube), VertexBufferCube, GL_STATIC_DRAW);

	//EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

#if(false)

	//2D Texture 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

#endif

#if(true)

	//3D Texture
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

#endif

	int width, height;

	GLuint texture, texture2;

	glGenTextures(1, &texture);
	glGenTextures(1, &texture2);

	//Texture 1
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	unsigned char* image = SOIL_load_image("./src/texture.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glUniform1i(glGetUniformLocation(object.Program, "texture1"), 0);

	//Texture 2
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	image = SOIL_load_image("./src/texture2.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	//liberar el buffer
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Movement uniforms
	GLint variableShader = glGetUniformLocation(object.Program, "downOffset");

	//Texture uniforms
	GLint textureOpacity = glGetUniformLocation(object.Program, "opacity");
	GLint transformationMatrix = glGetUniformLocation(object.Program, "trans");

	//Going 3D
	GLint view3D = glGetUniformLocation(object.Program, "view");
	GLint projection = glGetUniformLocation(object.Program, "proj");
	GLint model3D = glGetUniformLocation(object.Program, "model");

	//ENABLE Z-BUFFER	
	glEnable(GL_DEPTH_TEST);

	//CAMERA POSITIONING
	mat4 proj = perspective(radians(FOV), float(screenWidth) / float(screenHeight), 1.0f, 100.0f);
	//mat4 view = lookAt(cameraPos, direction, vec3(0.0f, 1.f, 0.f));
	//mat4 view = LookAtMatrix(cameraPos, direction, vec3(0.0f, 1.f, 0.f));

	//INITIAL CAMERA ROTATION
	lastMousePosition = mousePosition = vec2(WIDTH, HEIGHT/2);


#pragma endregion

	while (!glfwWindowShouldClose(window)) {
		//Key used check
		glfwSetKeyCallback(window, key_callback);

		DoMovement(window);

		//DELTA TIME
		actualTime = glfwGetTime();
		deltaTime = actualTime - lastFrame;

		//cout << deltaTime << endl;

		//Background clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);

		glBindVertexArray(VAO);

		//TEXTURES
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(object.Program, "texture1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(object.Program, "texture2"), 1);

		glUniform1f(textureOpacity, textOpacity);

		//VERTEX TRANSFORM
		glUniform1f(variableShader, 0.5 * abs(sin(glfwGetTime())));

		//MATRIX MODIFICATION
		glm::mat4 translationMat;
		glm::mat4 scalationMat;
		glm::mat4 rotationMat;
		glm::mat4 transformationMat;

#if(false)

		//2D TRANSFORMATION
		scalationMat = glm::scale(scalationMat, glm::vec3(0.5, 0.5, 0));
		translationMat = glm::translate(translationMat, glm::vec3(0.5f, 0.5f, 0.f));
		rotationMat = glm::rotate(rotationMat, glm::radians(deg), glm::vec3(0.0f, 0.0f, 1.0f));

#endif

#if(true)

		//3D TRANSFORMATION
		//rotationMat = glm::rotate(rotationMat, glm::radians(float(degX)), glm::vec3(1.0f, 0.0f, 0.0f));
		//rotationMat = glm::rotate(rotationMat, glm::radians(float(degY)), glm::vec3(0.0f, 1.0f, 0.0f));

#endif

		transformationMat = translationMat * rotationMat * scalationMat;
		//mat4 model = transformationMat;

		glUniformMatrix4fv(transformationMatrix, 1, GL_FALSE, value_ptr(transformationMat));

#if(false)

		//PLANE RENDERING

		if (WIDEFRAME) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

#endif

		//CAMERA MOVEMENT
		//GLfloat rad = 8.0f;
		//GLfloat X = sin(glfwGetTime()) * rad;
		//GLfloat Z = cos(glfwGetTime()) * rad;

		mat4 view;

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		
		glfwSetCursorPosCallback(window, cursor_position_callback);
		glfwGetCursorPos(window, &posX, &posY);

		mousePosition.x = posX;
		mousePosition.y = posY;

		offset = mousePosition - lastMousePosition;
		offset *= sensibility;

		/*if (offset.x != 0 || offset.y != 0) {
			cout << "Mouse position: " << mousePosition.x << " | " << mousePosition.y << " ||| Last mouse position: " << lastMousePosition.x << " | " << lastMousePosition.y << endl;
			cout << offset.x << " | " << offset.y << endl;
		}*/

		lastMousePosition = mousePosition;
				
		camYaw += offset.x;
		camPitch += offset.y;
		
		//direction = cameraPos + vec3(0, 0, -1);
		//direction = length(direction) * (normalize(cameraPos) + normalize(cameraRotation(camYaw, camPitch, cameraPos)));
		direction =  length(direction) * normalize(cameraRotation(camYaw, camPitch, cameraPos));
		//cameraDirection = normalize(direction - cameraPos);

		//view = lookAt(vec3(X,0.0,Z), direction, vec3(0.0f, 1.f, 0.f));
		//view = LookAtMatrix(vec3(X, 0.0f, Z), direction, vec3(0.0f, 1.f, 0.f));
		//view = lookAt(cameraPos, direction, vec3(0.0f, 1.f, 0.f));
		view = LookAtMatrix(cameraPos, direction, vec3(0.0f, 1.f, 0.f));
		direction = cameraDirection;

#if(true)

		//CUBE RENDERING		

		for (int i = 0; i < sizeof(CubesPositionBuffer) / sizeof(vec3); ++i) {
			mat4 model;
			if (i == 0) {

				//3D TRANSFORMATION
				rotationMat = glm::rotate(rotationMat, glm::radians(float(degX)), glm::vec3(1.0f, 0.0f, 0.0f));
				rotationMat = glm::rotate(rotationMat, glm::radians(float(degY)), glm::vec3(0.0f, 1.0f, 0.0f));

				model = translate(model, CubesPositionBuffer[i]) * rotationMat;
				glUniformMatrix4fv(model3D, 1, GL_FALSE, value_ptr(model));
			}
			else {

				model = translate(model, CubesPositionBuffer[i]) * rotate(model, float(glfwGetTime()) * radians(180.f), vec3(0.0f, 0.0f, 1.0f)) * rotate(model, float(glfwGetTime()) * radians(180.f), vec3(1.0f, 0.0f, 0.0f));
				glUniformMatrix4fv(model3D, 1, GL_FALSE, value_ptr(model));
			}

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}

		glUniformMatrix4fv(projection, 1, GL_FALSE, value_ptr(proj));
		glUniformMatrix4fv(view3D, 1, GL_FALSE, value_ptr(view));

		

#endif

		//UPDATE DELTA TIME
		lastFrame = actualTime;

		object.USE();

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// liberar la memoria de los VAO, EBO y VBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	exit(EXIT_SUCCESS);
}