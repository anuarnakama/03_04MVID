#include "engine/block.hpp"



Block::Block()
{
	_position = glm::vec3(0.0f, 0.0f, 0.0f);
	_size = glm::vec3(0.5f, 0.2f, 0.2f);
	_isDestroyed = false;
}

Block::Block(const glm::vec3& b_position, const glm::vec3& b_size, const bool& b_isDestroyed)
	:_position(b_position), _size(b_size), _isDestroyed(b_isDestroyed) {}

void Block::setIsDestroyed(bool b_isDestroyed)
{
	_isDestroyed = b_isDestroyed;
}

void Block::setPosition(glm::vec3 b_position)
{
	_position = b_position;
}

void Block::setSize(glm::vec3 b_size)
{
	_size = b_size;
}

glm::vec3 Block::getPosition()
{
	return _position;
}

bool Block::getIsDestroyed()
{
	return _isDestroyed;
}

glm::vec3 Block::getSize()
{
	return _size;
}
