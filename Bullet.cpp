#include "Bullet.h"

//INITATE FUNCTIONS
void Bullet::initVariables()
{
	this->isTargeting = false;
	this->isPursuit = false;
	this->time = 0;

	//this->variableTurn = 55;
	this->pursuitDegrees = 100;
}

//BULLET CONSTRUCTOR
Bullet::Bullet(sf::Texture* texture, float tankX, float tankY, float tankDir)
{
	//initializes all of the bullet features
	this->bulletSprite.setTexture(*texture);
	this->bulletSprite.setOrigin(2.5f, 2.5f);
	this->bulletSprite.setScale(4.f, 4.f);

	//other variables
	this->isDeleted = false;
	this->initVariables();

	//initial the direction
	this->initTheta = tankDir;
	double* pushBack = new double(tankDir);
	this->thetaTable.push_back(pushBack);
	targetAngle = initTheta;

	//initiate the position
	this->posTable.insert(this->posTable.begin(), new sf::Vector2f(tankX * -cos(tankDir * 0.0174533), tankY * -sin(tankDir * 0.0174533)));
	this->posTable.push_back(new sf::Vector2f(tankX + 25 * -cos(tankDir * 0.0174533), tankY + 25 * -sin(tankDir * 0.0174533)));
	this->posTable.push_back(new sf::Vector2f(tankX + 50 * -cos(tankDir * 0.0174533), tankY + 50 * -sin(tankDir * 0.0174533)));
}

//BULLET DESTRUCTOR
Bullet::~Bullet()
{
	//deletes all positions in the position table
	//cout << "position:" << endl;
	for (auto* i : this->posTable)
	{
		//cout << "x:" << round(i->x) << " y:" << round(i->y) << endl;
		delete i;
	}

	//cout << "theta:" << endl;
	for (auto* i : this->thetaTable)
	{
		//cout << round(*i) << " degrees" << endl;
		delete i;
	}
}

//BULLET FUNCTIONS
//outputs the position
sf::Vector2f Bullet::posWindow(int posNum)
{
	sf::Vector2f prevPos = sf::Vector2f(this->posTable[posNum - 1]->x, this->posTable[posNum - 1]->y);
	sf::Vector2f currentPos = sf::Vector2f(this->movementSpeed * -cos(0.0174533 * this->theta) + prevPos.x, this->movementSpeed * -sin(0.0174533 * this->theta) + prevPos.y);
	return currentPos;
}
//updates position
void Bullet::updatePos()
{
	this->posTable.push_back(new sf::Vector2f(this->posWindow(this->posTable.size()).x, this->posWindow(this->posTable.size()).y));
	this->tracking.push_back(sf::Vertex(this->getPosWindow(), sf::Color::Red));
	this->bulletSprite.setPosition(this->getPosWindow().x, this->getPosWindow().y);
}

void Bullet::boundTheta()
{
	if (this->thetaTable.size() > 3)
	{
		double prevTheta = *this->thetaTable[this->thetaTable.size() - 1];
		double currentTheta = this->theta;
		double dTdt = currentTheta - prevTheta;
		maxTurn = length[pathIts - 1] / 65;
		
		if (dTdt > maxMovementSpeed / maxTurn)
		{
			this->theta = prevTheta + maxMovementSpeed / maxTurn;
		}
		if (dTdt < - maxMovementSpeed / maxTurn)
		{
			this->theta = prevTheta - maxMovementSpeed / maxTurn;
		}
	}
}
//updates direction
void Bullet::updateTheta()
{

	this->bulletSprite.setRotation(this->theta);
	double* pushBack = new double(this->theta);
	this->thetaTable.push_back(pushBack);
	this->thetaIteration.push_back(pathIts);
	//cout << "theta:" << *this->thetaTable[this->thetaTable.size() - 1] << " size:" << this->thetaTable.size() << " iteration:" << this->thetaIteration[thetaIteration.size() - 1] << " size:" << this->thetaIteration.size() << endl;


	if (!this->isTargeting)
	{
		this->theta = this->initTheta;
		this->movementSpeed = this->maxMovementSpeed;
	}
	else
	{
		double dydt = this->targetPosWindow.y - this->posWindow(this->posTable.size()).y;
		double dxdt = this->targetPosWindow.x - this->posWindow(this->posTable.size()).x;
		double dydx = dydt / dxdt;

		double prevIteration = this->thetaIteration[this->thetaIteration.size() - 2];
		double currentIteration = this->thetaIteration[this->thetaIteration.size() - 1];
		double dIdt = currentIteration - prevIteration;


		//need a target theta!!!
		if (this->targetPosWindow.x > this->posWindow(this->posTable.size()).x)
		{
			this->theta = 57.2958 * atan(dydx) + 180;
		}
		else
		{
			this->theta = 57.2958 * atan(dydx);
		}
		if (dIdt == 1)
		{
			targetAngle = this->theta;

			if (!isPursuit)
			{
				//cout << "turn #" << pathIts - 1 << " " << maxTurn << " = " << length[pathIts - 1] << " / " << variableTurn << " pathTurns:" << this->pathTurns[pathIts] << endl;
			}
		}
		this->updatePath();
		if (!this->isPursuit)
		{
			this->variableTurn = 1;
			this->boundTheta();
		}
	}
}
//made a bool in order to return if out of bounds (true) or not (false)
bool Bullet::update()
{	
	//updates frames
	this->time += this->movementSpeed/20;

	if ((this->getPosWindow().x > 2800 || this->getPosWindow().y > 1900 || this->getPosWindow().x < 0 || this->getPosWindow().y < 0) && this->time >= 100 && !this->isTargeting)
	{
		//sets deleted variable to true and returns true
		this->isDeleted = true;
		return true;
	}

	//updates direction
	this->updateTheta();

	//updates position
	this->updatePos();

	return false;
}

