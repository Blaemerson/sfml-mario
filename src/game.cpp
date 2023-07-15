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
        if (event.key.code == sf::Keyboard::N) {
          delete player;
          initPlayer();
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
  renderMap();
  renderPlayer();

  window.setView(window.getDefaultView());

  window.display();
}

void Game::updatePlayer()
{
  player->update();
}

void Game::updateView()
{
  const float diff = 2.5;
  const unsigned int centerOffset = 48 * (diff * 3);
  const unsigned int scrollBegin = 48 * (diff * 2);
  const unsigned int levelEdgeLeft = 48 * (diff * 5);
  const unsigned int scrollEnd = 48 * (LEVEL_1_WIDTH - (diff * 8));
  const unsigned int levelEdgeRight = 48 * (LEVEL_1_WIDTH - (diff * 5));
  // std::cout << player->getPosition().x << std::endl;
    // view.setCenter(player->getPosition().x+(48 * 7), window.getSize().y/2.f);
  if (player->getPosition().x < scrollBegin) {
    view.setCenter(levelEdgeLeft, (LEVEL_1_HEIGHT * 48) / 3.25);
  } else if (player->getPosition().x > scrollEnd) {
    view.setCenter(levelEdgeRight, (LEVEL_1_HEIGHT * 48) / 3.25);
  } else {
    view.setCenter(player->getPosition().x+centerOffset, (LEVEL_1_HEIGHT * 48) / 3.25);
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
  int index = player_coords.x + (LEVEL_1_WIDTH) * (player_coords.y);
  int indexRight = (player_coords.x + 1) + (LEVEL_1_WIDTH) * (player_coords.y - 1);
  int indexLeft = (player_coords.x - 1) + (LEVEL_1_WIDTH) * (player_coords.y - 1);
  int indexUp = (player_coords.x) + (LEVEL_1_WIDTH) * (player_coords.y - 2);

  if (LEVEL_1[index] == 1 )
  {
    player->resetVelocityY();
    player->setPosition(player->getPosition().x, tilemap->getTile(index).getPosition().y - player->getGlobalBounds().height);
  }
  else if (LEVEL_1[indexUp] == 1 && player->getVelocity().y < 0.f) {
    player->resetVelocityY();
    player->setPosition(player->getPosition().x, tilemap->getTile(indexUp).getPosition().y + player->getGlobalBounds().height);
  }
  if (LEVEL_1[indexRight] == 1 &&  player->getVelocity().x > 0.f && ((tilemap->getTile(indexRight).getPosition().x - player->getGlobalBounds().width) - player->getPosition().x) < 0.0)
  {
    player->resetVelocityX();
    player->setPosition(tilemap->getTile(indexRight).getPosition().x - player->getGlobalBounds().width, player->getPosition().y);
  }
  else if (LEVEL_1[indexLeft] == 1 && player->getVelocity().x < 0.f && (player->getPosition().x - (tilemap->getTile(indexLeft).getPosition().x + player->getGlobalBounds().width)) < 0.0 )
  {
    player->resetVelocityX();
    player->setPosition(tilemap->getTile(indexLeft).getPosition().x + player->getGlobalBounds().width, player->getPosition().y);
  }


    for (int i = 0; i < LEVEL_1_HEIGHT; i++) {
      for (int j = 0; j < LEVEL_1_WIDTH; j++) {
        int idx = j + (LEVEL_1_WIDTH * i);
        if (idx == indexLeft + 1) {
          std::cout << "M";
        }
        else {
          std::cout << LEVEL_1[j + (LEVEL_1_WIDTH * i)];
        }
      }
      std::cout << std::endl;
    }

    std::cout << "Player x = " << player_coords.x << " y = " << player_coords.y << std::endl;
    std::cout << "Player vel x = " << player->getVelocity().x << std::endl;
    std::cout << "Player vel y = " << player->getVelocity().y << std::endl;
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

  tilemap = new TileMap("textures/tilesheet.png", *LEVEL_1, LEVEL_1_WIDTH, LEVEL_1_HEIGHT, 16);
  if (!tilemap->load()) {
        std::cout << "notloaded tilesheet" << std::endl;
        return;
    }
}
