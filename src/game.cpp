#include "../include/stdafx.h"
#include "../include/game.h"
#include "../include/levels.h"

Game::Game()
{
  initWindow();
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
// Accessors
const sf::RenderWindow& Game::getWindow() const
{
  return window;
}
void Game::processEvents()
{
  while (window.pollEvent(event))
  {
    switch (event.type)
    {
      case sf::Event::Closed:
      {
        window.close();
        break;
      }
      case sf::Event::KeyPressed:
      {
        if (event.key.code == sf::Keyboard::Q)
        {
          window.close();
        }
        else if (event.key.code == sf::Keyboard::Space)
        {
          player->jump();
        }
        break;
      }
      case sf::Event::KeyReleased:
      {
        if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D)
        {
          player->resetAnimTimer();
        }
        if (event.key.code == sf::Keyboard::N)
        {
          delete player;
          initPlayer();
        }
        break;
      }
      default:
      {
        break;
      }
    }
  }
}

	/* bool space_held = false; */
void Game::update(sf::Time deltaTime)
{
  player->update(deltaTime);
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

  window.clear(sf::Color::Cyan);

  window.setView(view);

  // Draw game objects
  renderMap();
  renderPlayer();

  window.setView(window.getDefaultView());

  window.display();
}


void Game::updateView()
{
  const float diff = 2.5;
  const unsigned int scroll_begin = 48 * diff * 4;
  const unsigned int level_edge_left = 48 * (diff * 4);
  const unsigned int scroll_end = 48 * (LEVEL_1_WIDTH - (diff * 4));
  const unsigned int level_edge_right = 48 * (LEVEL_1_WIDTH - (diff * 4));
  if (player->getPosition().x < scroll_begin) {
    view.setCenter(level_edge_left, (LEVEL_1_HEIGHT * 48) / 3.25);
  } else if (player->getPosition().x > scroll_end) {
    view.setCenter(level_edge_right, (LEVEL_1_HEIGHT * 48) / 3.25);
  } else {
    view.setCenter(player->getPosition().x, (LEVEL_1_HEIGHT * 48) / 3.25);
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
  int index_down = player_coords.x + (LEVEL_1_WIDTH) * (player_coords.y);
  int index_right = (player_coords.x + 1) + (LEVEL_1_WIDTH) * (player_coords.y - 1);
  int index_left = (player_coords.x - 1) + (LEVEL_1_WIDTH) * (player_coords.y - 1);
  int index_up = (player_coords.x) + (LEVEL_1_WIDTH) * (player_coords.y - 2);

  if (LEVEL_1[index_down] == 1 )
  {
    const sf::FloatRect tile_down = tilemap->getTile(index_down);
    player->resetVelocityY();
    player->setPosition(player->getPosition().x, tile_down.getPosition().y - player->getGlobalBounds().height);
  }
  if (LEVEL_1[index_up] == 1) {
    const sf::FloatRect tile_up = tilemap->getTile(index_up);
    // std::cout << "diff     = " << player->getPosition().y - (tile_up.getPosition().y + 48) << std::endl;
    if (player->getVelocity().y < 0.f && (player->getPosition().y - (tile_up.getPosition().y + 48)) < 10.0) {
      player->resetVelocityY();
      player->setPosition(player->getPosition().x, tile_up.getPosition().y + player->getGlobalBounds().height);
      player->collide();
    }
  }
  if (LEVEL_1[index_right] == 1)
  {
    const sf::FloatRect tile_right = tilemap->getTile(index_right);
    if (((tile_right.getPosition().x - player->getGlobalBounds().width) - player->getPosition().x) < 0.0) {
      player->resetVelocityX();
      player->setPosition(tile_right.getPosition().x - player->getGlobalBounds().width, player->getPosition().y);
      player->collide();
    }
  }
  if (LEVEL_1[index_left] == 1)
  {
    const sf::FloatRect tile_left = tilemap->getTile(index_left);
    if ((player->getPosition().x - (tile_left.getPosition().x + player->getGlobalBounds().width)) < 0.0 ) {
      player->resetVelocityX();
      player->setPosition(tile_left.getPosition().x + player->getGlobalBounds().width, player->getPosition().y);
      player->collide();
    }
  }

    // for (int i = 0; i < LEVEL_1_HEIGHT; i++) {
    //   for (int j = 0; j < LEVEL_1_WIDTH; j++) {
    //     int idx = j + (LEVEL_1_WIDTH * i);
    //     if (idx == indexLeft + 1) {
    //       std::cout << "M";
    //     }
    //     else {
    //       std::cout << LEVEL_1[j + (LEVEL_1_WIDTH * i)];
    //     }
    //   }
    //   std::cout << std::endl;
    // }
    //
    // std::cout << "Player x real = " << player->getPosition().x << " real y = " << player->getPosition().y << std::endl;
    // std::cout << "Player x = " << player_coords.x << " y = " << player_coords.y << std::endl;
    // std::cout << "Player vel x = " << player->getVelocity().x << std::endl;
    // std::cout << "Player vel y = " << player->getVelocity().y << std::endl;
}


void Game::renderPlayer()
{
  player->render(window);
}

void Game::renderMap()
{
  sf::RenderStates states;
  tilemap->draw(window, states);
}


void Game::initWindow()
{
  window.create(sf::VideoMode(16 * 60, 16 * 50), "MyGame", sf::Style::Close | sf::Style::Titlebar);
  window.setFramerateLimit(60);
  window.setKeyRepeatEnabled(false);

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
