#include "gameMechanic.h"
#include "../context/context.h"
#include "../spriteRenderer/spriteRenderer.h"
#include "../resourceHandler/resourceHandler.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

float ShakeTime = 0.0f;

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
	ResourceHandler::get().loadShader("shaders/particle.vs", "shaders/particle.fs", nullptr, "particle");
	ResourceHandler::get().loadShader("shaders/postProcessing.vs", "shaders/postProcessing.fs", nullptr, "postProcessing");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_width), static_cast<float>(m_height), 0.0f, -1.0f, 1.0f);
	ResourceHandler::get().getShader("sprite").setInt("image", 0, true);
	ResourceHandler::get().getShader("sprite").setMatrix4("projection", projection);
	ResourceHandler::get().getShader("particle").setInt("sprite", 0, true);
	ResourceHandler::get().getShader("particle").setMatrix4("projection", projection);

	ResourceHandler::get().loadTexture("assets/textures/awesomeface.png", true, "face");
	ResourceHandler::get().loadTexture("assets/textures/background.jpg", false, "background");
	ResourceHandler::get().loadTexture("assets/textures/block.png", false, "block");
	ResourceHandler::get().loadTexture("assets/textures/block_solid.png", false, "block_solid");
	ResourceHandler::get().loadTexture("assets/textures/paddle.png", true, "paddle");
	ResourceHandler::get().loadTexture("assets/textures/particle.png", true, "particle");
	ResourceHandler::get().loadTexture("assets/textures/speed.png", true, "powerup_speed");
	ResourceHandler::get().loadTexture("assets/textures/sticky.png", true, "powerup_sticky");
	ResourceHandler::get().loadTexture("assets/textures/pass-through.png", true, "powerup_pass-through");
	ResourceHandler::get().loadTexture("assets/textures/increase-padd-size.png", true, "powerup_increase-padd-size");
	ResourceHandler::get().loadTexture("assets/textures/confuse.png", true, "powerup_confuse");
	ResourceHandler::get().loadTexture("assets/textures/chaos.png", true, "powerup_chaos");

	m_gameLevels.emplace_back("assets/levels/standard.level", m_width, m_height / 2);
	m_gameLevels.emplace_back("assets/levels/standard2.level", m_width, m_height / 2);
	m_gameLevels.emplace_back("assets/levels/spaceInvader.level", m_width, m_height / 2);
	m_gameLevels.emplace_back("assets/levels/bounce.level", m_width, m_height / 2);

	m_pSpriteRenderer = new SpriteRenderer(ResourceHandler::get().getShader("sprite"));
	m_pParticleGenerator = new ParticleGenerator(
		ResourceHandler::get().getShader("particle"),
		ResourceHandler::get().getTexture("particle"),
		500
	);

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

	m_pPostProcessing = new PostProcessing(
		ResourceHandler::get().getShader("postProcessing"),
		m_width,
		m_height
	);

	m_currentlevel = 0;
}

void GameMechanic::update(float deltaTime)
{
	m_pBall->move(deltaTime, m_width);
	Collision collision = checkCollison(*m_pBall, *m_player);
	if (!m_pBall->isStuck() && collision.isCollided) {
		float centerBoard = m_player->getPos().x + playerSize.x / 2.0f;
		float distance = (m_pBall->getPos().x + ballRadius) - centerBoard;
		float percentage = distance / (playerSize.x / 2.0f);

		glm::vec2 oldVelocity = m_pBall->getVelocity();
		m_pBall->setVelocity({ initialBallVelocity.x * 2.0f * percentage, -1.0f * std::abs(m_pBall->getVelocity().y)});
		glm::vec2 newVelocity = glm::normalize(m_pBall->getVelocity()) * glm::length(oldVelocity);
		m_pBall->setVelocity(newVelocity);
	}
	doCollisions();
	m_pParticleGenerator->updateParticles(deltaTime, *m_pBall, 2, glm::vec2(ballRadius / 2.0f));
	if (m_pBall->getPos().y > m_height) {
		m_pBall->reset(
			{
				m_width / 2.0f - ballRadius,
				m_height - (ballRadius * 2) - playerSize.y
			},
			initialBallVelocity
		);
		m_player->setPosition({
			m_width / 2.0f - playerSize.x / 2.0f,
			m_height - playerSize.y
			}
		);
	}

	if (ShakeTime > 0.0f) {
		ShakeTime -= deltaTime;
		if (ShakeTime <= 0.0f) {
			m_pPostProcessing->m_shake = false;
		}
	}
	updatePowerUps(deltaTime);
}

