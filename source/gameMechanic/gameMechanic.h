#pragma once

#include <vector>
#include "../gameLevel/gameLevel.h"

class SpriteRenderer;
class Context;

enum class GameState {
	GAME_MENU,
	GAME_ACTIVE,
	GAME_WIN,
	GAME_LOSS
};

class GameMechanic
{
public:
	GameMechanic(int width, int height, Context& context);
	~GameMechanic();

	void init();
	void update(float deltaTime);
	void render();
	void processInput(float deltaTime);
	void clear();
private:
	GameState m_state = GameState::GAME_ACTIVE;
	int m_width, m_height;
	Context* m_context = nullptr;
	SpriteRenderer* m_pSpriteRenderer = nullptr;
	GameObject* m_player;
	std::vector<GameLevel> m_gameLevels;
	unsigned int m_currentlevel = 0;
};

