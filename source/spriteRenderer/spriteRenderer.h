#pragma once

#include <glm/glm.hpp>

#include "../shaderHandler/shaderHandler.h"
#include "../textureHandler/textureHandler.h"

class SpriteRenderer
{
public:
	SpriteRenderer(ShaderHandler& shader);
	~SpriteRenderer();
	void drawSprite(TextureHandler& texHandler, const glm::vec2& pos, const glm::vec2& size = glm::vec2(10, 10), float rotate = 0.0f, const glm::vec3& color = glm::vec3(1.0f));
private:
	void initRenderData();
	unsigned int m_quadVAO;
	ShaderHandler* m_pShaderHandler = nullptr;
};

