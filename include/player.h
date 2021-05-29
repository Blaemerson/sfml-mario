#pragma once
#include "../include/stdafx.h"

enum PLAYER_ANIMATION_STATES {IDLE = 0, MOVING_LEFT = 1, MOVING_RIGHT = 2, JUMPING = 3};

class Player
{
public:
  Player();
  virtual ~Player();

  //Accessors
  const bool& getAnimSwitch();
  const sf::Vector2f getPosition() const;
  sf::Vector2f getVelocity();
  const sf::FloatRect getGlobalBounds() const;

  //Modifiers
  void setPosition(const float x, const float y);
  void resetVelocityY();

  void resetAnimTimer();
  void move(const float dir_x, const float dir_y);
  void update();
  void render(sf::RenderTarget& target);
  void updateMovement();
  void updateAnimations();
  void updatePhysics();
  void jump();
  void collide();

private:
  sf::Sprite sprite;
  sf::Texture textureSheet;

  // Anim
  short animState;
  sf::IntRect currentFrame;
  sf::Clock animTimer;
  bool animSwitch;
  float anim_speed;

  float space_held = 0.f;

  // Physics
  sf::Vector2f velocity;
  float velocityMax;
  float velocityMin;
  float acceleration;
  float drag;
  float gravity;
  float velocityMaxY;

  bool isAirborne;

  // Move


  // Core

  void initVars();
  void initTexture();
  void initSprite();
  void initAnimations();
  void initPhysics();
};

