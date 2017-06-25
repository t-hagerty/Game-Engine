#include "Enemy.h"



Enemy::Enemy(int h, int w, double positionX, double positionY, double velX, double velY, int hp) : Character(h, w, positionX, positionY, velX, velY, hp)
{
}


Enemy::~Enemy()
{
}

void Enemy::determineMovement(double playerPosX, double plaerPosY)
{
}
