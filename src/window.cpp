#include "../include/window.hpp"

Window::Window() {
  setup("Window", sf::Vector2u(1920, 1080));
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

  is_focused = true;
  event_manager.addCallback("Fullscreen_toggle", &Window::toggleFullscreen, this);
  event_manager.addCallback("Window_close", &Window::close, this);
  create();
}

void Window::create() {
  auto style = (is_fullscreen ? sf::Style::Fullscreen : sf::Style::Default);
  window.create({size.x, size.y, 32}, title, style);
}

void Window::destroy() {
  window.close();
}

void Window::Update() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::LostFocus) {
      is_focused = false;
      event_manager.setFocus(false);
    } else if (event.type == sf::Event::GainedFocus) {
      is_focused = true;
      event_manager.setFocus(true);
    }
    event_manager.handleEvent(event);
  }

  event_manager.update();
}

void Window::toggleFullscreen(EventDetails* details) {
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

void Window::close(EventDetails* details) {
  is_done = true;
}

void Window::draw(sf::Drawable& drawable) {
  window.draw(drawable);
}

void Window::draw(sf::VertexArray vertices, sf::RenderStates states) {
  window.draw(vertices, states);
}
