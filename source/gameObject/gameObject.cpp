#include "gameObject.h"

GameObject::GameObject()
{}

GameObject::GameObject(TextureHandler& sprite, const glm::vec2 pos, const glm::vec2 size, const glm::vec2 velocity, const glm::vec3 color)
	:m_pSprite(&sprite)
	,m_position(pos)
	,m_size(size)
	,m_velocity(velocity)
	,m_color(color)
{
}

GameObject::~GameObject()
{
	m_pSprite = nullptr;
}

void GameObject::draw(SpriteRenderer& spriteRenderer)
{
	spriteRenderer.drawSprite(
		*m_pSprite,
		m_position,
		m_size,
		m_rotation,
		m_color
	);
}

bool GameObject::isDestroyed() const
{
	return m_isDestroyed;
}

bool GameObject::isSolid() const
{
	return m_isSolid;
}

const glm::vec2& GameObject::getPos() const
{
	return m_position;
}

void GameObject::setDestroyedState(bool state)
{
	m_isDestroyed = state;
}

void GameObject::setSolidState(bool state)
{
	m_isSolid = state;
}

void GameObject::setPosition(const glm::vec2& pos)
{
	m_position = pos;
}
