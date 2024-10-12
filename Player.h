#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

class Player
{
private:
	//player object
	sf::Sprite tankHeadSprite;
	sf::Sprite tankBodySprite;
	sf::Texture tankHeadTexture;
	sf::Texture tankBodyTexture1;
	sf::Texture tankBodyTexture2;

	//player qualities
	double movementSpeed;
	double turnSpeed;
	double attackCoolDown;
	double attackCoolDownMax;
	int tankBodyTexture;
	sf::Vector2f tankHeadPosWindow;
	string isCollision;

	//initiate player
	void initTankHead();
	void initTankBody();
	void initTexture();
	void initVariables();

public:
	//constructors and destructors
	Player();
	virtual ~Player();

	//functions
	void update();
	bool updateCoolDown();
	void move(const float dirX, const float dirY);
	void rotateHead(const float dirX, const float dirY);
	void rotateBody(const float direction);
	void render(sf::RenderTarget& target);
	sf::Vector2f getPosition();
	double getDirection();

	//setter functions for bullet time
	void setMovementSpeed(double a);
	void setTurnSpeed(double b);
	void setAttackCoolDown(double cooldown);
	void setIsCollision(string dir);
};

