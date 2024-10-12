#include "Wall.h"

void Wall::initTexture()
{
	if (!this->pointTexture.loadFromFile("point.png"))
	{
		//error
	}
}

//CONSTRUCTORS & DESTRUCTORS
Wall::Wall()
{
	this->declaredWall = false;

	this->initTexture();

	this->pointSprite.setTexture(this->pointTexture);
	this->pointSprite.setOrigin(2.5f, 2.5f);
	this->pointSprite.scale(4.f, 4.f);

}
Wall::~Wall()
{

}

void Wall::createPoint(sf::Vector2f pos)
{
	this->walls.push_back(sf::Vertex(pos));
}

void Wall::createWall()
{
	this->hitbox.setOutlineColor(sf::Color::White);
	this->hitbox.setOutlineThickness(1.f);
	this->hitbox.setFillColor(sf::Color::Transparent);
	this->hitbox.setPointCount(this->walls.size());
	for (int i=0; i<this->walls.size(); i++)
	{
		float x = this->walls[i].position.x;
		float y = this->walls[i].position.y;
		this->hitbox.setPoint(i, sf::Vector2f(x, y));
	}
	this->declaredWall = true;
}

void Wall::clear()
{
	this->walls.clear();
	this->declaredWall = false;
}
void Wall::update()
{
	
}

//law of cosines
float Wall::angleABC(sf::Vector2f pointA, sf::Vector2f pointB, sf::Vector2f pointC)
{
	float distanceAB = sqrt((pointA.x - pointB.x) * (pointA.x - pointB.x) + (pointA.y - pointB.y) * (pointA.y - pointB.y));
	float distanceCB = sqrt((pointC.x - pointB.x) * (pointC.x - pointB.x) + (pointC.y - pointB.y) * (pointC.y - pointB.y));
	float distanceAC = sqrt((pointA.x - pointC.x) * (pointA.x - pointC.x) + (pointA.y - pointC.y) * (pointA.y - pointC.y));

	float angleABC = 57.2958 * acos((distanceAB * distanceAB + distanceCB * distanceCB - distanceAC * distanceAC) / (2 * distanceAB * distanceCB));
	return angleABC;
}


//I don't understand how this works, research more...
bool Wall::clockwiseABC(sf::Vector2f pointA, sf::Vector2f pointB, sf::Vector2f pointC)
{
	float ta_x = pointA.x - pointB.x;
	float ta_y = pointA.y - pointB.y;
	float tc_x = pointC.x - pointB.x;
	float tc_y = pointC.y - pointB.y;
	float cross = tc_y * ta_x - tc_x * ta_y;
	return cross < 0;
}

bool Wall::isTouching(const sf::Vector2f& pos) {
	if (walls.size() < 3) // A polygon must have at least 3 vertices to be valid
		return false;

	bool inside = false;
	sf::Vector2f prevVertex = walls.back().position;

	for (const auto& vertex : walls) {
		sf::Vector2f currentVertex = vertex.position;

		// Check if the point is to the left of the line from prevVertex to currentVertex
		if (((currentVertex.y > pos.y) != (prevVertex.y > pos.y)) &&
			(pos.x < (prevVertex.x - currentVertex.x) * (pos.y - currentVertex.y) / (prevVertex.y - currentVertex.y) + currentVertex.x)) {
			inside = !inside;
		}

		prevVertex = currentVertex;
	}

	return inside;
}


void Wall::render(sf::RenderTarget& target)
{
	if (this->declaredWall)
	{
		target.draw(hitbox);
	}
	else 
	{
		if (this->walls.size() > 0)
		{
			for (int i = 0; i < this->walls.size(); i++)
			{
				float x = this->walls[i].position.x;
				float y = this->walls[i].position.y;
				pointSprite.setPosition(x, y);
				target.draw(pointSprite);
			}
		}
	}
}

std::vector<sf::Vertex> Wall::getWall()
{
	return this->walls;
}