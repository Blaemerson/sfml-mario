#pragma once
#include "stdafx.h"
#include <unordered_map>
#include <functional>

enum class EventType {
  KeyDown = sf::Event::KeyPressed,
  KeyUp = sf::Event::KeyReleased,
  MButtonDown = sf::Event::MouseButtonPressed,
  MButtonUp = sf::Event::MouseButtonReleased,
  MWheel = sf::Event::MouseWheelMoved,
  WindowResized = sf::Event::Resized,
  GainedFocus = sf::Event::GainedFocus,
  LostFocus = sf::Event::LostFocus,
  MEntered = sf::Event::MouseEntered,
  MLeft = sf::Event::MouseLeft,
  Closed = sf::Event::Closed,
  TextEntered = sf::Event::TextEntered,
  Keyboard = sf::Event::Count + 1, Mouse, Joystick
};

struct EventInfo {
  EventInfo() {code = -1;}
  EventInfo(int event) {code = event;}
  union{ int code; };
};

using Events = std::vector<std::pair<EventType, EventInfo>>;

struct EventDetails {
  EventDetails(const std::string& bindName) : name(bindName) {
    clear();
  }
  std::string name;

  sf::Vector2i size;
  sf::Uint32 text_entered;
  sf::Vector2i mouse;
  int mouse_wheel_delta;
  int keycode;

  void clear() {
    size = sf::Vector2i(0, 0);
    text_entered = 0;
    mouse = sf::Vector2i(0, 0);
    mouse_wheel_delta = 0;
    keycode = -1;
  }
};

struct Binding {
  Binding(const std::string& name) : name(name), details(name), c(0) {}

  void bindEvent(EventType type, EventInfo info = EventInfo()) {
    events.emplace_back(type, info);
  }

  Events events;
  std::string name;
  int c; // Number of events that are happening

  EventDetails details;
};

using Bindings = std::unordered_map<std::string, Binding*>;

using Callbacks = std::unordered_map<std::string, std::function<void(EventDetails*)>>;

class EventManager {
public:
  EventManager();
  ~EventManager();

  bool addBinding(Binding *binding);
  bool removeBinding(std::string name);

  void setFocus(const bool& focus);

  // Needs to be defined in the header
  template<class T>
   bool addCallback(const std::string& name, void(T::*func) (EventDetails*), T* instance) {
     auto tmp = std::bind(func, instance, std::placeholders::_1);
     return callbacks.emplace(name, tmp).second;
   }


  void removeCallback(const std::string& name) {
    callbacks.erase(name);
  }

  void handleEvent(sf::Event& event);
  void update();

  sf::Vector2i getMousePos(sf::RenderWindow* window = nullptr) {
    return (window ? sf::Mouse::getPosition(*window) : sf::Mouse::getPosition());
  }

private:
  void loadBindings();

  Bindings bindings;
  Callbacks callbacks;
  bool has_focus;
};

