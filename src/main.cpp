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

#include "Model.h"
#include "Camera.h"
#include "Object.h"

using namespace glm;
using namespace std;

#pragma region Variables

const GLint WIDTH = 800, HEIGHT = 800;
bool WIDEFRAME = false;
float textOpacity = 0;
float deg = 0;
int degX = 0, degY = 0;
bool view2D = false;

//Camera vectors
vec3 cameraPos(0.f, 0.f, 3.f);
vec3 cameraFront(0.f,0.f,-1.f);
vec3 cameraUp(0.0, 1.0, 0.0);

//Camera variables
float cameraVelocity = 10.f;
float sensitivity = 0.04;
float FOV = 60;

//Camera object declaration
Camera mainCam(cameraPos, cameraPos + cameraFront, sensitivity, FOV);

//Object declaration
FigureType fig;
vec3 cubePosition(0.0, -1.75, 0.0);
vec3 cubeRotation(0.f);
vec3 cubeScale(2.f);
Object cubito(cubeScale, cubeRotation, cubePosition, fig);

//Model drawed
int currentModel = 1;

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

	if (key == GLFW_KEY_KP_2) {
		cubito.Rotate(vec3(1.f, 0.0, 0.0));
	}
	if (key == GLFW_KEY_KP_8) {
		cubito.Rotate(vec3(-1.f, 0.0, 0.0));
	}
	if (key == GLFW_KEY_KP_4) {
		cubito.Rotate(vec3(0.0, 1.f, 0.0));
	}
	if (key == GLFW_KEY_KP_6) {
		cubito.Rotate(vec3(0.0, -1.f, 0.0));
	}
	if (key == GLFW_KEY_I) {
		cubito.Move(vec3(0.f, 1.0, 0.0));
	}
	if (key == GLFW_KEY_K) {
		cubito.Move(vec3(0.f, -1.0, 0.0));
	}
	if (key == GLFW_KEY_J) {
		cubito.Move(vec3(-1.0, 0.f, 0.0));
	}
	if (key == GLFW_KEY_L) {
		cubito.Move(vec3(1.0, 0.f, 0.0));
	}
	if (key == GLFW_KEY_1) {
		currentModel = 1;
	}
	if (key == GLFW_KEY_2) {
		currentModel = 2;
	}
	if (key == GLFW_KEY_3) {
		currentModel = 3;
	}
	if (key == GLFW_KEY_4) {
		currentModel = 4;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	mainCam.MouseMove(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	mainCam.MouseScroll(window, xoffset, yoffset);
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

	//MOUSE INPUT ENABLING
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//set windows and viewport
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//SHADERS
	//Shader object("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");
	//Shader object("./src/textureVertexShader.vert", "./src/textureFragmentShader.frag");
	//Shader object("./src/vertexShader3D.txt", "./src/fragmentShader3D.txt");
	Shader object("./src/modelVertexShader.txt", "./src/modelFragmentShader.txt");
	//Shader object("./src/lightingVertexShader.txt", "./src/lightingFragmentShader.txt");

	//3D MODEL LOADING
	Model firstModel("./src/3DModel/spider.obj");
	Model secondModel("./src/3DModel/Tractor.3ds");
	Model thirdModel("./src/3DModel/nanosuit.obj");
	Model fourthModel("./src/3DModel/farmhouse_obj.obj");

//OWN VBO, VAO, EBO
#if(false)

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

#endif

//2D TEXTURE
#if(false)

	//2D Texture 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

#endif

//3D TEXTURE
#if(false)

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

#endif

	int width, height;

//CUBE TEXTURES
#if (false)
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

#endif
	
	//liberar el buffer
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Movement uniforms
	GLint variableShader = glGetUniformLocation(object.Program, "downOffset");

	//Texture uniforms
	GLint textureOpacity = glGetUniformLocation(object.Program, "opacity");
	GLint transformationMatrix = glGetUniformLocation(object.Program, "trans");

	//3D MATRIX
	GLint view3D = glGetUniformLocation(object.Program, "view");
	GLint projection = glGetUniformLocation(object.Program, "proj");
	GLint model3D = glGetUniformLocation(object.Program, "model");

	//ENABLE Z-BUFFER	
	glEnable(GL_DEPTH_TEST);


#pragma endregion


	while (!glfwWindowShouldClose(window)) {

		//Key used check
		glfwSetKeyCallback(window, key_callback);
		
		//MOUSE INPUT/CAMERA CONTROLS
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		mainCam.DoMovement(window);

		//Background clear
		//glClearColor(0.0, 0.0, 0.0, 1.0);
		//glClearColor(1.0, 1.0, 1.0, 1.0);
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//ACTIVATE IF VAO AVAILABLE
		//glBindVertexArray(VAO);

//TEXTURES
#if(false)

		//TEXTURES
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(object.Program, "texture1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(object.Program, "texture2"), 1);

		glUniform1f(textureOpacity, textOpacity);

#endif

		//RANDOM VERTEX TRANSFORM
		//glUniform1f(variableShader, 0.5 * abs(sin(glfwGetTime())));

#pragma region CameraLookAtMatrices

		//MATRIX MODIFICATION
		glm::mat4 translationMat;
		glm::mat4 scalationMat;
		glm::mat4 rotationMat;
		glm::mat4 transformationMat;

#pragma endregion

#if(false)

		//2D TRANSFORMATION
		scalationMat = glm::scale(scalationMat, glm::vec3(0.5, 0.5, 0));
		translationMat = glm::translate(translationMat, glm::vec3(0.5f, 0.5f, 0.f));
		rotationMat = glm::rotate(rotationMat, glm::radians(deg), glm::vec3(0.0f, 0.0f, 1.0f));

#endif

#if(false)

		//3D TRANSFORMATION
		rotationMat = glm::rotate(rotationMat, glm::radians(float(degX)), glm::vec3(1.0f, 0.0f, 0.0f));
		rotationMat = glm::rotate(rotationMat, glm::radians(float(degY)), glm::vec3(0.0f, 1.0f, 0.0f));

#endif

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
		object.USE();

		//PROJECTION MATRIX
		mat4 proj = perspective(radians(mainCam.GetFOV()), float(screenWidth) / float(screenHeight), 1.0f, 100.0f);
		glUniformMatrix4fv(projection, 1, GL_FALSE, value_ptr(proj));

		//VIEW MATRIX
		mat4 view = mainCam.LookAt();

		//view = lookAt(vec3(X,0.0,Z), direction, vec3(0.0f, 1.f, 0.f));
		//view = LookAtMatrix(vec3(X, 0.0f, Z), direction, vec3(0.0f, 1.f, 0.f));
		//view = lookAt(cameraPos, direction, vec3(0.0f, 1.f, 0.f));
		glUniformMatrix4fv(view3D, 1, GL_FALSE, value_ptr(view));

		//MODEL TRANSFORMATION MATRIX
		//transformationMat = translationMat * rotationMat * scalationMat;
		
		mat4 model;

		switch (currentModel)
		{
		case 1:
			model = scale(model, vec3(0.1));
			firstModel.Draw(object, GL_FILL);
			break;
		case 2:
			model = rotate(model, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
			model = scale(model, vec3(0.1));
			secondModel.Draw(object, GL_FILL);
			break;
		case 3:
			model = scale(model, vec3(1.0));
			thirdModel.Draw(object, GL_FILL);
			break;
		case 4:
			model = scale(model, vec3(0.3));
			fourthModel.Draw(object, GL_FILL);
			break;
		}
		
		glUniformMatrix4fv(model3D, 1, GL_FALSE, value_ptr(model));

#if(false)

		//RANDOM CAMERA MOVEMENT
		GLfloat rad = 8.0f;
		GLfloat X = sin(glfwGetTime()) * rad;
		GLfloat Z = cos(glfwGetTime()) * rad;

#endif

//CUBE RENDERING		
#if(false)

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


#endif

//MODEL LOADING
#if(false)
		//firstModel.Draw(object, GL_FILL);
		secondModel.Draw(object, GL_FILL);
#endif

		//glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Liberar la memoria de los VAO, EBO y VBO
#if(false)
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
#endif
	// Terminate GLFW, clearing any resources allocated by GLFW.
	exit(EXIT_SUCCESS);
}