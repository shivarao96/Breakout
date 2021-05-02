#pragma once

#include <string>
#include <glm/glm.hpp>
#include "../gameObject/gameObject.h"
#include "../textureHandler/textureHandler.h"

struct PowerUp final: public GameObject
{
	std::string type;
	float duration;
	bool activated;
	PowerUp(
		const std::string& type,
		const glm::vec3& color,
		float duration,
		glm::vec2 position,
		TextureHandler& texturehandler
	)
		:GameObject(texturehandler, position, glm::vec2(60.0f, 20.0f), glm::vec2(0.0f, 150.0f), color)
		,type(type)
		,duration(duration)
		, activated(false)
	{}
};

