#include "gameMechanic.h"
#include "../context/context.h"
#include "../spriteRenderer/spriteRenderer.h"
#include "../resourceHandler/resourceHandler.h"
#include <glm/gtc/matrix_transform.hpp>

const glm::vec2 playerSize = {
	200.0f,
	20.0f
};
const float velocity = 500.0f;

GameMechanic::GameMechanic(int width, int height, Context& context)
	:m_width(width)
	,m_height(height)
	,m_context(&context)
{
}

GameMechanic::~GameMechanic()
{
	m_context = NULL;
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
		glm::vec2(
			m_width / 2.0f - playerSize.x,
			m_height - playerSize.y
		),
		playerSize
	);

	m_currentlevel = 2;
}

void GameMechanic::update(float deltaTime)
{
}

void GameMechanic::render()
{
	if (m_state == GameState::GAME_ACTIVE) {
		m_pSpriteRenderer->drawSprite(ResourceHandler::get().getTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(m_width, m_height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		m_player->draw(*m_pSpriteRenderer);

		m_gameLevels[m_currentlevel].draw(*m_pSpriteRenderer);
	}

}

void GameMechanic::processInput(float deltaTime)
{
	if (m_state == GameState::GAME_ACTIVE) {
		float vel = velocity * deltaTime;
		if (m_context->Keys[GLFW_KEY_A]) {
			if (m_player->getPos().x >= 0.0f) {
				m_player->setPosition({ m_player->getPos().x - vel, m_player->getPos().y });
			}
		}
		if (m_context->Keys[GLFW_KEY_D]) {
			if (m_player->getPos().x <= m_width - playerSize.x) {
				m_player->setPosition({ m_player->getPos().x + vel, m_player->getPos().y });
			}
		}
	} 
}

void GameMechanic::clear()
{
}
