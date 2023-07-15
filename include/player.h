#pragma once
#include "../include/stdafx.h"

enum PLAYER_ANIMATION_STATES {IDLE = 0, MOVING_LEFT = 1, MOVING_RIGHT = 2, JUMPING = 3};
typedef enum PLAYER_DIRECTION {LEFT, RIGHT} Direction;

class Player
{
public:
  Player();
  virtual ~Player();

  //Accessors
  const bool& getAnimSwitch();
  const sf::Vector2f getPosition() const;
  const sf::Vector2i getCoords() const;
  sf::Vector2f getVelocity();
  const sf::FloatRect getGlobalBounds() const;
  const Direction getFacing() const;

  //Modifiers
  void setPosition(const float x, const float y);
  void resetVelocityY();
  void resetVelocityX();

  void resetAnimTimer();
  void move(const float dir_x, const float dir_y);
  void update();
  void animate();
  void render(sf::RenderTarget& target);
  void updateMovement();
  void updateAnimations();
  void updatePhysics();
  void jump();
  void collide();

private:
  sf::Sprite m_sprite;
  sf::Texture m_tex_sheet;

  // Anim
  short m_anim_state;
  sf::IntRect m_curr_frame;
  sf::Clock m_anim_timer;
  bool m_anim_switch;
  float m_anim_speed;

  Direction facing;

  // Physics
  sf::Vector2f m_velocity;
  float m_velocity_max;
  float m_velocity_min;
  float m_acceleration;
  float m_drag;
  float m_gravity;
  float m_velocityMaxY;

  bool m_is_airborne;

  // Move


  // Core

  void initTexture();
  void initSprite();
  void initAnimations();
  void initPhysics();
};
