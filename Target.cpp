#include "Target.h"

//INITIALIZING FUNCTIONS
void Target::initTexture()
{
	if (!this->targetTexture.loadFromFile("target.png"))
	{
		//error
	}
}
void Target::initVariables()
{
	targetingEnemy = false;
	this->targetCoolDownMax = 50;
	this->targetCoolDown = this->targetCoolDownMax;
}
void Target::initTarget()
{
	this->targetSprite.setTexture(this->targetTexture);
	this->targetSprite.setOrigin(7.f, 7.f);
	this->targetSprite.setScale(4.f, 4.f);
}
//TARGET CONSTRUCTOR
Target::Target()
{
	this->initTexture();
	this->initVariables();
	this->initTarget();
}
Target::~Target()
{

}


//TARGET FUNCTIONS

bool Target::updateCoolDown()
{
	if (this->targetCoolDown >= this->targetCoolDownMax)
	{
		this->targetCoolDown = 0;
		return true;
	}
	return false;
}

void Target::setIsTargetingEnemy(bool target)
{
	this->targetingEnemy = target;
}
bool Target::isTargetingEnemy()
{
	return this->targetingEnemy;
}

void Target::setEnemyTargetedID(int ID)
{
	this->enemyTargetedID = ID;
}
int Target::getEnemyTargetedID()
{
	return this->enemyTargetedID;
}

void Target::update(sf::Vector2f position)
{
	this->targetPosWindow.x = position.x;
	this->targetPosWindow.y = position.y;

	++this->targetCoolDown;
	
	this->targetSprite.setPosition(this->targetPosWindow.x, this->targetPosWindow.y);
}
void Target::render(sf::RenderTarget& target)
{
	//draws in window
	target.draw(this->targetSprite);
}
sf::Vector2f Target::getPosWindow()
{
	return this->targetPosWindow;
}