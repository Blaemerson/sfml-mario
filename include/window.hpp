#pragma once

#include "stdafx.h"

class Window {
public:
  Window();
  Window(const std::string& title, const sf::Vector2u& size);
  ~Window();

  void beginDraw();
  void endDraw();

  void update();

  bool isDone();
  bool isFullscreen();

  void toggleFullscreen();

  void draw(sf::Drawable& drawable);
  void draw(sf::VertexArray vertices, sf::RenderStates states);

  sf::Vector2u getSize() {
    return size;
  }

  sf::RenderWindow* getTarget() {
    return &window;
  }

  void setView(sf::View view);

private:

  void setup(const std::string& title, const sf::Vector2u& size);
  void destroy();
  void create();

  sf::RenderWindow window;
  std::string title;
  sf::Vector2u size;
  bool is_fullscreen;
  bool is_done;
};
