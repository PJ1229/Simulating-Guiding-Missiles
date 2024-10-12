#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;

class Enemy
{
private:
	//enemy sprite
	sf::Sprite jetSprite;

	//tracking
	std::vector<sf::Vertex> tracking;

	//path
	string path;

	//enemy variables
	int enemyID;
	double movementSpeed;

	//position window
	sf::Vector2f enemyInitWindow;
	sf::Vector2f enemyPosWindow(int t);

	//direction
	double initTheta;
	double theta(int t);
	void direction();

	//time
	double time;

	//functions
	void initVariables();

public:
	//counts how many enemies are in the game
	static int howManyEnemies;

	//enemy constructors & destructors
	Enemy(sf::Texture* texture, float spawnX, float spawnY, float spawnDir);
	virtual ~Enemy();

	//enemy functions
	bool update();
	sf::Vector2f getPosWindow();
	bool isTouching(sf::Vector2f vector);
	void render(sf::RenderTarget& target);
	int getEnemyID();
	int getHowManyEnemys();
	void setMovementSpeed(double a);
};

