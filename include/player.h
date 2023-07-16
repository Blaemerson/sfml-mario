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
  void move(sf::Vector2f movement, sf::Time deltaTime);
  void update(sf::Time deltaTime);
  void updateMovement(sf::Time deltaTime);
  void updateAnimations();
  void updatePhysics();
  void animate();
  void render(sf::RenderTarget& target);
  void jump();
  void collide();

private:
  sf::Sprite sprite;
  sf::Texture tex_sheet;

  // Anim
  short anim_state;
  sf::IntRect curr_frame;
  sf::Clock anim_timer;
  bool anim_switch;
  float anim_speed;

  Direction facing;

  // Physics
  sf::Vector2f velocity;
  float velocity_max;
  float velocity_min;
  float acceleration;
  float drag;
  float gravity;
  float velocityMaxY;

  bool is_airborne;

  // Move


  // Core

  void initTexture();
  void initSprite();
  void initAnimations();
  void initPhysics();
};
