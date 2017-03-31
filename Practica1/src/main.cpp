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
const GLint WIDTH = 800, HEIGHT = 800;
bool WIDEFRAME = false;
float textOpacity = 0;
float deg = 0;
float FOV = 60;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//cuando se pulsa una tecla escape cerramos la aplicacion
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		WIDEFRAME = !WIDEFRAME;
	}
	if (key == GLFW_KEY_UP) {
		if (textOpacity < 1) {
			textOpacity += 0.1;
		}
		else textOpacity = 1;
	}
	if (key == GLFW_KEY_DOWN) {
		if (textOpacity > 0) {
			textOpacity -= 0.1;
		}
		else textOpacity = 0;
	}
	if (key == GLFW_KEY_RIGHT) {
		deg -= 5.f;
	}
	if (key == GLFW_KEY_LEFT) {
		deg += 5.f;
	}
}

int main() {
	//initGLFW
	//TODO
	GLFWwindow* window;
	if (!glfwInit())
		exit(EXIT_FAILURE);
	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//create a window
	//TODO
	window = glfwCreateWindow(WIDTH, HEIGHT, "Finestra del projecte", nullptr, nullptr);

	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	//set GLEW and inicializate
	//TODO
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		cout << "Error al iniciar glew" << endl;
		glfwTerminate();
		return NULL;
	}
	//set function when callback
	//TODO
	glfwSetKeyCallback(window, key_callback);
	//set windows and viewport
	//TODO
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	//fondo
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//TODO


	///////////// ----- SHADERS ----- /////////////
	//Shader object("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");
	//Shader object("./src/textureVertexShader.vert", "./src/textureFragmentShader.frag");
	Shader object("./src/vertexShader3D.txt", "./src/fragmentShader3D.txt");


	// Definir el buffer de vertices

	GLfloat VertexBufferObject[] = {
		0.5f,  0.5f, 0.0f,		0.f, 1.f, 1.f,		1.0f, 0.0f, // Top Right
		0.5f, -0.5f, 0.0f,		0.f, 1.f, 0.f,		1.0f, 1.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,		0.f, 1.f, 0.f,		0.0f, 1.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,		0.f, 1.f, 1.f,		0.0f, 0.0f  // Top Left 

	};

	// Definir el EBO
	GLuint index[]{
		0, 1, 2,
		0, 2, 3
	};

	/*glm::mat4 trans;
	glm::vec4 v1(1, 1, 0, 1);
	cout << v1.x << " " << v1.y << " " << v1.z << endl;
	trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0));
	v1 = trans * v1;
	cout << v1.x << " " << v1.y << " " << v1.z << endl;*/

	// Crear los VBO, VAO y EBO
	GLuint VBO, VAO, EBO;

	//reservar memoria para el VAO, VBO y EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Establecer el objeto
	//Declarar el VBO y el EBO
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;

	unsigned char* image = SOIL_load_image("./src/texture.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	image = SOIL_load_image("./src/texture2.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Enlazar el buffer con openGL
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferObject), VertexBufferObject, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	//Establecer las propiedades de los vertices

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//liberar el buffer
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//liberar el buffer de vertices

	GLint variableShader = glGetUniformLocation(object.Program, "downOffset");
	GLint textureOpacity = glGetUniformLocation(object.Program, "opacity");
	GLint transformationMatrix = glGetUniformLocation(object.Program, "trans");

	GLuint vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	mat4 proj = perspective(radians(FOV), float(screenWidth) / float(screenHeight), 0.0f, -10.0f);

	//bucle de dibujado
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwSetKeyCallback(window, key_callback);

		//Establecer el color de fondo
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0, 1.0, 1.0, 1.0);

	/////////////----- TEXTURES -----/////////////
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(object.Program, "texture1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(object.Program, "texture2"), 1);
		
		glUniform1f(textureOpacity, textOpacity);

	/////////////----- VERTEX TRANSFORM -----/////////////
		glUniform1f(variableShader, 0.5 * abs(sin(glfwGetTime())));

	/////////////----- MATRIX MODIFICATION -----/////////////
	//Transformation matrices
		glm::mat4 translationMat;
		glm::mat4 scalationMat;
		glm::mat4 rotationMat;
		glm::mat4 transformationMat;
		scalationMat = glm::scale(scalationMat, glm::vec3(0.5, 0.5, 0));
		translationMat = glm::translate(translationMat, glm::vec3(0.5f, 0.5f, 0.f));
		rotationMat = glm::rotate(rotationMat, glm::radians(deg), glm::vec3(0.0f, 0.0f, 1.0f));

		//transformationMat = scalationMat * translationMat;
		transformationMat = translationMat * rotationMat * scalationMat;

		glUniformMatrix4fv(transformationMatrix, 1, GL_FALSE, value_ptr(transformationMat));

		object.USE();

		glBindVertexArray(VAO);

		if (WIDEFRAME) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		//pintar con triangulos
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

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