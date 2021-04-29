#include "resourceHandler.h"
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>
#include <sstream>
#include <fstream>

ResourceHandler& ResourceHandler::get()
{
	static ResourceHandler rh;
	return rh;
}

void ResourceHandler::clear()
{
	for (auto& tex : m_textures) {
		tex.second.clear();
	}
	m_textures.clear();

	for (auto& shader : m_shaders) {
		shader.second.clear();
	}
	m_shaders.clear();
}

void ResourceHandler::loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, const std::string& name)
{
	m_shaders[name] = loadShaderFile(vShaderFile, fShaderFile, gShaderFile);
}

ShaderHandler& ResourceHandler::getShader(const std::string& name)
{
	return m_shaders[name];
}

void ResourceHandler::loadTexture(const char* file, bool alpha, const std::string& name)
{
	m_textures[name] = loadTextureFile(file, alpha);
}

TextureHandler& ResourceHandler::getTexture(const std::string& name)
{
	return m_textures[name];
}

ShaderHandler ResourceHandler::loadShaderFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
	std::string vertexCode, fragmentCode, geometricCode;
	try {
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vertexCodeStream, fragmentCodeStream;
		vertexCodeStream << vertexShaderFile.rdbuf();
		fragmentCodeStream << fragmentShaderFile.rdbuf();
		vertexShaderFile.close();
		fragmentShaderFile.close();
		vertexCode = vertexCodeStream.str();
		fragmentCode = fragmentCodeStream.str();
		if (gShaderFile != NULL) {
			std::ifstream geometricShaderFile(gShaderFile);
			std::stringstream geometricShaderStream;
			geometricShaderStream << geometricShaderFile.rdbuf();
			geometricShaderStream << geometricShaderFile.rdbuf();
			geometricShaderStream << geometricShaderFile.rdbuf();
			geometricCode = geometricShaderStream.str();
		}
	}
	catch (std::exception e) {
		std::cout << "ERROR::SHADER:: Failed to read shader file." << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometricCode.c_str();

	ShaderHandler shader;
	shader.compileShader(vShaderCode, fShaderCode, gShaderFile != NULL ? gShaderCode : nullptr);

	return shader;
}

TextureHandler ResourceHandler::loadTextureFile(const char* file, bool alpha)
{
	TextureHandler tex2D;
	if (alpha) {
		tex2D.setInternalFormat(GL_RGBA);
		tex2D.setImageFormat(GL_RGBA);
	}
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	tex2D.generateTex(width, height, data);
	stbi_image_free(data);

	return tex2D;
}
 