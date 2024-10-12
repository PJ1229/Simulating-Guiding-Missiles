#include "Player.h"

//INITIALIZING FUNCTIONS
void Player::initVariables()
{

}
void Player::initTexture()
{
	//loads tank head and body
	if (!this->tankHeadTexture.loadFromFile("tankhead.png"))
	{
		//error
	}
	if (!this->tankBodyTexture1.loadFromFile("tankbody.png"))
	{
		//error
	}
}

//initializes tank head
void Player::initTankHead() 
{
	//initializing characteristics
	this->tankHeadSprite.setTexture(this->tankHeadTexture);
	this->tankBodyTexture = 1;
	this->tankHeadSprite.setOrigin(7.5f, 7.5f); //set to half of the tankHead size
	this->tankHeadSprite.scale(4.f, 4.f); 
	this->tankHeadPosWindow.x = 2800 /2;
	this->tankHeadPosWindow.y = 1900 /2;
}

//initializes tank body (run under tank head)
void Player::initTankBody()
{
	this->tankBodySprite.setTexture(this->tankBodyTexture1);
	this->tankBodySprite.setOrigin(7.5f, 7.5f); //set to half of the tankHead size
	this->tankBodySprite.scale(4.f, 4.f);
}

//CONSTRUCTORS & DESTRUCTORS
//constructs player
Player::Player() 
{
	//calls initializing functions
	this->initTexture();
	this->initTankHead();
	this->initTankBody();
	this->initVariables();
}

//destructs player
Player::~Player() 
{
	
}

//FUNCTIONS
void Player::update() 
{

	//sets position of head and body using tank head position
	this->tankHeadSprite.setPosition(this->tankHeadPosWindow.x, this->tankHeadPosWindow.y);
	this->tankBodySprite.setPosition(this->tankHeadPosWindow.x, this->tankHeadPosWindow.y);

	//updates cooldown
	++this->attackCoolDown;
}

//if attack is ready, returns true
bool Player::updateCoolDown()
{
	if (this->attackCoolDown >= this->attackCoolDownMax)
	{
		this->attackCoolDown = 0;
		return true;
	}
	return false;
}

//moves player
void Player::move(const float dirX, const float dirY)
{
	//moves player head based of dirX and dirY
	this->tankHeadPosWindow.x += dirX * this->movementSpeed;
	this->tankHeadPosWindow.y += dirY * this->movementSpeed;
}

//rotates head
void Player::rotateHead(const float dirX, const float dirY)
{
	//rotates head based on theta=arctan((y1-y2)/(x1-x2))
	if ((this->tankHeadPosWindow.x > dirX && this->tankHeadPosWindow.y < dirY) || (this->tankHeadPosWindow.x < dirX && this->tankHeadPosWindow.y < dirY)) //quadrant 1 & 2 equation: pi/2+arctan((y1-y2)/(x1-x2))
	{
		this->tankHeadSprite.setRotation(round(-90 + -57.2958 * atan((dirX - this->tankHeadPosWindow.x) / (dirY - this->tankHeadPosWindow.y))));
	}
	else if ((this->tankHeadPosWindow.x < dirX && this->tankHeadPosWindow.y > dirY) || (this->tankHeadPosWindow.x > dirX && this->tankHeadPosWindow.y > dirY)) //quadrant 3 & 4 equation: 3pi/2+arctan((x1-x2)/(y1-y2))
	{
		this->tankHeadSprite.setRotation(round(-270 + -57.2958 * atan((dirX - this->tankHeadPosWindow.x) / (dirY - this->tankHeadPosWindow.y))));
	}
}

//rotates body
void Player::rotateBody(const float direction)
{
	double theta = this->tankBodySprite.getRotation();

	if (this->turnSpeed > 5)
	{
		theta = direction;
		this->tankBodySprite.setRotation(direction);
	}

	//waits until body is rotated to move the sprite
	if (direction == theta)
	{
		//calls move function
		if (direction == 0) //left
		{
			if (this->isCollision != "left")
			{
				this->move(-1, 0);
			}
		}
		if (direction == 180) //right
		{
			if (this->isCollision != "right")
			{
				this->move(1, 0);
			}
		}
		if (direction == 90) //up
		{
			if (this->isCollision != "up")
			{
				this->move(0, -1);
			}
		}
		if (direction == 270)
		{
			if (this->isCollision != "down")
			{
				this->move(0, 1);
			}
		}
	}

	//accounts for error when needing to go to 360 degrees

	else if (theta - direction < -180)
	{
		if (theta == 0)
		{
			theta = 360 - this->turnSpeed;
			this->tankBodySprite.setRotation(round(theta/this->turnSpeed) * this->turnSpeed);
		}
		else
		{
			theta -= this->turnSpeed;
			this->tankBodySprite.setRotation(round(theta / this->turnSpeed) * this->turnSpeed);
		}
	}

	//checks which direction requires less rotation, rotates 2.5 speed
	else if (theta - direction < 0 || this->tankBodySprite.getRotation() - direction > 180) // rotate clockwise
	{
		theta += this->turnSpeed;
		this->tankBodySprite.setRotation(round(theta / this->turnSpeed) * this->turnSpeed);
	}
	else if (theta - direction <= 180 && this->tankBodySprite.getRotation() - direction > 0) //rotate counterclockwise
	{
		theta -= this->turnSpeed;
		this->tankBodySprite.setRotation(round(theta / this->turnSpeed) * this->turnSpeed);
	}
}
//returns position window
sf::Vector2f Player::getPosition()
{
	return this->tankHeadPosWindow;
}

//returns direction window
double Player::getDirection()
{
	return this->tankHeadSprite.getRotation();
}

//renders player
void Player::render(sf::RenderTarget& target)
{
	target.draw(this->tankBodySprite);
	target.draw(this->tankHeadSprite);
}

//setter functions for bullet time
void Player::setMovementSpeed(double a)
{
	this->movementSpeed = a;
}
void Player::setTurnSpeed(double b)
{
	this->turnSpeed = b;
}
void Player::setAttackCoolDown(double cooldown)
{
	this->attackCoolDownMax = cooldown;
}

void Player::setIsCollision(string c)
{
	this->isCollision = c;
}