#include "../include/window.h"

Window::Window() {
  setup("Window", sf::Vector2u(640, 480));
}

Window::Window(const std::string& title, const sf::Vector2u& size) {
  setup(title, size);
}

Window::~Window() {
  destroy();
}

void Window::setView(sf::View view) {
  window.setView(view);
}

void Window::setup(const std::string& title, const sf::Vector2u& size) {
  this->title = title;
  this->size = size;
  this->is_fullscreen = false;
  this->is_done = false;

  window.setFramerateLimit(60);
  window.setKeyRepeatEnabled(false);
  create();
}

void Window::create() {
  auto style = (is_fullscreen ? sf::Style::Fullscreen : sf::Style::Default);
  window.create({size.x, size.y, 32}, title, style);
}

void Window::destroy() {
  window.close();
}

void Window::update() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      is_done = true;
    } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5) {
      toggleFullscreen();
    }
  }
}

void Window::toggleFullscreen() {
  is_fullscreen = !is_fullscreen;
  destroy();
  create();
}

void Window::beginDraw() {
  window.clear(sf::Color::Cyan);
}

void Window::endDraw() {
  window.display();
}

bool Window::isDone() {
  return is_done;
}

bool Window::isFullscreen() {
  return is_fullscreen;
}

void Window::draw(sf::Drawable& drawable) {
  window.draw(drawable);
}

void Window::draw(sf::VertexArray vertices, sf::RenderStates states) {
  window.draw(vertices, states);
}
