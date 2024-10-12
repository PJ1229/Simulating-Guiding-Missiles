#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <math.h>

class Wall
{
private:
	
	sf::Sprite pointSprite;
	sf::Texture pointTexture;

	std::vector<sf::Vertex> walls;
	
	sf::ConvexShape hitbox;

	bool declaredWall;

	void initTexture();

	float angleABC(sf::Vector2f pointA, sf::Vector2f pointB, sf::Vector2f pointC);

	bool clockwiseABC(sf::Vector2f pointA, sf::Vector2f pointB, sf::Vector2f pointC);

public:


	Wall();
	~Wall();

	void createPoint(sf::Vector2f pos);

	void createWall();

	void clear();

	void update();

	void render(sf::RenderTarget& target);

	std::vector<sf::Vertex> getWall();

	bool isTouching(const sf::Vector2f& pos);
};

