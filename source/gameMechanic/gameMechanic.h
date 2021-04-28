#pragma once

#include "../context/context.h"

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
	GameState state;
	int width, height;
	Context* m_context;
};

