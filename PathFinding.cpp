#include "PathFinding.h"
PathFinding::PathFinding()
{
	this->initVariables();
}
void PathFinding::clear()
{
	this->points.clear();

	this->visionPoints.clear();

	this->shortestPath.clear();

	this->pathLength.clear();

	this->convexHullPoints.clear();

	this->vision.clear();

	this->pathBounds.clear();
}

void PathFinding::clearObstacles()
{
	this->obstacles.clear();

	this->convexHullPoints.clear();
}
void PathFinding::initVariables()
{
	this->maxVision = 600;
	this->minVision = 100;
}

bool PathFinding::calculateShortestPath() {

	this->points.clear();
	sf::Vector2f A = startPoint;
	sf::Vector2f B = endPoint;
	shortestPath.push_back(sf::Vertex(A, sf::Color::Yellow));
	double distFrom = distance(A, B);

	const int height = 100;
	const int width = 100;
	double dist[height][width];
	bool towardsWall[height][width];

	while (distFrom >= 10) {
		calculateVision(A);
		pathLength.push_back(maxTouchRadius);
		sf::Vector2f output;
		const double movementSpeed = ((maxTouchRadius * 2) - 5) / height;
		initPathBounds(A, maxTouchRadius - 5);
		float prevDistFrom = distFrom;

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				sf::Vector2f newPos = { (float)movementSpeed * (x - (width / 2)) + A.x, (float)movementSpeed * (y - (height / 2)) + A.y };
				this->points.push_back(sf::Vertex(newPos, sf::Color::Red));
				//&& withinObject(newPos, pathBounds)

				if (!withinObject(newPos, vision) && distance(newPos, B) < distFrom)
				{
					if (withinObject(newPos, pathBounds))
					{
						distFrom = distance(newPos, B);
						output = newPos;
					}
				}
				else if (withinObject(newPos, vision))
				{
					this->points[points.size() - 1] = sf::Vertex(newPos, sf::Color::Blue);
				}

			}
		}
		if (prevDistFrom == distFrom)
		{
			return true;
		}
		A = output;
		shortestPath.push_back(sf::Vertex(A, sf::Color::Yellow));
	}
	shortestPath.push_back(sf::Vertex(B, sf::Color::Yellow));
	pathLength.push_back(maxTouchRadius);
	return false;
}

std::vector<sf::Vertex> PathFinding::getShortestPath()
{
	if (this->shortestPath.size() > 0)
	{
		return this->shortestPath;
	}
	return {};
}
std::vector<float> PathFinding::getPathLength()
{
	if (this->pathLength.size() > 0)
	{
		return this->pathLength;
	}
	return {};
}

void PathFinding::setStartPoint(sf::Vector2f a)
{
	this->startPoint = a;
}
void PathFinding::setEndPoint(sf::Vector2f a)
{
	this->endPoint = a;
}
void PathFinding::addObstacle(std::vector<sf::Vertex> a)
{
	this->obstacles.push_back(a);
	this->convexHullPoints.push_back(calculateConvexHullPoints(a));
}

void PathFinding::update()
{

}
void PathFinding::render(sf::RenderTarget& target)
{
	if (this->points.size() > 0)
	{
		target.draw(&points[0], points.size(), sf::Points);
	}
	if (this->visionPoints.size() > 0)
	{
		//target.draw(&visionPoints[0], visionPoints.size(), sf::LinesStrip);
	}
	if (this->shortestPath.size() > 0)
	{
		target.draw(&shortestPath[0], shortestPath.size(), sf::LinesStrip);
	}
	for(auto i : pathBounds)
	{
		//target.draw(&i[0], i.size(), sf::Points);
	}
}


