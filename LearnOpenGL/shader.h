#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>


class Shader {
public:

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader(Shader&&) noexcept = default;
	Shader& operator=(Shader&&) noexcept = default;

	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	unsigned int const getShaderProgram() { return ID; } 
	void use() const;

	void setBool(const std::string& name, bool value)const;
	void setInt(const std::string& name, int value)const;
	void setFloat(const std::string& name, float value)const;

private:
	unsigned int ID;

};

#endif
