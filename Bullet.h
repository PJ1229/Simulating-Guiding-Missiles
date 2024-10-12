#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Bullet
{
private:
	//bullet sprite
	sf::Sprite bulletSprite;
	bool isDeleted;

	//tracking
	std::vector<sf::Vertex> tracking;

	//variables
	double variableTurn;

	//pathfinding
	int pathIts;
	std::vector<sf::Vertex> path;
	std::vector<float> length;
	std::vector<float> pathTurns;
	std::vector<int> thetaIteration;
	float targetAngle;
	float angle(sf::Vector2f pointA, sf::Vector2f pointB, sf::Vector2f pointC);

	//movement speed
	float maxMovementSpeed;
	float movementSpeed;
	float maxTurn;

	//time and position
	double time;
	std::vector<sf::Vector2f*> posTable;
	sf::Vector2f posWindow(int frames);

	//direction
	double theta;
	double initTheta;
	std::vector<double*> thetaTable;
	void boundTheta();

	//targeting
	bool isTargeting;
	bool isPursuit;
	float pursuitDegrees;
	sf::Vector2f targetPosWindow;
	sf::Vector2f enemyPosWindow;


	//pathfinding
	void updatePath();

	//functions
	void initVariables();
	void updatePos();
	void updateTheta();

public:
	//bullet constructor & destructor
	Bullet(sf::Texture* texture, float tankX, float tankY, float tankDir);
	virtual ~Bullet();

	//update
	bool update();

	//position function
	sf::Vector2f getPosWindow();

	//targeting functions
	void setIsTargeting(bool targeting);
	void setTargetPosWindow(sf::Vector2f target);
	void setEnemyPosWindow(sf::Vector2f enemy);
	void setPathFinding(std::vector<sf::Vertex> path, std::vector<float> length);
	bool getIsTargeting();
	bool getIsPursuit();
	sf::Vector2f getTargetPosWindow();

	//render
	void render(sf::RenderTarget& target);

	//set movement speed
	void setMovementSpeed(float a);
};