//software stuff :)
float PathFinding::angle(sf::Vector2f pointA, sf::Vector2f pointB, sf::Vector2f pointC){float distanceAB = sqrt((pointA.x - pointB.x) * (pointA.x - pointB.x) + (pointA.y - pointB.y) * (pointA.y - pointB.y)); float distanceCB = sqrt((pointC.x - pointB.x) * (pointC.x - pointB.x) + (pointC.y - pointB.y) * (pointC.y - pointB.y)); float distanceAC = sqrt((pointA.x - pointC.x) * (pointA.x - pointC.x) + (pointA.y - pointC.y) * (pointA.y - pointC.y)); float angleABC = 57.2958 * acos((distanceAB * distanceAB + distanceCB * distanceCB - distanceAC * distanceAC) / (2 * distanceAB * distanceCB)); return angleABC;}
bool PathFinding::direction(sf::Vector2f pointA, sf::Vector2f pointB, sf::Vector2f pointC){float ta_x = pointA.x - pointB.x; float ta_y = pointA.y - pointB.y; float tc_x = pointC.x - pointB.x; float tc_y = pointC.y - pointB.y; float cross = tc_y * ta_x - tc_x * ta_y; return cross < 0;}
bool PathFinding::withinObject(sf::Vector2f point, const std::vector<std::vector<sf::Vertex>>& objects) {
    for (const auto& object : objects) {
        if (object.size() < 3) // Skip objects with less than 3 vertices (not a closed shape)
            continue;

        bool inside = false;
        for (size_t i = 0, j = object.size() - 1; i < object.size(); j = i++) {
            if (((object[i].position.y > point.y) != (object[j].position.y > point.y)) &&
                (point.x < (object[j].position.x - object[i].position.x) * (point.y - object[i].position.y) / 
                          (object[j].position.y - object[i].position.y) + object[i].position.x)) {
                inside = !inside;
            }
        }
        if (inside) {
            return true;
        }
    }
    return false;
}
std::vector<sf::Vertex> PathFinding::calculateConvexHullPoints(std::vector<sf::Vertex>& obstaclePoints) {
	std::vector<sf::Vertex> convexHull;
	if (obstaclePoints.size() <= 2) {
		// Convex hull not possible with less than 3 points
		return convexHull;
	}

	// Find the leftmost point
	sf::Vertex leftmost = obstaclePoints[0];
	for (const auto& point : obstaclePoints) {
		if (point.position.x < leftmost.position.x) {
			leftmost = point;
		}
	}

	convexHull.push_back(leftmost);

	auto nextToTop = [&convexHull](int k) -> sf::Vertex& {
		return convexHull[convexHull.size() - k - 1];
		};

	auto top = [&convexHull]() -> sf::Vertex& {
		return convexHull[convexHull.size() - 1];
		};

	auto secondTop = [&convexHull]() -> sf::Vertex& {
		return convexHull[convexHull.size() - 2];
		};

	std::sort(obstaclePoints.begin(), obstaclePoints.end(), [](const sf::Vertex& a, const sf::Vertex& b) {
		return (a.position.x == b.position.x) ? (a.position.y < b.position.y) : (a.position.x < b.position.x);
		});

	for (int i = 1; i < obstaclePoints.size(); ++i) {
		while (convexHull.size() > 1 &&
			orientation(secondTop(), top(), obstaclePoints[i]) != 2) {
			convexHull.pop_back();
		}
		convexHull.push_back(obstaclePoints[i]);
	}

	obstaclePoints = convexHull;
	return convexHull;
}

int PathFinding::orientation(const sf::Vertex& p1, const sf::Vertex& p2, const sf::Vertex& p3) {
	float val = (p2.position.y - p1.position.y) * (p3.position.x - p2.position.x) -
		(p2.position.x - p1.position.x) * (p3.position.y - p2.position.y);

	if (val == 0) return 0;  // Collinear
	return (val > 0) ? 1 : 2; // Clockwise or Counterclockwise
}

float PathFinding::distance(sf::Vector2f A, sf::Vector2f B)
{
	float x2 = (A.x - B.x) * (A.x - B.x);
	float y2 = (A.y - B.y) * (A.y - B.y);
	float distance = sqrt(x2 + y2);
	return distance;
}


