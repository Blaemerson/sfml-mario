#include "../include/stdafx.h"
#include "../include/game.h"

Game::Game()
{
  this->initWindow();
  this->initView();
  this->initPlayer();
  // this->initMap();
}

Game::~Game()
{
  delete this->player;
  // delete this->tilemap;
}

void Game::initView()
{
  this->view.setSize(this->window.getSize().x, this->window.getSize().y);
  this->view.setCenter(this->window.getSize().x/2.f, this->window.getSize().y/2.f);
}
// Accessors
const sf::RenderWindow& Game::getWindow() const
{
  return this->window;
}

	/* bool space_held = false; */
void Game::update()
{
  dt = dt_clock.restart().asSeconds();
  while (this->window.pollEvent(this->event))
  {
    switch (this->event.type)
    {
      case sf::Event::Closed:
        this->window.close();
        break;
      case sf::Event::KeyPressed:
        if (this->event.key.code == sf::Keyboard::Q)
        {
          this->window.close();
        }
        else if (this->event.key.code == sf::Keyboard::Space)
        {
          this->player->jump();
          std::cout << this->player->getPosition().x << std::endl;
        }
        break;
      case sf::Event::KeyReleased:
        if (this->event.key.code == sf::Keyboard::A || this->event.key.code == sf::Keyboard::D)
        {
          this->player->resetAnimTimer();
        }
        break;
      default:
        break;
    }
  }

  this->updatePlayer();
  // this->updateView();
  this->updateColision();
}

void Game::restartClock()
{
  this->elapsed = dt_clock.restart();
}

sf::Time Game::getElapsed()
{
  return this->elapsed;
}

void Game::render()
{
  this->window.clear(sf::Color::Cyan);

  this->window.setView(this->view);

  // Draw game objects
  this->renderPlayer();
  this->renderMap();

  this->window.setView(this->window.getDefaultView());

  this->window.display();
}

void Game::updatePlayer()
{
  this->player->update();
}

void Game::updateView()
{
  this->view.setCenter(this->player->getPosition().x+8, this->window.getSize().y/2.f);
}

void Game::updateColision()
{
  if (this->player->getPosition().y + this->player->getGlobalBounds().height > this->window.getSize().y)
  {
    this->player->resetVelocityY();
    this->player->setPosition(this->player->getPosition().x, this->window.getSize().y - this->player->getGlobalBounds().height);
  }
}


void Game::renderPlayer()
{
  this->player->render(this->window);
}

void Game::renderMap()
{
  this->tilemap->render(this->window);
}


void Game::initWindow()
{
  this->window.create(sf::VideoMode(1200, 800), "MyGame", sf::Style::Close | sf::Style::Titlebar);
  this->window.setFramerateLimit(120);
  this->window.setKeyRepeatEnabled(false);
}

void Game::initPlayer()
{
  this->player = new Player();
}

void Game::initMap()
{
  this->tilemap = new TileMap();
}
