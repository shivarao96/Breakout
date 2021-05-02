#pragma once

#include <glm/glm.hpp>
#include "../spriteRenderer/spriteRenderer.h"
#include "../textureHandler/textureHandler.h"

class GameObject
{
public:
	GameObject();
	GameObject(TextureHandler& sprite, const glm::vec2 pos, const glm::vec2 size, const glm::vec2 velocity = glm::vec2(0.0f), const glm::vec3 color = glm::vec3(1.0f), bool isSolid = false);
	~GameObject();
	virtual void draw(SpriteRenderer& spriteRenderer);
	
	bool isDestroyed() const;
	bool isSolid() const;
	const glm::vec2& getPos() const;
	const glm::vec2& getVelocity() const;
	const glm::vec2& getSize() const;

	void setDestroyedState(bool state);
	void setSolidState(bool state);
	void setPosition(const glm::vec2& pos);
	void setVelocity(const glm::vec2& velocity);
	glm::vec2 m_velocity = glm::vec2(0.0f);
	glm::vec2 m_size = glm::vec2(1.0f);
	glm::vec3 m_color = glm::vec3(1.0f);
	glm::vec2 m_position = glm::vec2(0.0f);

protected:
	TextureHandler* m_pSprite = nullptr;
	float m_rotation = 0.0f;
	bool m_isSolid = false;
	bool m_isDestroyed = false;
};

