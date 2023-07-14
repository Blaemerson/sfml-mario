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
  // std::cout << player->getPosition().x << std::endl;
    // view.setCenter(player->getPosition().x+(48 * 7), window.getSize().y/2.f);
  if (player->getPosition().x < (48 * 5)) {
    view.setCenter((48 * 12.5), (window.getSize().y / 48) + 256);
  } else if (player->getPosition().x > (48 * 76)) {
    view.setCenter((48 * 83.5), (window.getSize().y / 48) + 256);
  } else {
    view.setCenter(player->getPosition().x+(48 * 7.5), (window.getSize().y / 48) + 256);
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

  if (LEVEL_1[index] == 1 )
  {
    player->resetVelocityY();
    player->setPosition(player->getPosition().x, tilemap->getTile(index).getPosition().y - player->getGlobalBounds().height);
  }
  if (LEVEL_1[indexRight] == 1 && player->getVelocity().x > 0.f && ((tilemap->getTile(indexRight).getPosition().x - player->getGlobalBounds().width) - player->getPosition().x) < 0.0)
  {
    player->resetVelocityX();
    player->setPosition(tilemap->getTile(indexRight).getPosition().x - player->getGlobalBounds().width, player->getPosition().y);
  }
  else if (LEVEL_1[indexLeft] == 1 && player->getVelocity().x < 0.f && (player->getPosition().x - (tilemap->getTile(indexLeft).getPosition().x + player->getGlobalBounds().width)) < 0.0 )
  {
    player->resetVelocityX();
    player->setPosition(tilemap->getTile(indexLeft).getPosition().x + player->getGlobalBounds().width, player->getPosition().y);
  }


    // for (int i = 0; i < 8; i++) {
    //   for (int j = 0; j < LEVEL_1_WIDTH; j++) {
    //     if (j + (LEVEL_1_WIDTH * i) == index || j + (LEVEL_1_WIDTH * i) == indexRight || j + (LEVEL_1_WIDTH * i) == indexLeft) {
    //       std::cout << "M";
    //     }
    //     else {
    //       std::cout << LEVEL_1[j + (LEVEL_1_WIDTH * i)];
    //     }
    //   }
    //   std::cout << std::endl;
    // }
    //
    // std::cout << "player x = " << player_coords.x << " y = " << player_coords.y << std::endl;
    // std::cout << "Player pos y = " << player->getPosition().y << std::endl;
    // std::cout << "Standing on tile pos y = " << tilemap->getTile(index).getPosition().y << std::endl;
    // std::cout << "Standing on tile pos x = " << tilemap->getTile(index).getPosition().x << std::endl;
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

  tilemap = new TileMap("textures/tilesheet.png", *LEVEL_1, LEVEL_1_WIDTH, LEVEL_1_HEIGHT);
  if (!tilemap->load(sf::Vector2u(32, 32))) {
        std::cout << "notloaded tilesheet" << std::endl;
        return;
    }
}
