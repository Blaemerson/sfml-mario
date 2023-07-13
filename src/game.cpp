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

	/* bool space_held = false; */
void Game::update()
{
  while (window.pollEvent(event))
  {
    switch (event.type)
    {
      case sf::Event::Closed:
        window.close();
        break;
      case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Q)
        {
          window.close();
        }
        else if (event.key.code == sf::Keyboard::Space)
        {
          player->jump();
          // std::cout << player->getPosition().x << std::endl;
        }
        break;
      case sf::Event::KeyReleased:
        if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D)
        {
          player->resetAnimTimer();
        }
        break;
      default:
        break;
    }
  }

  updatePlayer();
  updateView();
  updateColision();
}

/*************************************/
/*               TIME                */
/*************************************/

void Game::restartClock()
{
  m_elapsed += m_clock.restart();
}

sf::Time Game::getElapsed()
{
  return m_elapsed;
}

void Game::render()
{

  window.clear(sf::Color::Cyan);

  window.setView(view);

  // Draw game objects
  renderPlayer();
  renderMap();

  window.setView(window.getDefaultView());

  window.display();
}

void Game::updatePlayer()
{
  player->update();
}

void Game::updateView()
{
  std::cout << player->getPosition().x << std::endl;
  if (player->getPosition().x > (48 * 13) && player->getPosition().x < (48 * LEVEL_1_WIDTH) - (48 * 27)) {
    view.setCenter(player->getPosition().x+(48 * 7), window.getSize().y/2.f);
  } else if (player->getPosition().x > (48 * LEVEL_1_WIDTH) - (48 * 27)) {
    view.setCenter(48 * 76, window.getSize().y/2.f);
  }
  else {
    view.setCenter(48 * 20, window.getSize().y/2.f);
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
  const unsigned int tileSize = player->getGlobalBounds().height;
  int index = (player_coords.x + (LEVEL_1_WIDTH * ((window.getSize().y / tileSize) - player_coords.y)));
  // std::cout << player_coords.y << std::endl;
  if (LEVEL_1[index] == 1 ) {
    std::cout << index << std::endl;
    player->resetVelocityY();
    player->setPosition(player->getPosition().x, window.getSize().y - tileSize - (tileSize * ((window.getSize().y / tileSize) - (player_coords.y - 1))));
  }
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
  window.create(sf::VideoMode(1200, 16 * 50), "MyGame", sf::Style::Close | sf::Style::Titlebar);
  window.setFramerateLimit(60);
  window.setKeyRepeatEnabled(false);

}

void Game::initPlayer()
{
  player = new Player();
}

void Game::initMap()
{

  tilemap = new TileMap("textures/tilesheet.png", *LEVEL_1);
  if (!tilemap->load(sf::Vector2u(32, 32), LEVEL_1_WIDTH, 8)) {
        std::cout << "notloaded tilesheet" << std::endl;
        return;
    }
}
