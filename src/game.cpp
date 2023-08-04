#include "../include/stdafx.h"
#include "../include/game.hpp"
#include "../include/levels.h"
#include <iostream>

#define DEBUG 0

Game::Game()
{
  textbox.setup(5, 14, 350, sf::Vector2f(0, -10));

  // initWindow();
  initView();
  initPlayer();
  initMap();
}

Game::~Game()
{
  delete player;
  delete tilemap;
}

void Game::initView()
{
  view.setSize(window.getSize().x, window.getSize().y);
  view.setCenter(window.getSize().x/2.f, window.getSize().y/2.f);
}

Window* Game::getWindow()
{
  return &window;
}

void Game::processEvents()
{
  window.update();
}

	/* bool space_held = false; */
void Game::update(sf::Time dt)
{
  player->update(dt);
  window.update();
  updateView();
  updateColision();
}

/*************************************/
/*               TIME                */
/*************************************/

sf::Time Game::restartClock()
{
  return clock.restart();
}

void Game::render()
{

  window.beginDraw();
  window.setView(view);

  // Draw game objects
  renderMap();
  renderPlayer();

  textbox.add("Player facing = " + std::to_string(player->getFacing()));
  textbox.add("Player real x = " + std::to_string(player->getPosition().x) + " real y = " + std::to_string(player->getPosition().y));
  textbox.add("Player vel x = " + std::to_string(player->getVelocity().x));
  textbox.add("Player vel y = " + std::to_string(player->getVelocity().y));
  textbox.render(*window.getTarget());
  textbox.setScreenPos(sf::Vector2f(view.getCenter().x - (window.getSize().x / 2.f), 0));

  window.endDraw();
}


void Game::updateView()
{
  const float diff = 2.5;
  const unsigned int scroll_begin = 48 * diff * 4;
  const unsigned int level_edge_left = 48 * (diff * 4);
  const unsigned int scroll_end = 48 * (LEVEL_1_WIDTH - (diff * 4));
  const unsigned int level_edge_right = 48 * (LEVEL_1_WIDTH - (diff * 4));

  if (player->getPosition().x < scroll_begin) {
    view.setCenter(level_edge_left, window.getSize().y / 2.f);
  } else if (player->getPosition().x > scroll_end) {
    view.setCenter(level_edge_right, window.getSize().y / 2.f);
  } else {
    view.setCenter(player->getPosition().x, window.getSize().y / 2.f);
  }
}

void Game::updateColision()
{
  if (player->getPosition().y + player->getGlobalBounds().height > window.getSize().y)
  {
    player->resetVelocityY();
    player->setPosition(player->getPosition().x, window.getSize().y - player->getGlobalBounds().height);

    return;
  }


  sf::Vector2i player_coords = player->getCoords();
  int player_idx = (player_coords.x) + (LEVEL_1_WIDTH) * (player_coords.y - 1);
  int down_idx = player_idx + LEVEL_1_WIDTH;
  int right_idx = player_idx + 1;
  int left_idx = player_idx - 1;
  int up_idx = player_idx - LEVEL_1_WIDTH;

  const int max_tiles = LEVEL_1_HEIGHT * LEVEL_1_WIDTH;

  if (down_idx > -1 && down_idx < max_tiles) {
    if (LEVEL_1[down_idx] == 1) {
      const sf::FloatRect tile_down = tilemap->getTile(down_idx);
      if (tile_down.intersects(player->getGlobalBounds())) {
        player->resetVelocityY();
        player->setPosition(player->getPosition().x, tile_down.top - 48);
        player->collide();
      }
    }
  }

  if (up_idx > -1 && up_idx < max_tiles) {
    if (LEVEL_1[up_idx] == 1) {
      if (player->getVelocity().y < 0.f) {
        const sf::FloatRect tile_up = tilemap->getTile(up_idx);
        if (tile_up.intersects(player->getGlobalBounds())) {
          player->setPosition(player->getPosition().x, tile_up.top + 48);
          player->resetVelocityY();
          player->collide();
        }
      }
    }
  }

  if (player_idx > -1 && player_idx < max_tiles) {
     if( LEVEL_1[player_idx] == 1) {
      const sf::FloatRect tile_player = tilemap->getTile(player_idx);
      if (tile_player.intersects(player->getGlobalBounds())) {
        player->setPosition(player->getPosition().x, tile_player.top + (player->getVelocity().y < 0 ? 48 : -48));
        player->resetVelocityY();
        player->collide();
      }
    }
  }

  if (left_idx > -1 && left_idx < max_tiles) {
    if (LEVEL_1[left_idx] == 1) {
      const sf::FloatRect tile_left = tilemap->getTile(left_idx);
      if (tile_left.intersects(player->getGlobalBounds())) {
        if (player->getVelocity().x < 0) {
          player->resetVelocityX();
        }
        player->setPosition(tile_left.left + 48, player->getPosition().y);
        player->collide();
      }
    }
  }

  if (right_idx > -1 && right_idx < max_tiles) {
    if (LEVEL_1[right_idx] == 1) {
      const sf::FloatRect tile_right = tilemap->getTile(right_idx);
      if (tile_right.intersects(player->getGlobalBounds())) {
        if (player->getVelocity().x > 0 ) {
          player->resetVelocityX();
        }
        player->setPosition(tile_right.left - 48, player->getPosition().y);
        player->collide();
      }
    }
  }

}

void Game::renderPlayer()
{
  player->render(*window.getTarget());
}

void Game::renderMap()
{
  sf::RenderStates states;
  tilemap->draw(*window.getTarget(), states);
}


void Game::initWindow() {
  // window.create(sf::VideoMode(16 * 60, 16 * 50), "MyGame", sf::Style::Close | sf::Style::Titlebar);
}

void Game::initPlayer()
{
  player = new Player();
}

void Game::initMap()
{
  tilemap = new TileMap("textures/tilesheet.png", *LEVEL_1, LEVEL_1_WIDTH, LEVEL_1_HEIGHT, 16);
  if (!tilemap->load()) {
        std::cout << "notloaded tilesheet" << std::endl;
        return;
    }
}
