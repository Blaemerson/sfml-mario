#include "../include/stdafx.h"
#include "../include/player.h"
#include <math.h>

Player::Player()
{
  initTexture();
  initSprite();
  initAnimations();
  initPhysics();
}

Player::~Player()
{
}

void Player::update()
{
  updateMovement();
  updateAnimations();
  updatePhysics();
}

/*************************************/
/*             RENDERING             */
/*************************************/
void Player::render(sf::RenderTarget& target)
{
  target.draw(m_sprite);
}

void Player::initTexture()
{
  if (!m_tex_sheet.loadFromFile("textures/mario.png"))
  {
    std::cout << "ERROR in Player.initTexture(): m_tex_sheet failed to load" << std::endl;
  }
}
void Player::initSprite()
{
  m_sprite.setTexture(m_tex_sheet);
  m_curr_frame = sf::IntRect(0, 0, 16, 16);
  m_sprite.setTextureRect(m_curr_frame);
  m_sprite.setScale(3.0f, 3.0f);
}

/*************************************/
/*              PHYSICS              */
/*************************************/
void Player::initPhysics()
{
  m_is_airborne = true;
  m_velocity_max = 4.5f;
  m_velocity_min = 0.f;
  m_acceleration = 0.40f;
  m_drag = 0.94f;
  m_gravity = 3.2f;
  m_velocityMaxY = 22.0f;
}

void Player::updatePhysics()
{
  // if jumping && not falling && spacebar is held: jump higher and stay in air longer
  m_anim_state == JUMPING && m_velocity.y <= 0.f && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ?
    m_velocity.y += (0.1f * m_gravity) * (1.5f * m_acceleration) : m_velocity.y += m_gravity * m_acceleration;

  // limit y velocity
  if (std::abs(m_velocity.y) > m_velocityMaxY)
  {
    m_velocity.y = m_velocityMaxY * ((m_velocity.y < 0.f) ? -1.f : 1.f);
  }

  // decelerate
  m_velocity *= m_drag;

  // limit deceleration
  if (std::abs(m_velocity.x) < m_velocity_min)
  {
    m_velocity.x = 0.f;
  }
  if (std::abs(m_velocity.y) < m_velocity_min)
  {
    m_velocity.y = 0.f;
  }

  m_sprite.move(m_velocity);
}

void Player::collide()
{
}

/*************************************/
/*              MOVEMENT             */
/*************************************/
void Player::move(const float dir_x, const float dir_y)
{
  // m_acceleration
  m_velocity.x += dir_x * m_acceleration;
  m_velocity.y += dir_y * m_acceleration * m_gravity;

  // limit velocity
  if (std::abs(m_velocity.x) > m_velocity_max)
  {
    m_velocity.x = m_velocity_max * ((m_velocity.x < 0.f) ? -1.f : 1.f);
  }
}

void Player::jump()
{
  if (!m_is_airborne)
  {
    m_is_airborne = true;
    m_anim_state = JUMPING;
    m_velocity.y = -sqrt(2.0f) * m_gravity * 10.f;
  }
}

void Player::updateMovement()
{
  if (m_velocity.y == 0.f)
  {
    m_is_airborne = false;
    if (std::abs(m_velocity.x) < 0.45f || m_anim_state == JUMPING) 
    {
      m_anim_state = IDLE;
    }
  }
  else 
  {
    m_is_airborne = true;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
  {
    // If jumping to the left, slow down speed to the right
    m_is_airborne && m_velocity.x > 0.f ? move(-0.2f, 0.f) : move(-1.f, 0.f);
    if (m_anim_state != JUMPING || !m_is_airborne)
    {
      m_anim_state = MOVING_LEFT;
    }
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
  {
    // If jumping to the right, slow down speed to the left
    m_is_airborne && m_velocity.x < 0.f ? move(0.2f, 0.f) : move(1.f, 0.f);
    if (m_anim_state != JUMPING || !m_is_airborne)
    {
      m_anim_state = MOVING_RIGHT;
    }
  }
}

/*************************************/
/*             ANIMATION             */
/*************************************/
void Player::initAnimations()
{
  m_anim_state = IDLE;
  m_anim_speed = 0.22f;
  m_anim_timer.restart();
  m_anim_switch = true;
}

void Player::animate()
{
  if (m_anim_timer.getElapsedTime().asSeconds() >= m_anim_speed || getAnimSwitch())
  {
    m_curr_frame.left += 16.f;
    if (m_curr_frame.left >= 32.f)
    {
      m_curr_frame.left = 0;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
      m_anim_speed -= 0.01f * std::abs(m_velocity.x);
    }
    else
    {
      m_anim_speed += 0.01f * std::abs(m_velocity.x);
    }
    if (m_anim_speed < 0.10f) 
    {
      m_anim_speed = 0.1f;
    }
    // m_anim_timer.restart();
    resetAnimTimer();
    m_sprite.setTextureRect(m_curr_frame);
  }
}

void Player::updateAnimations()
{
  if (m_anim_state == IDLE)
  {
    m_anim_speed = 0.22f;
    m_curr_frame.left = 0;
    m_sprite.setTextureRect(m_curr_frame);
  }
  else if (m_anim_state == MOVING_RIGHT)// || m_velocity.x > 0.6f)
  {
    animate();
    // turn sprite
    if (m_anim_state != JUMPING)
    {
      m_sprite.setScale(3.f, 3.f);
      m_sprite.setOrigin(0.f, 0.f);
    }
  }
  else if (m_anim_state == MOVING_LEFT)// || m_velocity.x < -0.6f)
  {
    animate();
    // turn sprite
    if (m_anim_state != JUMPING)
    {
      m_sprite.setScale(-3.f, 3.f);
      m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 3.f, 0.f);
    }
  }
  else if (m_anim_state == JUMPING)
  {
    m_curr_frame.left = 32.f;
    m_sprite.setTextureRect(m_curr_frame);
  }
}

const bool& Player::getAnimSwitch()
{
  if (m_anim_switch)
  {
    m_anim_switch = false;
  }
  return m_anim_switch;
}

void Player::resetAnimTimer()
{
  m_anim_timer.restart();
  m_anim_switch = true;
}

/*************************************/
/*         GETTERS / SETTERS         */
/*************************************/
const sf::Vector2f Player::getPosition() const
{
  return m_sprite.getPosition();
}
void Player::setPosition(const float x, const float y)
{
  m_sprite.setPosition(x, y);
  m_velocity.y = 0.f;
}

sf::Vector2f Player::getVelocity()
{
  return m_velocity;
}

const sf::FloatRect Player::getGlobalBounds() const
{
  return m_sprite.getGlobalBounds();
}

void Player::resetVelocityY()
{
  m_velocity.y = 0.f;
}
