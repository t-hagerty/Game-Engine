#pragma once
#include "GUIElement.h"
#include "Image.h"
#include "Entity.h"
#include <vector>
class HealthBar :
	public GUIElement
{
public:
	HealthBar(double x, double y, double width, double height, bool visible, std::string imageFilePath, SDL_Surface* targetSurface, SDL_Renderer* targetRenderer,
		double maxHP);
	~HealthBar();
	int getNumberHearts();
	void update(double currentHealth);
	void setMaxHealth(double newMax);
	void setIsVisible(bool visible) override;
	void toggleVisibility() override;
	bool render() override;
private:
	const int HEALTH_PER_HEART = 4;
	int numberHearts;
	double prevHealth;
	double maxHealth;
	std::vector<Image*> hearts;
};

