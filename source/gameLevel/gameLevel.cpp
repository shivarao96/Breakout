#include "gameLevel.h"
#include "../resourceHandler/resourceHandler.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

GameLevel::GameLevel()
{
}

GameLevel::GameLevel(const char* fileName, unsigned int levelWidth, unsigned int levelHeight)
{
	load(fileName, levelWidth, levelHeight);
}



void GameLevel::load(const char* fileName, unsigned int levelWidth, unsigned int levelHeight)
{
	m_bricks.clear();
	std::ifstream file(fileName);
	if (file) {
		std::string line;
		std::vector<std::vector<unsigned int>> tiles;
		while (std::getline(file, line)) // read each line from level file
		{
			unsigned int brickId;
			std::vector<unsigned int> levelIds;
			std::istringstream sstream(line);
			while (sstream >> brickId) {
				levelIds.emplace_back(brickId);
			}
			tiles.emplace_back(levelIds);
		}
		if (tiles.size() > 0) {
			init(tiles, levelWidth, levelHeight);
		}
	}

}

void GameLevel::draw(SpriteRenderer& spriteRenderer)
{
	for (auto& brick : m_bricks) {
		if (!brick.isDestroyed()) {
			brick.draw(spriteRenderer);
		}
	}
}

bool GameLevel::isAllDestroyed() const
{
	for (const auto& brick : m_bricks) {
		if (!brick.isSolid() && !brick.isDestroyed()) {
			return false;
		}
	}
	return true;
}

std::vector<GameObject>& GameLevel::getBricks()
{
	return m_bricks;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
	int height = tileData.size();
	int width = tileData[0].size();
	float brickWidth = levelWidth / static_cast<float>(width);
	float brickHeight = levelHeight / static_cast<float>(height);

	for (int y = 0; y < height; y++)  {
		for (int x = 0; x < width; x++) {
			if (tileData[y][x] == 1) {
				m_bricks.emplace_back(
					ResourceHandler::get().getTexture("block_solid"),
					glm::vec2(brickWidth * x, brickHeight * y),
					glm::vec2(brickWidth, brickHeight),
					glm::vec2(0.0f),
					glm::vec3(0.8f, 0.8f, 0.7f),
					true
				);
			}
			else if (tileData[y][x] > 1) {
				glm::vec3 color = glm::vec3(1.0f);
				if (tileData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);
				m_bricks.emplace_back(
					ResourceHandler::get().getTexture("block"),
					glm::vec2(brickWidth * x, brickHeight * y),
					glm::vec2(brickWidth, brickHeight),
					glm::vec2(0.0f),
					color
				);
			}
		}
	}
}
