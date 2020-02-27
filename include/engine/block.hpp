#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <glm/glm.hpp>

class Block {

public:
	Block();
	Block(const glm::vec3& b_position, const glm::vec3& b_size, const bool& b_isDestroyed);
	void setPosition(glm::vec3 b_position);
	void setSize(glm::vec3 b_size);
	void setIsDestroyed(bool b_isDestroyed);
	glm::vec3 getPosition();
	glm::vec3 getSize();
	bool getIsDestroyed();

private:
	glm::vec3 _position;
	glm::vec3 _size = glm::vec3(0.5f, 0.2f, 0.2f);
	bool _isDestroyed = false;

};

#endif
