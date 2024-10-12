#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Target
{
private:
	//target sprite
	sf::Sprite targetSprite;
	sf::Texture targetTexture;

	//target variables
	sf::Vector2f targetPosWindow;
	bool targetingEnemy;
	int enemyTargetedID;
	double targetCoolDown;
	double targetCoolDownMax;

	//initializer functions
	void initTexture();
	void initTarget();
	void initVariables();

public:
	//target constructors & desctructors
	Target();
	virtual ~Target();

	//target functions
	void update(sf::Vector2f position);
	void render(sf::RenderTarget& target);
	void setEnemyTargetedID(int ID);
	int getEnemyTargetedID();
	void setIsTargetingEnemy(bool target);
	bool isTargetingEnemy();
	bool updateCoolDown();
	sf::Vector2f getPosWindow();
};

