#include "shaderHandler.h"
#include <glad/glad.h>
#include <iostream>

void ShaderHandler::compileShader(const char* vertexSource, const char* fragmentSource, const char* geometricSource)
{
	auto checkCompileError = [&](unsigned int object, const char* type) {
		int success;
		char infoLog[1024];
		if (type == "PROGRAM") {
			glGetProgramiv(object, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(object, 1024, NULL, infoLog);
				std::cout << "ERROR::LINKING SHADER PROGRAM\n" << type << "::" << infoLog << std::endl;
			}
		}
		else {
			glGetShaderiv(object, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(object, 1024, NULL, infoLog);
				std::cout << "ERROR::COMPILING SHADER PROGRAM\n" << type << "::" << infoLog << std::endl;
			}
		}
	};

	unsigned int vertexShaderProgram, fragmentShaderProgram, geomtricShaderProgram;
	// vertex shader
	vertexShaderProgram = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderProgram, 1, &vertexSource, NULL);
	glCompileShader(vertexShaderProgram);
	checkCompileError(vertexShaderProgram, "VERTEX");
	// fragment shader
	fragmentShaderProgram = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderProgram, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShaderProgram);
	checkCompileError(fragmentShaderProgram, "FRAGMENT");
	// geometric shader
	if (geometricSource != NULL) {
		geomtricShaderProgram = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geomtricShaderProgram, 1, &geometricSource, NULL);
		glCompileShader(geomtricShaderProgram);
		std::cout << "This should not get triggered now !" << std::endl;
		checkCompileError(geomtricShaderProgram, "GEOMETRIC");
	}
	
	m_shaderId = glCreateProgram();
	glAttachShader(m_shaderId, vertexShaderProgram);
	glAttachShader(m_shaderId, fragmentShaderProgram);
	if (geometricSource != NULL)
		glAttachShader(m_shaderId, geomtricShaderProgram);
	glLinkProgram(m_shaderId);
	checkCompileError(m_shaderId, "PROGRAM");

	glDeleteShader(vertexShaderProgram);
	glDeleteShader(fragmentShaderProgram);
	if (geometricSource != NULL)
		glDeleteShader(geomtricShaderProgram);
}

void ShaderHandler::useShader()
{
	glUseProgram(m_shaderId);
}


ShaderHandler& ShaderHandler::getShader()
{
	return *this;
}

void ShaderHandler::setInt(const char* paramName, int value, bool useShader)
{
	if (useShader)
		this->useShader();
	glUniform1i(glGetUniformLocation(m_shaderId, paramName), value);
}

void ShaderHandler::setFloat(const char* paramName, float value, bool useShader)
{
	if (useShader)
		this->useShader();
	glUniform1f(glGetUniformLocation(m_shaderId, paramName), value);
}

void ShaderHandler::setVector2f(const char* paramName, const glm::vec2& value, bool useShader)
{
	if (useShader)
		this->useShader();
	glUniform2f(glGetUniformLocation(m_shaderId, paramName), value.x, value.y);
}

void ShaderHandler::setVector3f(const char* paramName, const glm::vec3& value, bool useShader)
{
	if (useShader)
		this->useShader();
	glUniform3f(glGetUniformLocation(m_shaderId, paramName), value.x, value.y, value.z);
}

void ShaderHandler::setVector4f(const char* paramName, const glm::vec4& value, bool useShader)
{
	if (useShader)
		this->useShader();
	glUniform4f(glGetUniformLocation(m_shaderId, paramName), value.x, value.y, value.z, value.w);
}

void ShaderHandler::setMatrix4(const char* paramName, const glm::mat4& value, bool useShader)
{
	if (useShader)
		this->useShader();
	glUniformMatrix4fv(glGetUniformLocation(m_shaderId, paramName), 1, false, &value[0][0]); // here problem might come
}

void ShaderHandler::clear()
{
	glDeleteProgram(m_shaderId);
}

unsigned int ShaderHandler::getShaderId()
{
	return m_shaderId;
}
