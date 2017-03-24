#ifndef SHADER_HPP
#define SHADER_HPP

class Shader {

public:
	GLuint Program;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();

	void USE();
};

#endif
