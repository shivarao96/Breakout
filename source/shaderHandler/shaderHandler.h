#pragma once

#include<glm/glm.hpp>

class ShaderHandler
{
public:
	void compileShader(const char* vertexSource, const char* fragmentSource, const char* geometricSource);
	void useShader();
	ShaderHandler& getShader();
	void setInt(const char* paramName, int value, bool useShader = false);
	void setFloat(const char* paramName, float value, bool useShader = false);
	void setVector2f(const char* paramName, const glm::vec2& value, bool useShader = false);
	void setVector3f(const char* paramName, const glm::vec3& value, bool useShader = false);
	void setVector4f(const char* paramName, const glm::vec4& value, bool useShader = false);
	void setMatrix4(const char* paramName, const glm::mat4& value, bool useShader = false);
	void clear();

	unsigned int getShaderId();
	unsigned int m_shaderId;
private:
};

