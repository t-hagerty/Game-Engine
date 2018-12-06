#include "HealthBar.h"



HealthBar::HealthBar(double x, double y, double width, double height, bool visible, std::string imageFilePath, SDL_Surface* targetSurface, SDL_Renderer* targetRenderer,
	double maxHP) : GUIElement(x, y, width, height, visible, imageFilePath, targetSurface, targetRenderer)
{
	maxHealth = maxHP;
	numberHearts = std::ceil(maxHealth / HEALTH_PER_HEART);
	prevHealth = 0; //So that update will fully run in the constructor
	for (int i = 0; i < numberHearts; i++)
	{
		hearts.push_back(new Image(x + (i * 32), y, 32, 32, true, "hearts.bmp", 5, trgtSurface, trgtRenderer));
	}
	update(maxHealth);
}


HealthBar::~HealthBar()
{

}

int HealthBar::getNumberHearts()
{
	return numberHearts;
}

void HealthBar::update(double currentHealth)
{
	if (prevHealth == currentHealth)
	{
		return;
	}
	int i = 0;
	for (i; i < numberHearts; i++)
	{
		hearts[i]->setAnimationFrame(HEALTH_PER_HEART);
	}
	i = std::floor(currentHealth / HEALTH_PER_HEART);
	hearts[i]->setAnimationFrame(((int)currentHealth - (i * HEALTH_PER_HEART)) % HEALTH_PER_HEART);
	i++;
	for (i; i < numberHearts; i++)
	{
		hearts[i]->setAnimationFrame(0);
	}
	for (int j = 0; j < numberHearts; j++)
	{
		printf("Heart %d on frame %d", j, hearts[j]->getAnimationFrame());
	}
	printf("\n");
	prevHealth = currentHealth;
}

void HealthBar::setMaxHealth(double newMax)
{
	maxHealth = newMax;
	numberHearts = ceil(maxHealth / HEALTH_PER_HEART);
}

void HealthBar::setIsVisible(bool visible)
{
	for (Image* i : hearts)
	{
		i->setIsVisible(visible);
	}
	isVisible = visible;
}

void HealthBar::toggleVisibility()
{

	for (Image* i : hearts)
	{
		i->toggleVisibility();
	}
	isVisible = !isVisible;
}

bool HealthBar::render()
{
	bool success = true;
	for (std::size_t i = 0; i < hearts.size(); i++)
	{
		if (!hearts[i]->render())
		{
			success = false;
			printf("Heart #%d in health bar could not render!\n", i);
		}
	}
	return success;
}
