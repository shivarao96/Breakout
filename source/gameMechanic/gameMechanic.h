#pragma once

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
	GameState state;
	int m_width, m_height;
	Context* m_context = nullptr;
	SpriteRenderer* m_pSpriteRenderer = nullptr;
};

