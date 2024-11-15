#ifndef ENEMY_H
#define ENEMY_H
#include <glm/glm.hpp>

class Enemy {
private:
	glm::mat3x2 positionM;
public:
	Enemy();
	~Enemy();
	glm::mat3x2 get();
};
#endif