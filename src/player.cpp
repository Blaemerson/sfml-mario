#include "../include/player.hpp"
#include <math.h>

Player::Player() {
  facing = RIGHT;
  initTexture();
  initSprite();
  initAnimations();
  initPhysics();
}

Player::~Player() {}

void Player::Update(const sf::Time& dt) {
  updateMovement(dt);
  updateAnimations();
  updatePhysics();
}

void Player::render(sf::RenderTarget& target) {
  target.draw(sprite);
}

void Player::initTexture() {
  if (!tex_sheet.loadFromFile("../textures/mario.png")) {
    std::cout << "ERROR in Player.initTexture(): tex_sheet failed to load"
              << std::endl;
  }
}

void Player::initSprite() {
  sprite.setTexture(tex_sheet);
  curr_frame = sf::IntRect(0, 0, 16, 16);
  sprite.setTextureRect(curr_frame);
  sprite.setScale(3.0f, 3.0f);
}

void Player::initPhysics() {
  velocity_x_max = 4.6f;
  velocity_y_max = 18.0f;
  velocity_min = 0.1f;
  acceleration = 0.80f;
  drag = 0.94;
  gravity = 1.5f;
}

void Player::updatePhysics() {

  // if jumping && not falling && spacebar is held: jump higher and stay in air longer
  if (anim_state == JUMPING && velocity.y <= 0.f && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
    if ((facing == LEFT && velocity.x < 0.f) || (facing == RIGHT && velocity.x > 0.f)) {
      velocity.y += 0.06f * gravity * acceleration;
    } else {
      velocity.y += 0.07f * gravity * acceleration;
    }
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
    velocity.y += 0.97 * gravity * acceleration;
  } else {
    velocity.y += gravity * acceleration;
  }

  // limit y velocity
  if (std::abs(velocity.y) > velocity_y_max) {
    velocity.y = velocity_y_max * ((velocity.y < 0.f) ? -1.f : 1.f);
  }

  // decelerate
  velocity *= drag;

  // limit deceleration
  if (std::abs(velocity.x) < velocity_min) {
    velocity.x = 0.f;
  }
  if (std::abs(velocity.y) < 0.0) {
    velocity.y = 0.f;
  }

  sprite.move(velocity);
}

void Player::Collide() {
  is_airborne = true;
}

const Direction Player::getFacing() const { return facing; }

void Player::move(const sf::Vector2f& movement, sf::Time dt) {
  if (anim_state != JUMPING) {
    facing = movement.x > 0 ? RIGHT : LEFT;
  }

  velocity.x += movement.x * acceleration * dt.asSeconds();
  velocity.y += movement.y * acceleration * gravity * dt.asSeconds();

  if (std::abs(velocity.x) > velocity_x_max) {
    velocity.x = velocity_x_max * ((velocity.x < 0.f) ? -1.f : 1.f);
  }
}

void Player::jump() {
  if (!is_airborne) {
    is_airborne = true;
    anim_state = JUMPING;
    velocity.y = -sqrt(2.0f) * gravity * 10.f;
  }
}

void Player::updateMovement(sf::Time dt) {
  sf::Vector2f movement(0.f, 0.f);

  if (velocity.y == 0.f) {
    if (anim_state != JUMPING) {
      is_airborne = false;
    }

    if (std::abs(velocity.x) < 0.45f || anim_state == JUMPING) {
      anim_state = IDLE;
    }
  } else {
    is_airborne = true;
  }

  // TODO: Move input code out of here
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
    // If jumping to the left, slow down speed to the right
    movement.x = (is_airborne && velocity.x > 0.f ? -8.0f : -40.f);
    if (anim_state != JUMPING && !is_airborne) {
      anim_state = MOVING_LEFT;
    }
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
    // If jumping to the right, slow down speed to the left
    movement.x = (is_airborne && velocity.x < 0.f ? 8.0f : 40.f);
    if (anim_state != JUMPING && !is_airborne) {
      anim_state = MOVING_RIGHT;
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
    if (anim_state != JUMPING) {
      std::cout << "jumping" << std::endl;
      jump();
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
    running = true;
    velocity_x_max = 8.0f;
    velocity_y_max = 20.f;
    acceleration = 0.90f;
    anim_speed_min = 0.06f;
  } else {
    initPhysics();

    anim_speed_min = 0.1f;
  }

  move(movement, dt);
}

void Player::initAnimations() {
  anim_state = IDLE;
  anim_speed = 0.22f;
  anim_speed_min = 0.1f;
  anim_timer.restart();
  anim_switch = true;
}

void Player::animate() {
  if (anim_timer.getElapsedTime().asSeconds() >= anim_speed ||
      getAnimSwitch()) {
    curr_frame.left += 16.f;
    if (curr_frame.left >= 32.f) {
      curr_frame.left = 0;
    }

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
         sf::Keyboard::isKeyPressed(sf::Keyboard::D))) {
      anim_speed -= 0.01f * std::abs(velocity.x);
    } else {
      anim_speed += 0.01f * std::abs(velocity.x);
    }

    if (anim_speed < anim_speed_min) {
      anim_speed = anim_speed_min;
    }

    resetAnimTimer();
    sprite.setTextureRect(curr_frame);
  }
}

void Player::updateAnimations() {
  switch (anim_state)
  {
    case (IDLE):
    {
      anim_speed = 0.22f;
      curr_frame.left = 0;
      sprite.setTextureRect(curr_frame);
      break;
    }
    case (MOVING_RIGHT):
    {
      animate();
      // turn sprite
      if (anim_state != JUMPING)
      {
        sprite.setScale(3.f, 3.f);
        sprite.setOrigin(0.f, 0.f);
      }
      break;
    }
    case (MOVING_LEFT):
    {
      animate();
      // turn sprite
      if (anim_state != JUMPING)
      {
        sprite.setScale(-3.f, 3.f);
        sprite.setOrigin(sprite.getGlobalBounds().width / 3.f, 0.f);
      }
      break;
    }
    case (JUMPING):
    {
      curr_frame.left = 32.f;
      sprite.setTextureRect(curr_frame);
      break;
    }
    default: break;
  }
}

const bool &Player::getAnimSwitch() {
  if (anim_switch) {
    anim_switch = false;
  }
  return anim_switch;
}

void Player::resetAnimTimer() {
  anim_timer.restart();
  anim_switch = true;
}

void Player::resetRunTimer() {
  run_timer.restart();
}

const sf::Vector2f Player::GetPosition() const {
  return sprite.getPosition();
}

void Player::SetPosition(const float x, const float y) {
  sprite.setPosition(x, y);
}

const sf::Vector2i Player::getCoords() const {
  return sf::Vector2i(
      std::round(GetPosition().x / 48),
      std::round(((GetPosition().y + (velocity.y <= 0.f ? 24 : 48)) / 48)));
}

const sf::Vector2f Player::GetVelocity() const {
  return velocity;
}

const sf::FloatRect Player::GetGlobalBounds() const {
  return sprite.getGlobalBounds();
}

void Player::ResetVelocityY() { velocity.y = 0.f; }

void Player::ResetVelocityX() { velocity.x = 0.f; }
