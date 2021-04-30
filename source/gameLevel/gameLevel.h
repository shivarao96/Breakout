#pragma once

#include <vector>
#include "../gameObject/gameObject.h"
#include "../spriteRenderer/spriteRenderer.h"

class GameLevel
{
public:
	GameLevel();
	GameLevel(const char* fileName, unsigned int levelWidth, unsigned int levelHeight);
	void load(const char* fileName, unsigned int levelWidth, unsigned int levelHeight);
	void draw(SpriteRenderer& spriteRenderer);
	bool isAllDestroyed() const;
private:
	std::vector<GameObject> m_bricks;
	void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

