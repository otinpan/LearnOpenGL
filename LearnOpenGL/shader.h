#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
	void setMatrix4(const std::string& name,glm::mat4 matrix)const;
	void setMatrix3(const std::string& name, glm::mat3 matrix)const;
	void setVec3(const std::string& name, glm::vec3 vec)const;

private:
	unsigned int ID;

};

#endif
