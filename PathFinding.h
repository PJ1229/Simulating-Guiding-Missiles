#pragma once

#include<SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string_view>
#include <cmath>
#include <math.h>
#include <queue>

class PathFinding
{
private:

	//variables
	sf::Vector2f startPoint;
	sf::Vector2f endPoint;
	std::vector<std::vector<sf::Vertex>> obstacles;
	std::vector<std::vector<sf::Vertex>> convexHullPoints;

	//pathing
	std::vector<std::vector<sf::Vertex>> vision;
	std::vector<sf::Vertex> visionPoints;
	std::vector<sf::Vertex> shortestPath;
	std::vector<sf::Vertex> points;
	std::vector<float> pathLength;


	//bounds
	std::vector<std::vector<sf::Vertex>> pathBounds;
	void initPathBounds(sf::Vector2f start, float radius);

	//vision
	bool outOfBounds(sf::Vector2f point);
	void calculateVision(sf::Vector2f playerPos);
	double maxTouchRadius;
	int maxVision;
	int minVision;

	//calculations
	float angle(sf::Vector2f pointA, sf::Vector2f pointB, sf::Vector2f pointC);
	bool direction(sf::Vector2f pointA, sf::Vector2f pointB, sf::Vector2f pointC);
	float distance(sf::Vector2f A, sf::Vector2f B);
	int orientation(const sf::Vertex& p1, const sf::Vertex& p2, const sf::Vertex& p3);

	//initiate
	void initVariables();

public:
	PathFinding();

	//calculations
	bool calculateShortestPath();
	std::vector<sf::Vertex> calculateConvexHullPoints(std::vector<sf::Vertex>& obstaclePoints);

	//clear functions
	void clear();
	void clearObstacles();

	//setter functions
	void setStartPoint(sf::Vector2f a);
	void setEndPoint(sf::Vector2f a);
	void addObstacle(std::vector<sf::Vertex> a);

	//getter functions
	std::vector<sf::Vertex> getShortestPath();
	std::vector<float> getPathLength();

	//within object functions
	bool withinObject(sf::Vector2f point, const std::vector<std::vector<sf::Vertex>>& objects);

	//game engine
	void update();
	void render(sf::RenderTarget& target);
};

