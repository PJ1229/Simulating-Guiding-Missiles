#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

//INITATE FUNCTIONS
int Enemy::howManyEnemies;
//initiates variables
void Enemy::initVariables() //eventually completely implement these variables into the game
{
	this->path = "static";
	//paths: static, circle, clover

	this->time = 0;
}

//ENEMY CONSTUCTOR
Enemy::Enemy(sf::Texture* texture, float spawnX, float spawnY, float spawnDir)
{
	//initializes all of the bullet features
	++howManyEnemies;
	this->enemyID = howManyEnemies;
	this->jetSprite.setTexture(*texture);
	this->jetSprite.setOrigin(7.f, 7.f);
	this->jetSprite.setScale(4.f, 4.f);
	this->jetSprite.setRotation(spawnDir + 270);
	
	//initializes functions used for math
	this->enemyInitWindow.x = spawnX; 
	this->enemyInitWindow.y = spawnY;
	this->initTheta = spawnDir;
	
	this->initVariables();

}

//ENEMY DESTRUCTOR
Enemy::~Enemy()
{
	//--howManyEnemies;
}

//MATH
double Enemy::theta(int t)
{
	if (this->path == "static")
	{
		return this->initTheta;
	}
	if(this->path == "circle") //changes theta by 1 degree every frame
	{
		return -1 * t + this->initTheta;
	}
	if (this->path == "clover") //changes theta by 1 degree every frame
	{
		return -0.25 * t + this->initTheta; //-0.25 * t + this->initTheta;
	}
}
sf::Vector2f Enemy::enemyPosWindow(int t) 
{
	if (this->path == "static")
	{
		double x = this->enemyInitWindow.x;
		double y = this->enemyInitWindow.y;
		return sf::Vector2f(x, y);
	}
	if (this->path == "circle") //uses x=rcos(theta) and y=rsin(theta) to move in a circular motion
	{
		double radius = 200;
		double theta = this->theta(t) * 0.0174533;
		double x = radius * cos(theta) + this->enemyInitWindow.x;
		double y = radius * sin(theta) + this->enemyInitWindow.y;
		return sf::Vector2f(x, y);
	}
	if (this->path == "clover")
	{
		double radius = 300;
		double theta = this->theta(t) * 0.0174533;
		double x = radius * cos(theta) * sin(2 * theta) + this->enemyInitWindow.x;
		double y = radius * sin(theta) * sin(2 * theta) + this->enemyInitWindow.y;
		return sf::Vector2f(x, y);
	}
}
void Enemy::direction()
{
	if (this->path == "static")
	{
		this->jetSprite.setRotation(this->theta(this->time));
	}
	if (this->path == "circle")
	{
		this->jetSprite.setRotation(this->theta(this->time) + 90);
	}
	if (this->path == "clover")
	{
		this->jetSprite.setRotation(3 * this->theta(this->time));
	}
}

//ENEMY FUNCTIONS
bool Enemy::update() //change to not bool
{
	//sets position
	this->jetSprite.setPosition(this->enemyPosWindow(this->time).x, this->enemyPosWindow(this->time).y);
	this->direction();

	this->tracking.push_back(sf::Vertex(this->getPosWindow(), sf::Color::Blue));

	//updates frames
	time += movementSpeed;

	//else return false;
	return false;
}

sf::Vector2f Enemy::getPosWindow()
{
	return this->enemyPosWindow(time);
}
bool Enemy::isTouching(sf::Vector2f vector)
{	
	if (jetSprite.getGlobalBounds().contains(vector.x, vector.y))
	{
		return true;
	}
	return false;

}

int Enemy::getEnemyID()
{
	return this->enemyID;
}
int Enemy::getHowManyEnemys()
{
	return this->howManyEnemies;
}

//renders enemy
void Enemy::render(sf::RenderTarget& target)
{
	target.draw(&tracking[0], tracking.size(), sf::LinesStrip);

	target.draw(this->jetSprite);
}

//setter function for bullet time
void Enemy::setMovementSpeed(double a)
{
	this->movementSpeed = a;
}