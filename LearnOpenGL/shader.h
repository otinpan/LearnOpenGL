#pragma once
#include<string>
#include<glad/glad.h>

class Shader {
public:
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader(Shader&&) noexcept = default;
	Shader& operator=(Shader&&) noexcept = default;

	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	unsigned int const GetShaderProgram() { return shaderProgram; } 
	void use() const;

private:
	unsigned int shaderProgram;

};

