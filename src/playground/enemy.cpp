#include <glm/glm.hpp>
#include <random>
#include "enemy.h"

    Enemy::Enemy() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> distrib(-0.90f, 0.90f);
        float postionX = distrib(gen);
        float postionY = distrib(gen);

        positionM = glm::mat3x2(postionX, postionY, postionX + 0.10f, postionY + 0.10f, postionX + 0.2f, postionY);

    }

    Enemy::~Enemy() {}

    glm::mat3x2 Enemy::get() {
        return positionM;
    }


