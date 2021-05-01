#pragma once

#include "../gameObject.h"

class BallObject: public GameObject
{
public:
	BallObject();
	BallObject(TextureHandler& ballSprite, float radius, const glm::vec2& pos, const glm::vec2& velocity);
	const glm::vec2& move(float deltaTime, int screenWidth);
	void reset(const glm::vec2& pos, const glm::vec2& velocity);
	
	bool isStuck() const;
	void setStuckState(bool state);
private:
	float m_radius;
	bool m_stuck;
};

