#include "spriteRenderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer(ShaderHandler& shader):m_pShaderHandler(&shader)
{
	initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &m_quadVAO);
}

void SpriteRenderer::drawSprite(TextureHandler& texHandler, const glm::vec2& pos, const glm::vec2& size, float rotate, const glm::vec3& color)
{
	m_pShaderHandler->useShader();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(pos, 0.0f));

	model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	model = glm::scale(model, glm::vec3(size, 1.0f));

	m_pShaderHandler->setMatrix4("model", model);
	m_pShaderHandler->setVector3f("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texHandler.bindTex();
	
	glBindVertexArray(m_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
	unsigned int VBO;
	float vertices[] = {
		0.0f, 1.0f,   0.0f, 1.0f,
		1.0f, 0.0f,   1.0f, 0.0f,
		0.0f, 0.0f,   0.0f, 0.0f,

		0.0f, 1.0f,   0.0f, 1.0f,
		1.0f, 1.0f,   1.0f, 1.0f,
		1.0f, 0.0f,   1.0f, 0.0f
	};

	glGenVertexArrays(1, &m_quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(m_quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
