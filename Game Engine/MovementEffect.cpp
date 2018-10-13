#include "MovementEffect.h"



MovementEffect::MovementEffect(float accelMult, float decelMult, float velXChange, float velYChange, float maxVelChange)
{
	accelerationMultiplier = accelMult;
	decelerationMultiplier = decelMult;
	xChange = velXChange;
	yChange = velYChange;
	maxVelocityChange = maxVelChange;
}


MovementEffect::~MovementEffect()
{
}

float MovementEffect::getAccelerationMultiplier()
{
	return accelerationMultiplier;
}

void MovementEffect::setAccelerationMultiplier(float newAccel)
{
	accelerationMultiplier = newAccel;
}

float MovementEffect::getDecelerationMultiplier()
{
	return decelerationMultiplier;
}

void MovementEffect::setDecelerationMultiplier(float newDecel)
{
	decelerationMultiplier = newDecel;
}

float MovementEffect::getXChange()
{
	return xChange;
}

void MovementEffect::setXChange(float newXChange)
{
	xChange = newXChange;
}

float MovementEffect::getYChange()
{
	return yChange;
}

void MovementEffect::setYChange(float newYChange)
{
	yChange = newYChange;
}

float MovementEffect::getMaxVelocityChange()
{
	return maxVelocityChange;
}

void MovementEffect::setMaxVelocityChange(float newMax)
{
	maxVelocityChange = newMax;
}