void Bullet::updatePath()
{
	this->movementSpeed = round(this->maxMovementSpeed * 0.5);
	//this->movementSpeed = 0;
	if (!this->isPursuit)
	{

		this->targetPosWindow = sf::Vector2f(this->path[this->pathIts].position.x, this->path[this->pathIts].position.y);

		float dist = sqrt((this->targetPosWindow.x - this->getPosWindow().x) * (this->targetPosWindow.x - this->getPosWindow().x) + (this->targetPosWindow.y - this->getPosWindow().y) * (this->targetPosWindow.y - this->getPosWindow().y));

		//this->movementSpeed = round(this->maxMovementSpeed * 0.5);

		if (dist <= 20 && this->pathIts < this->path.size() - 3)
		{

			++this->pathIts;
			//++this->pathIts;
		}
		if (this->pathIts == this->path.size() - 3)
		{
			this->movementSpeed = 0;
			this->isPursuit = true;
		}
	}
	else
	{
		//this->movementSpeed = this->maxMovementSpeed;
		this->targetPosWindow = this->enemyPosWindow;
	}
}
void Bullet::setIsTargeting(bool targeting)
{
	if (!targeting && this->thetaTable.size() > 3)
	{
		this->initTheta = this->theta;
	}
	this->isTargeting = targeting;
}
bool Bullet::getIsTargeting()
{
	return this->isTargeting;
}
bool Bullet::getIsPursuit()
{
	return this->isPursuit;
}

void Bullet::setTargetPosWindow(sf::Vector2f target)
{
	this->targetPosWindow = target;
}
void Bullet::setEnemyPosWindow(sf::Vector2f enemy)
{
	this->enemyPosWindow = enemy;
}
sf::Vector2f Bullet::getTargetPosWindow()
{
	return this->targetPosWindow;
}
void Bullet::setPathFinding(std::vector<sf::Vertex> path, std::vector<float> length)
{
	this->path.clear();
	this->pathIts = 1;
	thetaIteration.push_back(pathIts);
	if (path.size() > 0)
	{
		this->path = path;
		cout << "angles" << endl;
		for (int i=0; i < this->path.size() - 3; i++)
		{
			sf::Vector2f A = sf::Vector2f(path[i].position.x, path[i].position.y);
			sf::Vector2f B = sf::Vector2f(path[i + 1].position.x, path[i + 1].position.y);
			sf::Vector2f C = sf::Vector2f(path[i + 2].position.x, path[i + 2].position.y);
			pathTurns.push_back(round(18000 - 100 * angle(A, B, C)) / 100);
			cout << pathTurns[pathTurns.size() - 1] << endl;
		}
		cout << endl;
	}

	if (length.size() > 0)
	{
		this->length = length;
	}
}
sf::Vector2f Bullet::getPosWindow()
{
	//accounts for out of bounds vector table
	if (!this->isDeleted)
	{
		return this->posWindow(this->posTable.size());
	}
	else
	{
		return sf::Vector2f(-99999.f, -99999.f);
	}
}
//renders bullet, draws from window reference
void Bullet::render(sf::RenderTarget& target)
{
	target.draw(&tracking[0], tracking.size(), sf::LineStrip);

	//draws in window
	target.draw(this->bulletSprite);
}
//setter functions for bullet time
void Bullet::setMovementSpeed(float a)
{
	this->maxMovementSpeed = a;
}

#include <SFML/System/Vector2.hpp>
#include <cmath> // for sqrt and acos

float Bullet::angle(sf::Vector2f pointA, sf::Vector2f pointB, sf::Vector2f pointC)
{
	// Calculate distances
	float distanceAB = sqrt((pointA.x - pointB.x) * (pointA.x - pointB.x) + (pointA.y - pointB.y) * (pointA.y - pointB.y));
	float distanceCB = sqrt((pointC.x - pointB.x) * (pointC.x - pointB.x) + (pointC.y - pointB.y) * (pointC.y - pointB.y));
	float distanceAC = sqrt((pointA.x - pointC.x) * (pointA.x - pointC.x) + (pointA.y - pointC.y) * (pointA.y - pointC.y));

	// Check for division by zero
	if (distanceAB == 0 || distanceCB == 0) {
		return 0; // Return some default value or handle this case based on your requirements
	}

	// Calculate cosine value
	float cosValue = (distanceAB * distanceAB + distanceCB * distanceCB - distanceAC * distanceAC) / (2 * distanceAB * distanceCB);

	// Check if the cosine value is within valid range [-1, 1]
	if (cosValue <= -1 || cosValue >= 1) {
		// Handle invalid case, maybe return a default value or handle it based on your requirements
		return 180; // Default value
	}

	// Calculate angle in degrees
	float angleABC = 57.2958 * acos(cosValue);

	return angleABC;
}
