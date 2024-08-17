#pragma once

#include "eventmanager.hpp"
#include "stdafx.h"

class Window {
public:
  Window();
  Window(const std::string& title, const sf::Vector2u& size);
  ~Window();

  void beginDraw();
  void endDraw();

  void Update();

  bool isDone();
  bool isFullscreen();

  void toggleFullscreen(EventDetails* details);

  void draw(sf::Drawable& drawable);
  void draw(sf::VertexArray vertices, sf::RenderStates states);

  sf::Vector2u GetSize() {
    return size;
  }

  sf::RenderWindow* GetTarget() {
    return &window;
  }

  void setView(sf::View view);

  bool isFocused();
  EventManager* getEventManager();
  void close(EventDetails* details = nullptr);

private:

  void setup(const std::string& title, const sf::Vector2u& size);
  void destroy();
  void create();

  sf::RenderWindow window;
  std::string title;
  sf::Vector2u size;
  bool is_fullscreen;
  bool is_done;

  EventManager event_manager;
  bool is_focused;
};
