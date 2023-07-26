#include "../include/eventmanager.hpp"
#include <fstream>
#include <sstream>

EventManager::EventManager(): has_focus(true) {
  loadBindings();
}

EventManager::~EventManager() {
  for (auto &itr : bindings) {
    delete itr.second;
    itr.second = nullptr;
  }
}

bool EventManager::addBinding(Binding *binding) {
  if (bindings.find(binding->name) != bindings.end()) {
    return false;
  }

  return bindings.emplace(binding->name, binding).second;
}

bool EventManager::removeBinding(std::string name) {
  auto itr = bindings.find(name);
  if (itr == bindings.end()) {
    return false;
  }

  delete itr->second;
  bindings.erase(itr);
  return true;
}

void EventManager::handleEvent(sf::Event& event) {
  for (auto &b_itr : bindings) {
    Binding* bind = b_itr.second;
    for (auto &e_itr : bind->events) {
      EventType sfml_event = (EventType) event.type;
      if (e_itr.first != sfml_event) {
        continue;
      }

      if (sfml_event == EventType::KeyDown || sfml_event == EventType::KeyUp) {
        if (e_itr.second.code == event.key.code) {
          // Matching event/keystroke
          // Increase count
          if (bind->details.keycode != -1) {
            bind->details.keycode = e_itr.second.code;
          }

          ++(bind->c);
          break;
        }
      } else if (sfml_event == EventType::MButtonDown || sfml_event == EventType::MButtonUp) {
        if (e_itr.second.code == event.mouseButton.button) {
          // Matching event/keystroke
          // Increase count
          bind->details.mouse.x = event.mouseButton.x;
          bind->details.mouse.y = event.mouseButton.y;
          if (bind->details.keycode != -1) {
            bind->details.keycode = e_itr.second.code;
          }

          ++(bind->c);
          break;
        }
      } else {
        // No need for additional checking
        if (sfml_event == EventType::MWheel) {
          bind->details.mouse_wheel_delta = event.mouseWheel.delta;
        } else if (sfml_event == EventType::WindowResized) {
          bind->details.size.x = event.size.width;
          bind->details.size.y = event.size.height;
        } else if (sfml_event == EventType::TextEntered) {
          bind->details.text_entered = event.text.unicode;
        }

        ++(bind->c);
      }
    }
  }
}

void EventManager::update() {
  if (!has_focus) {
    return;
  }

  for (auto &b_itr : bindings) {
    Binding* bind = b_itr.second;
    for (auto &e_itr : bind->events) {
      switch (e_itr.first) {
        case (EventType::Keyboard) :
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(e_itr.second.code))) {
            if (bind->details.keycode != -1) {
              bind->details.keycode = e_itr.second.code;
            }
            ++(bind->c);
          }
        break;
        case (EventType::Mouse) :
          if (sf::Mouse::isButtonPressed(sf::Mouse::Button(e_itr.second.code))) {
            if (bind->details.keycode != -1) {
              bind->details.keycode = e_itr.second.code;
            }
            ++(bind->c);
          }
        break;
        case (EventType::Joystick) :
          // expand
        break;
      }
    }

    if (bind->events.size() == bind->c) {
      auto call_itr = callbacks.find(bind->name);
      if (call_itr != callbacks.end()) {
        call_itr->second(&bind->details);
      }
    }

    bind->c = 0;
    bind->details.clear();
  }
}

void EventManager::loadBindings() {
  std::string delimiter = ":";

  std::ifstream bindings;
  bindings.open("keys.cfg");

  if (!bindings.is_open()) {
    std::cout << "! Failed to load keys.cfg" << std::endl;
    return;
  }

  std::string line;
  while (std::getline(bindings, line)) {
    std::stringstream keystream(line);
    std::string callback_name;

    keystream >> callback_name;

    Binding* bind = new Binding(callback_name);
    while (!keystream.eof()) {
      std::string keyval;
      keystream >> keyval;

      int start = 0;
      int end = keyval.find(delimiter);

      if (end == std::string::npos) {
        delete bind;
        bind = nullptr;
        break;
      }

      EventType type = EventType(stoi(keyval.substr(start, end - start)));

      int code = stoi(keyval.substr(end + delimiter.length(), keyval.find(delimiter, end + delimiter.length())));

      EventInfo event_info;
      event_info.code = code;

      bind->bindEvent(type, event_info);
    }

    if (!addBinding(bind)) {
      delete bind;
    }
    bind = nullptr;

  }

  bindings.close();
}

void EventManager::setFocus(const bool &focus) {
  has_focus = focus;
}
