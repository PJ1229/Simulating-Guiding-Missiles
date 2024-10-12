#include "Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

//INITIALIZING FUNCTIONS
//initializes textures
void Game::initTextures()
{
	//initializes the bullet textures
	this->bulletTextures["bullet"] = new sf::Texture();
	this->bulletTextures["bullet"]->loadFromFile("bullet.png");

	//initializes the enemy textures
	this->enemyTextures["enemy"] = new sf::Texture();
	this->enemyTextures["enemy"]->loadFromFile("fighterjet.png");
}
//initializes variables
void Game::initVariables()
{
	this->window = nullptr;

	//speed of game
	this->time = 1;
	this->isBulletTime = false;

	//respawn time for enemys
	this->respawnCoolDownMax = 50.f;
	this->respawnCoolDown = this->respawnCoolDownMax;

	//wall respawn time
	this->wallCoolDownMax = 30.f;
	this->wallCoolDown = this->wallCoolDownMax;

	this->makeWall = false;
	this->walls.push_back(new Wall());
}
//initializes the window
void Game::initWindow()
{
	//initializes window
	this->videoMode.height = 1900;
	this->videoMode.width = 2800;
	this->window = new sf::RenderWindow(this->videoMode, "main");

	//framerate is >144
	this->window->setFramerateLimit(144);
}
void Game::initPlayer()
{
	//initializes player
	this->player = new Player();
}
void Game::initTarget()
{
	//initializes target
	this->target = new Target();
}
void Game::initPathFinding()
{
	this->pathfinding = new PathFinding();
}


//CONSTRUCTORS & DESTRUCTORS
Game::Game() 
{
	//calls all initializng functions
	this->initVariables();
	this->initWindow();
	this->initTextures();
	this->initPlayer();
	this->initTarget();
	this->initPathFinding();

}
Game::~Game() 
{
	//deletes window & player & target
	delete this->window;
	delete this->player;
	delete this->target;
	delete this->pathfinding;

	//BULLET
	//delete bullet textures
	for (auto& i : this->bulletTextures)
	{
		delete i.second;
	}
	//delete bullet objects
	for (auto *i : this->bullets)
	{
		delete i;
	}

	//ENEMY
	//delete enemy textures
	for (auto& i : this->enemyTextures)
	{
		delete i.second;
	}
	//delete enemy objects
	for (auto* i : this->enemies)
	{
		delete i;
	}

	//WALLS
	for (auto* i : this->walls)
	{
		delete i;
	}
}

//ACCESSORS
const bool Game::getWindowIsOpen() const
{
	return this->window->isOpen();
}


//FUNCTION
//checks if window is open, closes window with esc key
void Game::pollEvents()
{
	//event polling
	while (this->window->pollEvent(this->ev)) 
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape) 
				this->window->close();
			break;
		}
	}

}

//TIME
void Game::bulletTime(double speed)
{
	this->player->setMovementSpeed(2 * speed);
	this->player->setTurnSpeed(5 * speed);
	this->player->setAttackCoolDown(50.f * (1/ speed)); //50.f * (1/ speed)
	for (auto* bullet : this->bullets)
	{
		bullet->setMovementSpeed(round(20 * speed)); //20 * speed
	}
	for (auto* enemy : this->enemies)
	{
		enemy->setMovementSpeed(4 * speed); //1.5 * speed
	}
}

void Game::updateBulletTime()
{
	if (this->isBulletTime)
	{
		this->bulletTime(0.1); //0.1
	}
	else
	{
		this->bulletTime(1);
	}
}

