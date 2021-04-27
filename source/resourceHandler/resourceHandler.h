#pragma once

#include <map>
#include <string>
#include "../shaderHandler/shaderHandler.h"
#include "../textureHandler/textureHandler.h"

class ResourceHandler
{
public:

	static ResourceHandler& get();
	void clear();
	void loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, const std::string& name);
	ShaderHandler& getShader(const std::string& name);

	void loadTexture(const char* file, bool alpha, const std::string& name);
	TextureHandler& getTexture(const std::string& name);

	ResourceHandler(const ResourceHandler&&) = delete;
	ResourceHandler(ResourceHandler&&) = delete;

	ResourceHandler& operator=(const ResourceHandler&) = delete;
	ResourceHandler& operator=(ResourceHandler&) = delete;
protected:
	ResourceHandler() = default;
private:

	ShaderHandler loadShaderFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile);
	TextureHandler loadTextureFile(const char* file, bool alpha);

	std::map<std::string, ShaderHandler> m_shaders;
	std::map<std::string, TextureHandler> m_textures;
};

