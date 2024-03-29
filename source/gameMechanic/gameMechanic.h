#pragma once

#include <vector>
#include "../gameLevel/gameLevel.h"
#include "../gameObject/ballObject/ballObject.h"
#include "../particleGenerator/particleGenerator.h"
#include "../postProcessing/postProcessing.h";
#include <glm/glm.hpp>
#include "../powerUp/powerUp.h"

class SpriteRenderer;
class Context;

enum class CollisionDirection {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

struct Collision {
	bool isCollided;
	CollisionDirection direction;
	glm::vec2 difference;

	Collision(bool isCollided, CollisionDirection direction, const glm::vec2& difference):isCollided(isCollided), direction(direction), difference(difference) {}
};


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
	Collision checkCollison(GameObject& ball, GameObject& gameObj);
	void doCollisions();
	static CollisionDirection getCollisionDirection(const glm::vec2& difference);
	void spawnPowerUps(GameObject& obj);
	void activatePowerUp(PowerUp& powerUp);
	void updatePowerUps(float deltaTimer);
	bool isOtherPowerActive(const std::string type);

	std::vector<PowerUp> m_powerUps;
	GameState m_state = GameState::GAME_ACTIVE;
	int m_width, m_height;
	Context* m_context = nullptr;
	SpriteRenderer* m_pSpriteRenderer = nullptr;
	ParticleGenerator* m_pParticleGenerator = nullptr;
	std::vector<GameLevel> m_gameLevels;
	unsigned int m_currentlevel = 0;
	
	GameObject* m_player = nullptr;
	BallObject* m_pBall = nullptr;
	PostProcessing* m_pPostProcessing = nullptr;
};