void GameMechanic::render()
{
	if (m_state == GameState::GAME_ACTIVE) {
		m_pPostProcessing->beginRenderer();
		m_pSpriteRenderer->drawSprite(ResourceHandler::get().getTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(m_width, m_height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		m_gameLevels[m_currentlevel].draw(*m_pSpriteRenderer);
		m_player->draw(*m_pSpriteRenderer);
		m_pParticleGenerator->drawParticles();
		m_pBall->draw(*m_pSpriteRenderer);
		for (auto& powerup : m_powerUps) {
			if (!powerup.isDestroyed()) {
				powerup.draw(*m_pSpriteRenderer);
			}
		}
		m_pPostProcessing->endRenderer();
		m_pPostProcessing->renderPostProcessing(glfwGetTime());
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
			//startCheckingCollision = true;
			m_pBall->setStuckState(false);
		}
		if (m_context->Keys[GLFW_KEY_LEFT_SHIFT]) {
			//startCheckingCollision = false;
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

Collision GameMechanic::checkCollison(GameObject& ball, GameObject& gameObj)
{
	glm::vec2 ballCenter = glm::vec2(ball.getPos() + ballRadius);
	glm::vec2 aabb_half_size = glm::vec2(gameObj.getSize().x / 2.0f, gameObj.getSize().y / 2.0f);
	glm::vec2 aabbCenter = glm::vec2(gameObj.getPos().x + aabb_half_size.x, gameObj.getPos().y + aabb_half_size.y);

	glm::vec2 difference = ballCenter - aabbCenter;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_size, aabb_half_size);
	glm::vec2 closest = aabbCenter + clamped;
	difference = closest - ballCenter;
	if (glm::length(difference) <= ballRadius) {
		return Collision(true, getCollisionDirection(difference), difference);
	}
	else {
		return Collision(false, CollisionDirection::UP, glm::vec2(0.0f)) ;
	}
}

void GameMechanic::doCollisions()
{
	for (auto& brick : m_gameLevels[m_currentlevel].getBricks()) {
		if (!brick.isDestroyed()) {
			Collision collision = checkCollison(*m_pBall, brick);
			if (collision.isCollided) {
				if (!brick.isSolid()) {
					brick.setDestroyedState(true);
					spawnPowerUps(brick);
				}
				else {
					ShakeTime = 0.5f;
					m_pPostProcessing->m_shake = true;
				}
				CollisionDirection dir = collision.direction;
				const glm::vec2 diff = collision.difference;
				
				if (!(m_pBall->m_passthrough && !m_pBall->isSolid())) {
					if (dir == CollisionDirection::LEFT || dir == CollisionDirection::RIGHT) {
						m_pBall->setVelocity(glm::vec2(-m_pBall->getVelocity().x, m_pBall->getVelocity().y));
						float penetration = ballRadius - std::abs(diff.x);
						if (dir == CollisionDirection::LEFT) {
							m_pBall->setPosition(glm::vec2(m_pBall->getPos().x + penetration, m_pBall->getPos().y));
						}
						else {
							m_pBall->setPosition(glm::vec2(m_pBall->getPos().x - penetration, m_pBall->getPos().y));
						}
					}
					else {
						m_pBall->setVelocity(glm::vec2(m_pBall->getVelocity().x, -m_pBall->getVelocity().y));
						float penetration = ballRadius - std::abs(diff.y);
						if (dir == CollisionDirection::DOWN) {
							m_pBall->setPosition(glm::vec2(m_pBall->getPos().x, m_pBall->getPos().y + penetration));
						}
						else {
							m_pBall->setPosition(glm::vec2(m_pBall->getPos().x, m_pBall->getPos().y - penetration));
						}
					}
				}

			}
		}
	}

	for (auto& powerUp : m_powerUps) {
		if (!powerUp.isDestroyed()) {
			if (powerUp.getPos().y > m_height) {
				powerUp.setDestroyedState(true);
			}
			Collision collision = checkCollison(*m_player, powerUp);
			if (collision.isCollided) {
				activatePowerUp(powerUp);
				powerUp.setDestroyedState(true);
				powerUp.activated = true;
			}
		}
	}
}

CollisionDirection GameMechanic::getCollisionDirection(const glm::vec2& difference)
{
	int max = 0.0f;
	int bestMatch = -1;
	const glm::vec2 normalDirection[] = {
		glm::vec2(0.0f, 1.0f),  // up
		glm::vec2(1.0f, 0.0f),  // right
		glm::vec2(0.0f, -1.0f), // down
		glm::vec2(-1.0f, 0.0f), // left
	};

	for (int i = 0; i < 4; i++) {
		float dotProduct = glm::dot(glm::normalize(difference), normalDirection[i]);
		if (dotProduct > max) {
			bestMatch = i;
			max = dotProduct;
		}
	}
	return (CollisionDirection)bestMatch;
}

void GameMechanic::spawnPowerUps(GameObject& obj)
{
	auto shouldSpawn = [&](unsigned int chance) {
		const unsigned int random = rand() % chance;
		return random == 0;
	};

	if (shouldSpawn(75)) {
		m_powerUps.emplace_back(
			"speed", glm::vec3(0.5f, 0.5f, 1.0f), 10.0f, obj.getPos(), ResourceHandler::get().getTexture("powerup_speed")
		);
	}
	//if (shouldSpawn(75)) {
	//	m_powerUps.emplace_back(
	//		"sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, obj.getPos(), ResourceHandler::get().getTexture("powerup_sticky")
	//	);
	//}
	if (shouldSpawn(75)) {
		m_powerUps.emplace_back(
			"pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, obj.getPos(), ResourceHandler::get().getTexture("powerup_pass-through")
		);
	}
	if (shouldSpawn(75)) {
		m_powerUps.emplace_back(
			"pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, obj.getPos(), ResourceHandler::get().getTexture("powerup_increase-padd-size")
		);
	}
	if (shouldSpawn(15)) {
		m_powerUps.emplace_back(
			"confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, obj.getPos(), ResourceHandler::get().getTexture("powerup_confuse")
		);
	}
	if (shouldSpawn(15)) {
		m_powerUps.emplace_back(
			"chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, obj.getPos(), ResourceHandler::get().getTexture("powerup_chaos")
		);
	}
}

void GameMechanic::activatePowerUp(PowerUp& powerUp)
{
	if (powerUp.type == "speed") {
		m_pBall->m_velocity *= 1.2;
	}
	//else if (powerUp.type == "sticky") {
	//	m_pBall->m_sticky = true;
	//	m_player->m_color = glm::vec3(1.0f, 0.5f, 1.0f);
	//}
	else if (powerUp.type == "pass-through") {
		m_pBall->m_passthrough = true;
		m_pBall->m_color = glm::vec3(1.0f, 0.5f, 0.5f);
	}		
	else if (powerUp.type == "powerup_increase-padd-size") {
		m_player->m_size.x += 50;
	}	
	else if (powerUp.type == "confuse") {
		if (!m_pPostProcessing->m_chaos) {
			m_pPostProcessing->m_confuse = true;
		}
	}	
	else if (powerUp.type == "chaos") {
		if (!m_pPostProcessing->m_confuse) {
			m_pPostProcessing->m_chaos = true;
		}
	}
}

void GameMechanic::updatePowerUps(float deltaTimer)
{
	for (auto& powerUps: m_powerUps) {
		powerUps.m_position += powerUps.m_velocity * deltaTimer;
		if (powerUps.activated) {
			powerUps.duration -= deltaTimer;
			if (powerUps.duration <= 0.0f) {
				powerUps.activated = false;
				if (powerUps.type == "sticky") {
					if (!isOtherPowerActive("sticky")) {
						m_pBall->m_sticky = false;
					}
				}
				if (powerUps.type == "pass-through") {
					if (!isOtherPowerActive("pass-through")) {
						m_pBall->m_passthrough = false;
						m_pBall->m_color = glm::vec3(1.0f);
					}
				}
				if (powerUps.type == "confuse") {
					if (!isOtherPowerActive("confuse")) {
						m_pPostProcessing->m_confuse = false;
					}
				}
				if (powerUps.type == "chaos") {
					if (!isOtherPowerActive("chaos")) {
						m_pPostProcessing->m_chaos = false;
					}
				}
			}
		}
	}

	m_powerUps.erase(
		std::remove_if(
			m_powerUps.begin(),
			m_powerUps.end(),
			[](const PowerUp& powerup){return !powerup.activated && powerup.isDestroyed(); }
		),
		m_powerUps.end()
	);
}

bool GameMechanic::isOtherPowerActive(const std::string type)
{
	for (auto& powerup: m_powerUps) {
		if (powerup.activated) {
			if (powerup.type == type) {
				return false;
			}
		}
	}
	return false;
}


