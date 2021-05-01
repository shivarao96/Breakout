#include "ballObject.h"
#include <iostream>

BallObject::BallObject() :m_stuck(true), m_radius(12.5f)
{
}

BallObject::BallObject(TextureHandler& ballSprite, float radius, const glm::vec2& pos, const glm::vec2& velocity)
	: GameObject(ballSprite, pos, glm::vec2(radius * 2), velocity)
	, m_radius(radius)
	, m_stuck(true)
{
}

const glm::vec2& BallObject::move(float deltaTime, int screenWidth)
{
	if (!m_stuck) {
		m_position += m_velocity * deltaTime;
		if (m_position.x <= 0.0f) {
			m_velocity.x = -m_velocity.x;
			m_position.x = 0.0f;
		}
		if (m_position.x + m_size.x >= screenWidth) {
			m_velocity.x = -m_velocity.x;
			m_position.x = screenWidth - m_size.x;
		}
		if (m_position.y <= 0.0f) {
			m_velocity.y = -m_velocity.y;
			m_position.y = 0.0f;
		}
	}
	return m_position;
}

void BallObject::reset(const glm::vec2& pos, const glm::vec2& velocity)
{
	m_position = pos;
	m_velocity = velocity;
	m_stuck = true;
}

bool BallObject::isStuck() const
{
	return m_stuck;
}

void BallObject::setStuckState(bool state)
{
	m_stuck = state;
}
