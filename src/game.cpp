#include "../include/stdafx.h"
#include "../include/game.hpp"
#include "../include/levels.h"
#include <iostream>

#define DEBUG 0

Game::Game()
{
  textbox.Setup(5, 14, 350, sf::Vector2f(0, -10));

  // initWindow();
  InitializeView();
  InitializePlayer();
  InitializeMap();
}

Game::~Game()
{
  delete player;
  delete tilemap;
}

void Game::InitializeView()
{
  view.setSize(window.GetSize().x, window.GetSize().y);
  view.setCenter(window.GetSize().x/2.f, window.GetSize().y/2.f);
}

Window* Game::GetWindow()
{
  return &window;
}

void Game::ProcessEvents()
{
  window.Update();
}

	/* bool space_held = false; */
void Game::Update(sf::Time timeDelta)
{
  player->Update(timeDelta);
  window.Update();
  UpdateView();
  UpdateCollision();
}

/*************************************/
/*               TIME                */
/*************************************/

sf::Time Game::RestartClock()
{
  return clock.restart();
}

void Game::Render()
{

  window.beginDraw();
  window.setView(view);

  // Draw game objects
  RenderTileMap();
  RenderPlayer();

  textbox.add("Player facing = " + std::to_string(player->getFacing()));
  textbox.add("Player real x = " + std::to_string(player->GetPosition().x) + " real y = " + std::to_string(player->GetPosition().y));
  textbox.add("Player vel x = " + std::to_string(player->GetVelocity().x));
  textbox.add("Player vel y = " + std::to_string(player->GetVelocity().y));
  textbox.render(*window.GetTarget());
  textbox.setScreenPos(sf::Vector2f(view.getCenter().x - (window.GetSize().x / 2.f), 0));

  window.endDraw();
}


void Game::UpdateView()
{
  const float diff = 2.5;
  const unsigned int scroll_begin = 48 * diff * 4;
  const unsigned int level_edge_left = 48 * (diff * 4);
  const unsigned int scroll_end = 48 * (LEVEL_1_WIDTH - (diff * 4));
  const unsigned int level_edge_right = 48 * (LEVEL_1_WIDTH - (diff * 4));

  if (player->GetPosition().x < scroll_begin) {
    view.setCenter(level_edge_left, window.GetSize().y / 2.f);
  } else if (player->GetPosition().x > scroll_end) {
    view.setCenter(level_edge_right, window.GetSize().y / 2.f);
  } else {
    view.setCenter(player->GetPosition().x, window.GetSize().y / 2.f);
  }
}

void Game::UpdateCollision()
{
  if (player->GetPosition().y + player->GetGlobalBounds().height > window.GetSize().y)
  {
    player->ResetVelocityY();
    player->SetPosition(player->GetPosition().x, window.GetSize().y - player->GetGlobalBounds().height);

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
      const sf::FloatRect tile_down = tilemap->GetTile(down_idx);
      if (tile_down.intersects(player->GetGlobalBounds())) {
        player->ResetVelocityY();
        player->SetPosition(player->GetPosition().x, tile_down.top - 48);
        player->Collide();
      }
    }
  }

  if (up_idx > -1 && up_idx < max_tiles) {
    if (LEVEL_1[up_idx] == 1) {
      if (player->GetVelocity().y < 0.f) {
        const sf::FloatRect tile_up = tilemap->GetTile(up_idx);
        if (tile_up.intersects(player->GetGlobalBounds())) {
          player->SetPosition(player->GetPosition().x, tile_up.top + 48);
          player->ResetVelocityY();
          player->Collide();
        }
      }
    }
  }

  if (player_idx > -1 && player_idx < max_tiles) {
     if( LEVEL_1[player_idx] == 1) {
      const sf::FloatRect tile_player = tilemap->GetTile(player_idx);
      if (tile_player.intersects(player->GetGlobalBounds())) {
        player->SetPosition(player->GetPosition().x, tile_player.top + (player->GetVelocity().y < 0 ? 48 : -48));
        player->ResetVelocityY();
        player->Collide();
      }
    }
  }

  if (left_idx > -1 && left_idx < max_tiles) {
    if (LEVEL_1[left_idx] == 1) {
      const sf::FloatRect tile_left = tilemap->GetTile(left_idx);
      if (tile_left.intersects(player->GetGlobalBounds())) {
        if (player->GetVelocity().x < 0) {
          player->ResetVelocityX();
        }
        player->SetPosition(tile_left.left + 48, player->GetPosition().y);
        player->Collide();
      }
    }
  }

  if (right_idx > -1 && right_idx < max_tiles) {
    if (LEVEL_1[right_idx] == 1) {
      const sf::FloatRect tile_right = tilemap->GetTile(right_idx);
      if (tile_right.intersects(player->GetGlobalBounds())) {
        if (player->GetVelocity().x > 0 ) {
          player->ResetVelocityX();
        }
        player->SetPosition(tile_right.left - 48, player->GetPosition().y);
        player->Collide();
      }
    }
  }

}

void Game::RenderPlayer()
{
  player->render(*window.GetTarget());
}

void Game::RenderTileMap()
{
  sf::RenderStates states;
  tilemap->draw(*window.GetTarget(), states);
}


void Game::InitializeWindow() {
  // window.create(sf::VideoMode(16 * 60, 16 * 50), "MyGame", sf::Style::Close | sf::Style::Titlebar);
}

void Game::InitializePlayer()
{
  player = new Player();
}

void Game::InitializeMap()
{
  tilemap = new TileMap("../textures/tilesheet.png", *LEVEL_1, LEVEL_1_WIDTH, LEVEL_1_HEIGHT, 16);
  if(!tilemap->Load())
  {
    std::cout << "notloaded tilesheet" << std::endl;
    return;
  }
}
