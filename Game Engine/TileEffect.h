#pragma once
class TileEffect
{
public:
	TileEffect(float accelMult = 1, float decelMult = 1, float velXChange = 0, float velYChange = 0, float maxVelChange = 0, float knockback = 0, float tileDamage = 0);
	virtual ~TileEffect();
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
	float getKnockbackForce();
	void setKnockbackForce(float newForce);
	float getDamage();
	void setDamage(float newDamage);
private:
	float accelerationMultiplier;
	float decelerationMultiplier;
	float xChange;
	float yChange;
	float maxVelocityChange;
	float knockbackForce;
	float damage;
};

