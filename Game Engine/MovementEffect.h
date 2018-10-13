#pragma once
class MovementEffect
{
public:
	MovementEffect(float accelMult = 1, float decelMult = 1, float velXChange = 0, float velYChange = 0, float maxVelChange = 0);
	virtual ~MovementEffect();
	float getAccelerationMultiplier();
	void setAccelerationMultiplier(float newAccel);
	float getDecelerationMultiplier();
	void setDecelerationMultiplier(float newDecel);
	float getXChange();
	void setXChange(float newXChange);
	float getYChange();
	void setYChange(float newYChange);
	float getMaxVelocityChange();
	void setMaxVelocityChange(float newMax);
private:
	float accelerationMultiplier;
	float decelerationMultiplier;
	float xChange;
	float yChange;
	float maxVelocityChange;
};

