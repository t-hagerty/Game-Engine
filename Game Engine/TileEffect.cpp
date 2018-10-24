#include "TileEffect.h"



TileEffect::TileEffect(float accelMult, float decelMult, float velXChange, float velYChange, float maxVelChange, float knockback, float tileDamage)
{
	accelerationMultiplier = accelMult;
	decelerationMultiplier = decelMult;
	xChange = velXChange;
	yChange = velYChange;
	maxVelocityChange = maxVelChange;
	knockbackForce = knockback;
	damage = tileDamage;
}


TileEffect::~TileEffect()
{
}

float TileEffect::getAccelerationMultiplier()
{
	return accelerationMultiplier;
}

void TileEffect::setAccelerationMultiplier(float newAccel)
{
	accelerationMultiplier = newAccel;
}

float TileEffect::getDecelerationMultiplier()
{
	return decelerationMultiplier;
}

void TileEffect::setDecelerationMultiplier(float newDecel)
{
	decelerationMultiplier = newDecel;
}

float TileEffect::getXChange()
{
	return xChange;
}

void TileEffect::setXChange(float newXChange)
{
	xChange = newXChange;
}

float TileEffect::getYChange()
{
	return yChange;
}

void TileEffect::setYChange(float newYChange)
{
	yChange = newYChange;
}

float TileEffect::getMaxVelocityChange()
{
	return maxVelocityChange;
}

void TileEffect::setMaxVelocityChange(float newMax)
{
	maxVelocityChange = newMax;
}

float TileEffect::getKnockbackForce()
{
	return knockbackForce;
}

void TileEffect::setKnockbackForce(float newForce)
{
	knockbackForce = newForce;
}

float TileEffect::getDamage()
{
	return damage;
}

void TileEffect::setDamage(float newDamage)
{
	damage = newDamage;
}
