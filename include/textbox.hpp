#pragma once
#include "stdafx.h"

using MessageContainer = std::vector<std::string>;

class Textbox {
public:
  Textbox();
  Textbox(int visible, int charSize, int width, sf::Vector2f screenPos);
  ~Textbox();

  void setup(int visible, int charSize, int width, sf::Vector2f screenPos);
  void add(std::string message);
  void clear();

  void render(sf::RenderTarget& window);

  void setScreenPos(sf::Vector2f screenPos);

private:
  MessageContainer messages;
  int numVisible;

  sf::RectangleShape backdrop;
  sf::Vector2f screenPos;
  sf::Font font;
  sf::Text content;
};
