#include "../include/stdafx.h"
#include "../include/player.h"
#include <math.h>

Player::Player()
{
	this->initTexture();
	this->initSprite();
	this->initVars();
	this->initAnimations();
	this->initPhysics();
}

Player::~Player()
{
}

void Player::collide()
{
}

void Player::resetAnimTimer()
{
	this->animTimer.restart();
	this->animSwitch = true;
}

void Player::move(const float dir_x, const float dir_y)
{
	// acceleration
	this->velocity.x += dir_x * this->acceleration;
	this->velocity.y += dir_y * this->acceleration;

	// limit velocity
	if (std::abs(this->velocity.x) > this->velocityMax)
	{
		this->velocity.x = this->velocityMax * ((this->velocity.x < 0.f) ? -1.f : 1.f);
	}
}

const sf::Vector2f Player::getPosition() const
{
	return this->sprite.getPosition();
}

void Player::update()
{
	this->updateMovement();
	this->updateAnimations();
	this->updatePhysics();
}

void Player::jump()
{
	if (!this->isAirborne)
	{
		this->isAirborne = true;
		this->animState = JUMPING;
		this->velocity.y = -sqrt(2.0f) * this->gravity * 10.f;
	}
}

void Player::updateMovement()
{
	if (this->velocity.y == 0.f)
	{
		this->isAirborne = false;
		if (std::abs(this->velocity.x) < 0.65f or this->animState == JUMPING) this->animState = IDLE;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		this->move(-1.f, 0.f);
		if (this->animState != JUMPING || !this->isAirborne)
		{
			this->animState = MOVING_LEFT;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		this->move(1.f, 0.f);
		if (this->animState != JUMPING || !this->isAirborne)
		{
			this->animState = MOVING_RIGHT;
		}
	}
}

sf::Vector2f Player::getVelocity()
{
	return this->velocity;
}

void Player::updateAnimations()
{
	if (this->animState == IDLE)
	{
		this->anim_speed = 0.22f;
		this->currentFrame.left = 0;
		this->sprite.setTextureRect(this->currentFrame);
	}
	else if (this->animState == MOVING_RIGHT || this->velocity.x > 0.65f)
	{
		if (this->animTimer.getElapsedTime().asSeconds() >= anim_speed or getAnimSwitch())
		{
			this->currentFrame.left += 16.f;
			if (this->currentFrame.left >= 32.f)
			{
				this->currentFrame.left = 0;
			}

			/* if (anim_speed > 0.10f) anim_speed -= 0.02f / std::abs(1.f/this->velocity.x); */
			if (anim_speed > 0.10f) anim_speed -= 0.02f * std::abs(this->velocity.x);
			this->animTimer.restart();
			this->sprite.setTextureRect(this->currentFrame);
		}
		// Turn
		this->sprite.setScale(3.f, 3.f);
		this->sprite.setOrigin(0.f, 0.f);
	}
	else if (this->animState == MOVING_LEFT || this->velocity.x < -0.65f)
	{
		if (this->animTimer.getElapsedTime().asSeconds() >= this->anim_speed or getAnimSwitch())
		{
			this->currentFrame.left += 16.f;
			if (this->currentFrame.left >= 32.f)
			{
				this->currentFrame.left = 0;
			}

			// Gradually increase animation speed
			if (anim_speed > 0.10f) anim_speed -= 0.02f / std::abs(1.f/this->velocity.x);

			this->animTimer.restart();
			this->sprite.setTextureRect(this->currentFrame);
		}
		// Turn
		this->sprite.setScale(-3.f, 3.f);
		this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 3.f, 0.f);
	}
	if (this->animState == JUMPING)
	{
		this->currentFrame.left = 32.f;
		this->sprite.setTextureRect(this->currentFrame);
	}
}

void Player::updatePhysics()
{
	// gravity
	this->velocity.y += 0.25f * this->gravity;
	if (std::abs(this->velocity.x) > this->velocityMaxY)
	{
		this->velocity.y = this->velocityMaxY * ((this->velocity.y < 0.f) ? -1.f : 1.f);
	}

	// decel
	this->velocity *= this->drag;

	// limit decel
	if (std::abs(this->velocity.x) < this->velocityMin)
	{
		this->velocity.x = 0.f;
	}
	if (std::abs(this->velocity.y) < this->velocityMin)
	{
		this->velocity.y = 0.f;
	}

	this->sprite.move(this->velocity);
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);

	sf::CircleShape circle;
	circle.setFillColor(sf::Color::Red);
	circle.setRadius(3.f);
	circle.setPosition(this->sprite.getPosition());

	/* target.draw(circle); */
}

void Player::initVars()
{
	this->animState = IDLE;
	this->anim_speed = 0.22f;
}

void Player::initTexture()
{
	if (!this->textureSheet.loadFromFile("textures/mario.png"))
	{
		std::cout << "ERROR in Player: texturesheet failed to load" << std::endl;
	}
}

void Player::initSprite()
{
	this->sprite.setTexture(this->textureSheet);
	this->currentFrame = sf::IntRect(0, 0, 16, 16);
	this->sprite.setTextureRect(this->currentFrame);
	this->sprite.setScale(3.0f, 3.0f);
}

void Player::initAnimations()
{
	this->animTimer.restart();
	this->animSwitch = true;
}

void Player::initPhysics()
{
	this->velocityMax = 3.0f;
	this->velocityMin = 0.05f;
	this->acceleration = 0.35;
	this->drag = 0.97f;
	this->gravity = 1.0f;
	this->velocityMaxY = 3.0f;
}

const bool& Player::getAnimSwitch()
{
	bool anim_switch = this->animSwitch;

	if (this->animSwitch)
	{
		this->animSwitch = false;
	}

	return this->animSwitch;
}


const sf::FloatRect Player::getGlobalBounds() const
{
	return this->sprite.getGlobalBounds();
}

//Modifiers
void Player::setPosition(const float x, const float y)
{
	this->sprite.setPosition(x, y);
	this->velocity.y = 0.f;
}

void Player::resetVelocityY()
{
	this->velocity.y = 0.f;
}
