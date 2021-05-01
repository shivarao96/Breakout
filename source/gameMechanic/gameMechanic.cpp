#include "gameMechanic.h"
#include "../context/context.h"
#include "../spriteRenderer/spriteRenderer.h"
#include "../resourceHandler/resourceHandler.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

const glm::vec2 playerSize = {
	100.0f,
	20.0f
};
const float velocity = 500.0f;

static bool startCheckingCollision = false;

const glm::vec2  initialBallVelocity = {
	100.0f,
	-300.0f
};
const float ballRadius = 12.5f;

GameMechanic::GameMechanic(int width, int height, Context& context)
	:m_width(width)
	,m_height(height)
	,m_context(&context)
{
}

GameMechanic::~GameMechanic()
{
	m_context = NULL;
	delete m_player;
	delete m_pSpriteRenderer;
}

void GameMechanic::init()
{
	ResourceHandler::get().loadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_width), static_cast<float>(m_height), 0.0f, -1.0f, 1.0f);
	ResourceHandler::get().getShader("sprite").setInt("image", 0, true);
	ResourceHandler::get().getShader("sprite").setMatrix4("projection", projection);

	m_pSpriteRenderer = new SpriteRenderer(ResourceHandler::get().getShader("sprite"));

	ResourceHandler::get().loadTexture("assets/textures/awesomeface.png", true, "face");
	ResourceHandler::get().loadTexture("assets/textures/background.jpg", false, "background");
	ResourceHandler::get().loadTexture("assets/textures/block.png", false, "block");
	ResourceHandler::get().loadTexture("assets/textures/block_solid.png", false, "block_solid");
	ResourceHandler::get().loadTexture("assets/textures/paddle.png", true, "paddle");

	m_gameLevels.emplace_back("assets/levels/standard.level", m_width, m_height / 2);
	m_gameLevels.emplace_back("assets/levels/standard2.level", m_width, m_height / 2);
	m_gameLevels.emplace_back("assets/levels/spaceInvader.level", m_width, m_height / 2);
	m_gameLevels.emplace_back("assets/levels/bounce.level", m_width, m_height / 2);

	m_player = new GameObject(
		ResourceHandler::get().getTexture("paddle"),
		{
			m_width / 2.0f - playerSize.x /2.0f,
			m_height - playerSize.y
		},
		playerSize
	);

	m_pBall = new BallObject(
		ResourceHandler::get().getTexture("face"),
		ballRadius,
		{
			m_width / 2.0f - ballRadius,
			m_height - (ballRadius * 2) - playerSize.y
		},
		initialBallVelocity
	);

	m_currentlevel = 2;
}

void GameMechanic::update(float deltaTime)
{
	glm::vec2 newPos = m_pBall->move(deltaTime, m_width);
	if (startCheckingCollision) {
		if (
			checkCollison(*m_pBall, *m_player)
			) {
			m_pBall->setVelocity({m_pBall->getVelocity().x, -m_pBall->getVelocity().y});
			//m_pBall->setPosition({ m_pBall->getPos().x, m_player->getPos().y - playerSize.y / 2 - ballRadius });
		}
	}
	doCollisions();
}

void GameMechanic::render()
{
	if (m_state == GameState::GAME_ACTIVE) {
		m_pSpriteRenderer->drawSprite(ResourceHandler::get().getTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(m_width, m_height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		m_gameLevels[m_currentlevel].draw(*m_pSpriteRenderer);
		m_player->draw(*m_pSpriteRenderer);
		m_pBall->draw(*m_pSpriteRenderer);
	}

}

void GameMechanic::processInput(float deltaTime)
{
	if (m_state == GameState::GAME_ACTIVE) {
		float vel = velocity * deltaTime;
		if (m_context->Keys[GLFW_KEY_A]) {
			if (m_player->getPos().x >= 0.0f) {
				m_player->setPosition({ m_player->getPos().x - vel, m_player->getPos().y });
				if (m_pBall->isStuck()) {
					m_pBall->setPosition({ m_pBall->getPos().x - vel, m_pBall->getPos().y });
				}
			}
		}
		if (m_context->Keys[GLFW_KEY_D]) {
			if (m_player->getPos().x <= m_width - playerSize.x) {
				m_player->setPosition({ m_player->getPos().x + vel, m_player->getPos().y });
				if (m_pBall->isStuck()) {
					m_pBall->setPosition({ m_pBall->getPos().x + vel, m_pBall->getPos().y });
				}
			}
		}
		if (m_context->Keys[GLFW_KEY_SPACE]) {
			startCheckingCollision = true;
			m_pBall->setStuckState(false);
		}
		if (m_context->Keys[GLFW_KEY_LEFT_SHIFT]) {
			startCheckingCollision = false;
			m_pBall->reset(
				{
					m_player->getPos().x + playerSize.x / 2 - ballRadius,
					m_player->getPos().y - playerSize.y / 2- ballRadius
				},
				initialBallVelocity
			);
		}
	} 
}

void GameMechanic::clear()
{
}

bool GameMechanic::checkCollison(BallObject& ball, GameObject& gameObj)
{
	glm::vec2 ballCenter = glm::vec2(ball.getPos() + ballRadius);
	glm::vec2 aabb_half_size = glm::vec2(gameObj.getSize().x / 2.0f, gameObj.getSize().y / 2.0f);
	glm::vec2 aabbCenter = glm::vec2(gameObj.getPos().x + aabb_half_size.x, gameObj.getPos().y + aabb_half_size.y);

	glm::vec2 difference = ballCenter - aabbCenter;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_size, aabb_half_size);
	glm::vec2 closest = aabbCenter + clamped;
	difference = closest - ballCenter;
	return glm::length(difference) < ballRadius;
}

void GameMechanic::doCollisions()
{
	for (auto& brick : m_gameLevels[m_currentlevel].getBricks()) {
		if (!brick.isDestroyed()) {
			if (checkCollison(*m_pBall, brick)) {
				if (brick.isSolid()) {
					m_pBall->setVelocity({ m_pBall->getVelocity().x, -m_pBall->getVelocity().y });
				}
				else {
					brick.setDestroyedState(true);
				}
			}
		}
	}
}
