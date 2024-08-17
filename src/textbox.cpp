#include "../include/textbox.hpp"

Textbox::Textbox() {
  Setup(5, 9, 200, sf::Vector2f(0, 0));
}

Textbox::Textbox(int visible, int charSize, int width, sf::Vector2f screenPos) {
  Setup(visible, charSize, width, screenPos);
}

Textbox::~Textbox() {
  clear();
}

void Textbox::Setup(int visible, int charSize, int width, sf::Vector2f screenPos) {
  numVisible = visible;
  this->screenPos = screenPos;

  sf::Vector2f offset(2.0f, 2.0f);

  font.loadFromFile("../Arial.TTF");
  content.setFont(font);
  content.setString("");
  content.setCharacterSize(charSize);
  content.setFillColor(sf::Color::White);
  content.setPosition(screenPos + offset);

  backdrop.setSize(sf::Vector2f(width, (visible * (charSize * 1.2f))));
  backdrop.setFillColor(sf::Color(90, 90, 90, 150));
  backdrop.setPosition(this->screenPos);
}

void Textbox::setScreenPos(sf::Vector2f screenPos) {
  this->screenPos = screenPos;
  content.setPosition(screenPos + sf::Vector2f(2.0f, 2.0f));
  backdrop.setPosition(screenPos);
}

void Textbox::add(std::string message) {
  messages.push_back(message);
  if (messages.size() < 6) {
    return;
  }
  messages.erase(messages.begin());
}

void Textbox::clear() {
  messages.clear();
}

void Textbox::render(sf::RenderTarget& window) {
  std::string content;

  for (std::string &m : messages) {
    content.append(m + "\n");
  }

  if (content != "") {
    this->content.setString(content);
    window.draw(backdrop);
    window.draw(this->content);
  }
}