//update button & mouse inputs
void Game::updateInput()
{
	//player inputs (WSAD)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->player->rotateBody(0);
		if (this->playerDirection == "false")
		{
			this->playerDirection = "left";
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		this->player->rotateBody(180);
		if (this->playerDirection == "false")
		{
			this->playerDirection = "right";
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		this->player->rotateBody(90);
		if (this->playerDirection == "false")
		{
			this->playerDirection = "up";
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		this->player->rotateBody(270);
		if (this->playerDirection == "false")
		{
			this->playerDirection = "down";
		}
	}

	
	//SET TARGET
	//if the mouse button is pressed and the bullet cooldown is 0, then
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->target->updateCoolDown() && !this->makeWall)
	{
		if (this->target->isTargetingEnemy()) //if the target is targeting the enemy
		{
			this->target->setIsTargetingEnemy(false); //then it changes it's status to not targetting
		}
		else
		{
			for (auto* enemy : this->enemies) //iteraters through every enemy
			{
				if (enemy->isTouching(this->target->getPosWindow())) //if the mouse is not targetting an enemy, and it is touching an enemy
				{
					this->target->setIsTargetingEnemy(true); //then it changes it's status to targetting an enemy
					this->target->setEnemyTargetedID(enemy->getEnemyID()); //and it tracks the specific enemy needed to track by getting the enemy ID and setting it to the target

					this->pathfinding->clear();
				}
			}
		}
	}
	

	//SPAWN BULLETS 
	// CURRENTLY NOT USING BULLETS UNTIL CREATE PURSUIT CURVE FUNCTION
	//bullet inputs (space)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->player->updateCoolDown() && !this->makeWall)
	{
		this->bullets.push_back(new Bullet(this->bulletTextures["bullet"], this->player->getPosition().x, this->player->getPosition().y, this->player->getDirection()));
		if (this->target->isTargetingEnemy())
		{
			this->pathfinding->clear();
			this->callPathFinding();
			this->bullets[this->bullets.size() - 1]->setPathFinding(this->pathfinding->getShortestPath(), this->pathfinding->getPathLength());
			this->bullets[this->bullets.size() - 1]->setIsTargeting(true);
		}
	}

	//SPAWN ENEMIES
	//temporary
	//spawn enemy with Q
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && this->respawnCoolDown == this->respawnCoolDownMax && !this->makeWall)
	{
		this->enemies.push_back(new Enemy(this->enemyTextures["enemy"], this->getMousePosWindow().x, this->getMousePosWindow().y, 0));
		this->respawnCoolDown = 0;
	}

	//SPAWN WALLS
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && this->wallCoolDown == this->wallCoolDownMax)
	{
		if (makeWall)
		{
			if (this->walls.size() > 1)
			{
				this->walls[walls.size() - 1]->createWall();
			}
			this->makeWall = false;
		}
		else
		{
			this->walls.push_back(new Wall());
			this->makeWall = true;
		}
		this->wallCoolDown = 0;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->wallCoolDown == this->wallCoolDownMax && this->makeWall)
	{
		if (this->walls.size() > 1)
		{
			this->walls[walls.size() - 1]->createPoint(sf::Vector2f(this->getMousePosWindow().x, this->getMousePosWindow().y));
			this->wallCoolDown = 0;
		}
	}

	//BULLET TIME
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && this->respawnCoolDown == this->respawnCoolDownMax)
	{
		if (!this->isBulletTime)
		{
			this->respawnCoolDown = 0;
			this->isBulletTime = true;
		}
		else
		{
			this->respawnCoolDown = 0;
			this->isBulletTime = false;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && this->respawnCoolDown == this->respawnCoolDownMax)
	{

		this->respawnCoolDown = 0;
		for (auto i : this->walls)
		{
			i->clear();
		}
		this->pathfinding->clear();
		this->pathfinding->clearObstacles();

	}
}


//updates the bullet object
void Game::updateBullet()
{
	//initializes counter
	unsigned counter1 = 0;

	//iterates through all bullets
	for (auto* bullet : this->bullets)
	{
		if (bullet->getIsTargeting() && !this->target->isTargetingEnemy())
		{
			bullet->setIsTargeting(false);
		}

		if (bullet->getIsPursuit())
		{
			bullet->setEnemyPosWindow(target->getPosWindow());
		}

		//if targeting enemy
		if (this->target->isTargetingEnemy())
		{
			//bullet->setPathFinding(this->pathfinding->getShortestPath());

			for (auto* enemy : this->enemies)
			{
				if (enemy->getEnemyID() == this->target->getEnemyTargetedID())
				{

					if (bullet->update()) //calls upon bullet->update
					{

						//deletes sprite
						delete this->bullets.at(counter1);

						//deletes texture
						this->bullets.erase(this->bullets.begin() + counter1);
						--counter1;
					}
				}
			}
		}
		else if (bullet->update()) //set parameter to mousePosWindow for no reason
		{
			//deletes sprite
			delete this->bullets.at(counter1);

			//deletes texture
			this->bullets.erase(this->bullets.begin() + counter1);
			--counter1;
		}

		//COLLISION DETECTION
		//iterates through all the enemies
		unsigned counter2 = 0;
		for (auto* enemy : this->enemies)
		{
			//if within enemy hitbox range, deletes enemy
			//enemy->isTouching(bullet->getPosWindow())
			if (enemy->isTouching(bullet->getPosWindow()))
			{

				//resets isTargetingEnemy function from target
				if (enemy->getEnemyID() == this->target->getEnemyTargetedID() && this->target->isTargetingEnemy())
				{
					this->target->setIsTargetingEnemy(false);
				}
				
				//deletes enemy
				delete this->enemies.at(counter2);
				this->enemies.erase(this->enemies.begin() + counter2);

				//deletes bullet
				delete this->bullets.at(counter1);
				this->bullets.erase(this->bullets.begin() + counter1);

				//reduces counters
				--counter1;
				--counter2;
			}
			//counts how many enemies iterated through
			++counter2;
			
		}
		//counts how many bullets iterated through
		++counter1;
		
	}
}

