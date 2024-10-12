#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <map>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Target.h"
#include "Wall.h"
#include "PathFinding.h"

class Game
{
private:
	//window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	//resources
	std::map<std::string, sf::Texture*> bulletTextures;
	std::vector<Bullet*> bullets;
	std::map<std::string, sf::Texture*> enemyTextures;
	std::vector<Enemy*> enemies;
	std::vector<Wall*> walls;

	//enemy respawn cooldown
	double respawnCoolDown;
	double respawnCoolDownMax;
	double wallCoolDown;
	double wallCoolDownMax;

	//objects
	Player* player;
	Bullet* bullet;
	Enemy* enemy;
	Target* target;
	Wall* wall;
	PathFinding* pathfinding;

	//mouse positions
	sf::Vector2i mousePosWindow;

	//private functions
	void initVariables();
	void initWindow();
	void initPlayer();
	void initTarget();
	void initTextures();
	void initPathFinding();

	//time
	double time;
	void bulletTime(double speed);
	bool isBulletTime;
	
	//wall
	bool makeWall;
	string playerDirection;

public:
	//variables
	sf::Vector2f windowSize;

	//constructors & destructors
	Game();
	virtual ~Game();

	//accessors
	const bool getWindowIsOpen() const;
	sf::Vector2i getMousePosWindow();

	//functions
	void pollEvents();
	void update();
	void updateInput();
	void updateBullet();
	void updateEnemy();
	void updateTarget();
	void updatePlayer();
	void updateWall();
	void updateBulletTime();
	void updateMousePosWindow();
	void callPathFinding();
	void render();
};