bool PathFinding::outOfBounds(sf::Vector2f point)
{
	if (point.y > 1900 || point.y < 0 || point.x >2800 || point.x < 0) //1900,0 2800,0
	{
		return true;
	}
	return false;
}
void PathFinding::calculateVision(sf::Vector2f playerPos)
{
	this->vision.clear();
	std::vector<double> maxDist;
	std::vector<double> degreesDetected;
	const int maxIterations = 36;

	sf::Vector2f start = playerPos;
	bool touchingObject = false;
	std::vector<std::string> type;
	std::vector<sf::Vector2f> pos;
	float numTouching = 0;


	sf::Vertex iterator = sf::Vertex(sf::Vector2f(start));
	for (int degrees = 0; degrees < maxIterations; degrees++)
	{
		float iterations = 0;
		iterator = sf::Vertex(sf::Vector2f(start));
		while (!withinObject(sf::Vector2f(iterator.position.x, iterator.position.y), this->obstacles) && !outOfBounds(sf::Vector2f(iterator.position.x, iterator.position.y)))
		{
			iterations++;
			iterator.position.x = -cos(degrees * (360 / maxIterations) * 0.0174533) * iterations + start.x;
			iterator.position.y = -sin(degrees * (360 / maxIterations) * 0.0174533) * iterations + start.y;
		}
		if (withinObject(sf::Vector2f(iterator.position.x, iterator.position.y), this->obstacles))
		{
			maxDist.push_back(distance(sf::Vector2f(iterator.position.x, iterator.position.y), start));
			//degreesDetected.push_back(degrees * (360 / maxIterations));
		}
	}
	if (maxDist.size() > 0)
	{
		for (auto i : maxDist)
		{
			this->maxTouchRadius += i;
		}
		this->maxTouchRadius = round(this->maxTouchRadius / maxDist.size());
	}

	//bounds
	if (this->maxTouchRadius < minVision)
	{
		this->maxTouchRadius = minVision;
	}
	if (this->maxTouchRadius > maxVision / 2)
	{
		this->maxTouchRadius = maxVision / 2;
	}


	iterator = sf::Vertex(sf::Vector2f(start), sf::Color::Green);
	for (int degrees = 0; degrees < maxIterations; degrees++)
	{
		float iterations = 0;
		iterator = sf::Vertex(sf::Vector2f(start), sf::Color::Green);
		//!withinObject(sf::Vector2f(iterator.position.x, iterator.position.y)) && 
		while (!withinObject(sf::Vector2f(iterator.position.x, iterator.position.y), this->obstacles) && !outOfBounds(sf::Vector2f(iterator.position.x, iterator.position.y)) && iterations <= maxTouchRadius * 2)
		{
			iterations++;
			iterator.position.x = -cos(degrees * (360/ maxIterations) * 0.0174533) * iterations + start.x;
			iterator.position.y = -sin(degrees * (360 / maxIterations) * 0.0174533) * iterations + start.y;
		}
		if (withinObject(sf::Vector2f(iterator.position.x, iterator.position.y), this->obstacles))
		{
			type.push_back("touching");
			pos.push_back(sf::Vector2f(iterator.position.x, iterator.position.y));
			degreesDetected.push_back(degrees * (360 / maxIterations));
			++numTouching;
		}
		else
		{
			type.push_back("not touching");
			pos.push_back(sf::Vector2f(iterator.position.x, iterator.position.y));
		}
	}

	std::vector<std::vector<float>> storage;
	for (int i = 0; i < maxIterations; i++)
	{

		if ((i != 0 && type[i] == "touching" && type[i - 1] == "not touching") || (i == 0 && type[i] == "touching" && type[maxIterations - 1] == "not touching"))
		{
			//std::cout << "working" << std::endl;
			storage.resize(storage.size() + 1);
			storage[storage.size() - 1].push_back(i - 1);
			int j = i;
			int add = 0;
			while (type[j] == "touching")
			{
				j++;
				if (j == maxIterations)
				{
					j = 0;
				}
				add++;
				storage[storage.size() - 1].push_back(j);
			}
			i += add;
		}
	}

	this->vision.resize(storage.size());
	for (int i = 0; i < storage.size(); i++)
	{
			this->vision[i].resize(storage[i].size() + 2);
			sf::Vertex temp = sf::Vertex(start, sf::Color(255, 255, 255, 50));
			this->vision[i][0] = temp;
			this->visionPoints.push_back(temp);
			for (int j = 0; j < storage[i].size(); j++)
			{
				temp.position.x = -cos(storage[i][j] * (360 / maxIterations) * 0.0174533) * (this->maxTouchRadius * 2) + start.x;
				temp.position.y = -sin(storage[i][j] * (360 / maxIterations) * 0.0174533) * (this->maxTouchRadius * 2) + start.y;
				this->vision[i][j + 1] = temp;
				this->visionPoints.push_back(temp);
			}
			this->vision[i][storage[i].size() + 1] = sf::Vertex(start, sf::Color(255, 255, 255, 50));
			this->visionPoints.push_back(sf::Vertex(start, sf::Color(255, 255, 255, 50)));
	}
}
void PathFinding::initPathBounds(sf::Vector2f start, float radius)
{
	this->pathBounds.clear();
	float maxIterations = 36;
	sf::Vertex iterator;
	this->pathBounds.resize(1);
	for (int degrees = 0; degrees < maxIterations; degrees++)
	{
		iterator.position.x = -cos(degrees * (360 / maxIterations) * 0.0174533) * radius + start.x;
		iterator.position.y = -sin(degrees * (360 / maxIterations) * 0.0174533) * radius + start.y;
		this->pathBounds[0].push_back(iterator);
	}
	iterator.position.x = -cos(0) * radius + start.x;
	iterator.position.y = -sin(0) * radius + start.y;
	this->pathBounds[0].push_back(iterator);
}