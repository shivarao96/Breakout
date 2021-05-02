#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "../shaderHandler/shaderHandler.h"
#include "../textureHandler/textureHandler.h"
#include "../gameObject/gameObject.h"

struct Particle
{
	glm::vec2 position, velocity;
	glm::vec4 color;
	float life;

	Particle():position(0.0f), velocity(0.0f), color(1.0f), life(0.0f){}
};

class ParticleGenerator
{
public:
	ParticleGenerator(ShaderHandler& shaderhandler, TextureHandler& textureHandler, unsigned int totalParticles)
		: m_pShaderHandler(&shaderhandler)
		, m_pTextureHandler(&textureHandler)
		, m_particleCount(totalParticles)
	{
		init();
	}

	void drawParticles() {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		m_pShaderHandler->useShader();
		for (auto& particle : m_particles) {
			if (particle.life > 0.0f) {
				m_pShaderHandler->setVector2f("offset", particle.position);
				m_pShaderHandler->setVector4f("color", particle.color);
				m_pTextureHandler->bindTex();
				glBindVertexArray(m_VAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindVertexArray(0);
			}
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void updateParticles(float deltaTimer,const GameObject& gameObject, unsigned int newParticlesCount, const glm::vec2& offset) {
		for (unsigned int i = 0; i < newParticlesCount; ++i) {
			unsigned int newPartIndex = fetchFirstUnusedParticle();
			respawnParticle(m_particles[newPartIndex], gameObject, offset);
		}

		// update the particles
		for (unsigned int i = 0; i < m_particleCount; ++i) {
			Particle& p = m_particles[i];
			p.life -= deltaTimer;
			if (p.life > 0.0f) {
				p.position -= p.velocity * deltaTimer;
				p.color.a -= deltaTimer * 2.5f;
			}
		}
	}

private:
	void init() {
		
		const float vertices[] = {
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};
 		
		unsigned int VBO;
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
		glBindVertexArray(0);

		for (unsigned int i = 0; i < m_particleCount; ++i) {
			m_particles.push_back(Particle());
		}
	}

	unsigned int fetchFirstUnusedParticle() {
		for (unsigned int i = m_lastUsedParticle; i < m_particleCount; ++i) {
			if (m_particles[i].life <= 0.0f) {
				m_lastUsedParticle = i;
				return i;
			}
		}
		// do linear if not found in above;
		for (unsigned int i = 0; i < m_lastUsedParticle; i++) {
			if (m_particles[i].life <= 0.0f) {
				m_lastUsedParticle = i;
				return i;
			}
		}

		m_lastUsedParticle = 0;
		return 0;
	}

	void respawnParticle(Particle& particle, const GameObject& gameObject, const glm::vec2& offset) {
		float random = ((rand() % 100) - 50) / 10.0f;
		float randomColor = 0.5f + ((rand() % 100) / 100.0f);
		particle.position = gameObject.getPos() + offset + random;
		particle.velocity = gameObject.getVelocity() * 0.1f;
		particle.life = 1.0f;
		particle.color = glm::vec4(randomColor, randomColor, randomColor, 1.0f);
	}

	unsigned int m_particleCount = 10;
	unsigned int m_VAO;
	ShaderHandler* m_pShaderHandler = nullptr;
	TextureHandler* m_pTextureHandler = nullptr;
	std::vector<Particle> m_particles;
	unsigned int m_lastUsedParticle = 0;
};