//updates enemy object
void Game::updateEnemy()
{
	//initializes counter
	unsigned counter = 0;

	//iterates through all bullets
	for (auto* enemy : this->enemies)
	{

		//if enemy is out of bound
		if (enemy->update())
		{
			//deletes sprite
			delete this->enemies.at(counter);

			//deletes texture
			this->enemies.erase(this->enemies.begin() + counter);
			--counter;
		}

		//counts how many enemies iterated through
		++counter;
	}

	//spawn cooldown
	if (this->respawnCoolDown < this->respawnCoolDownMax)
	{
		++this->respawnCoolDown;
	}
}

void Game::updateTarget()
{
	if (this->target->isTargetingEnemy())
	{
		for (auto* enemy : this->enemies)
		{
			if (target->getEnemyTargetedID() == enemy->getEnemyID())
			{
				this->target->update(enemy->getPosWindow());
			}
		}
	}
	else
	{
		this->target->update(sf::Vector2f(this->getMousePosWindow()));
	}
}

void Game::updatePlayer()
{

	unsigned counter = 0;

	if (counter == 0)
	{
		this->playerDirection = "false";
	}

	this->player->setIsCollision(this->playerDirection);


	if (!this->target->isTargetingEnemy())
	{
		this->player->rotateHead(this->getMousePosWindow().x, this->getMousePosWindow().y);
	}
	else
	{
		this->player->rotateHead(this->target->getPosWindow().x, this->target->getPosWindow().y);
	}
	this->player->update();
}

void Game::updateWall()
{
	if (this->wallCoolDown < this->wallCoolDownMax)
	{
		++this->wallCoolDown;
	}

	
	for (auto* wall : this->walls)
	{

		unsigned counter = 0;
		for (auto* bullet : this->bullets)
		{
			if (wall->isTouching(bullet->getPosWindow()) && !bullet->getIsTargeting())
			{
				delete this->bullets.at(counter);
				this->bullets.erase(this->bullets.begin() + counter);
				--counter;
			}
			++counter;
		}
	}
	
}

void::Game::callPathFinding()
{
	if (this->target->isTargetingEnemy())
	{
		//setting start point
		this->pathfinding->setStartPoint(this->player->getPosition());

		//setting end point
		for (auto* enemy : this->enemies)
		{
			if (target->getEnemyTargetedID() == enemy->getEnemyID())
			{
				this->pathfinding->setEndPoint(enemy->getPosWindow());
			}
		}

		if (!this->makeWall && this->walls.size() > 0 )
		{
			for (auto* wall : this->walls)
			{
				this->pathfinding->addObstacle(wall->getWall());
			}
		}
	}
	if (this->pathfinding->calculateShortestPath())
	{
		this->target->setIsTargetingEnemy(false);
	}
}

//updates mouse position
void Game::updateMousePosWindow()
{
	//set mouse position relative to window
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
}

//updates game
void Game::update() 
{
	//key press & mouse updates
	this->updateInput();
	this->pollEvents();
	this->updateMousePosWindow();
		
	//updates bullet
	this->updateBullet();

	//updates enemy
	this->updateEnemy();

	//updates player
	this->updatePlayer();

	//updates target
	this->updateTarget();

	//updates walls
	this->updateWall();

	//updates bullet time
	this->updateBulletTime();

}

//renders game
//layer 1: background
//layer 2: bullets
//layer 3: player
//layer 4: walls
//layer 5: enemy
//layer 6: target
//layer 7: pathfinding
void Game::render() 
{
	//clears window
	if (this->makeWall && this->isBulletTime)
	{
		this->window->clear(sf::Color{ 50,50,50,0 });
	}
	else if (this->makeWall)
	{
		this->window->clear(sf::Color{ 75,75,75,0 });
	}
	else if (this->isBulletTime)
	{
		this->window->clear(sf::Color{ 0,0, 0,0 });
	}
	else
	{
		this->window->clear(sf::Color{ 25,25,25,0 });
	}

	//renders path
	if (this->target->isTargetingEnemy())
	{
		this->pathfinding->render(*this->window);
	}


	//for every bullet object, calls render
	for (auto* bullet : this->bullets)
	{
		bullet->render(*this->window);
	}

	//calls render from player
	this->player->render(*this->window);

	for (auto* wall : this->walls)
	{
		wall->render(*this->window);
	}

	//for every enemy object, calls render
	for (auto* enemy : this->enemies)
	{
		enemy->render(*this->window);
	}

	//calls render from target
	this->target->render(*this->window);

	//displays renders
	this->window->display();
}

//returns mouse position
sf::Vector2i Game::getMousePosWindow() 
{
	return this->mousePosWindow;
}