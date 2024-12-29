#include "SFML/Window/ContextSettings.hpp"
#include <SFML/System/Time.hpp>
#include <boost/dll/runtime_symbol_info.hpp>
#include <cmath>
#include <gravity_sim.hpp>
#include <iostream>
GravitySim::GravitySim()
    : window(sf::VideoMode(800, 800), "Gravity simulator", sf::Style::Default,
             [] {
                 sf::ContextSettings settings;
                 settings.antialiasingLevel = 8;
                 return settings;
             }()),
      sim(), delta_clock(), cursor_clock(), frame_time{sf::Time::Zero},
      dt{0.01}, accumulator(0.0), movement(0, 0), text{""}, cursor{""},
      is_inputing{false} {

    boost::system::error_code ec;
    boost::filesystem::path program_path = boost::dll::program_location(ec);
    if (ec) {
        exit(1);
    }
    std::string font_path = (program_path.parent_path() / ".." / "resources" /
                             "JetBrainsMono-Regular.ttf")
                                .string();
    if (!font.loadFromFile(font_path)) {
        exit(1);
    }
    ui_text.setFont(font);
    ui_text.setCharacterSize(18);
    ui_text.setString("Density: " + std::to_string(sim.get_density()));

    main_view = window.getView();
    sim_view = window.getView();
}

bool GravitySim::is_running() { return window.isOpen(); }

void GravitySim::loop() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::Resized: {
            sf::Vector2f size(event.size.width, event.size.height);
            sf::Vector2f size_delta = size - sim_view.getSize();
            // center moves because of size increase
            main_view.setCenter(sim_view.getCenter() + size_delta / 2.f);
            main_view.setSize(size);
            sim_view.setCenter(sim_view.getCenter() + size_delta / 2.f);
            sim_view.setSize(size);
            break;
        }
        case sf::Event::KeyPressed:
            switch (event.key.code) {
            case sf::Keyboard::Escape:
                window.close();
                break;
            case sf::Keyboard::E:
                sim.set_density(sim.get_density() + 1);
                break;
            case sf::Keyboard::Q:
                sim.set_density(sim.get_density() - 1);
                break;
            case sf::Keyboard::W:
                movement.y = -1;
                break;
            case sf::Keyboard::A:
                movement.x = -1;
                break;
            case sf::Keyboard::S:
                movement.y = 1;
                break;
            case sf::Keyboard::D:
                movement.x = 1;
                break;
            default:
                break;
            }
            ui_text.setString("Density: " + std::to_string(sim.get_density()));
            break;
        case sf::Event::KeyReleased:
            switch (event.key.code) {
            case sf::Keyboard::W:
            case sf::Keyboard::S:
                movement.y = 0;
                break;
            case sf::Keyboard::A:
            case sf::Keyboard::D:
                movement.x = 0;
                break;
            case sf::Keyboard::Enter:
                double new_density;
                try {
                    new_density = std::stod(text);
                } catch (const std::invalid_argument&) {
                } catch (const std::out_of_range&) {
                }
                sim.set_density(new_density);
                ui_text.setString("Density: " +
                                  std::to_string(sim.get_density()));
                text = "";
                is_inputing = false;
                sim.paused = false;
                break;
            default:
                break;
            }
            break;
        case sf::Event::TextEntered:
            if (('0' <= event.text.unicode && event.text.unicode <= '9') ||
                event.text.unicode == '.') {
                text += event.text.unicode;
                is_inputing = true;
                sim.paused = true;
            }
            break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Button::Left) {
                sim.add_point(window.mapPixelToCoords(
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
                sim.paused = true;
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Button::Left) {
                sim.consume_point(window.mapPixelToCoords(
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
                sim.paused = false;
            }
            break;
        case sf::Event::MouseMoved: {
            sim.mouse_moved(window.mapPixelToCoords(
                sf::Vector2i(event.mouseMove.x, event.mouseMove.y)));
            break;
        case sf::Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                sim_view.zoom(pow(1.1f, event.mouseWheelScroll.delta));
            }
            break;
        }

        default:
            break;
        }
    }

    accumulator += frame_time.asSeconds();

    while (accumulator >= dt) {
        sim.update(dt);
        accumulator -= dt;
    }

    if (movement != sf::Vector2f(0, 0)) {
        sim_view.move(movement);
    }
    if (is_inputing) {
        if (cursor_clock.getElapsedTime() >= sf::milliseconds(500)) {
            cursor_clock.restart();
            if (cursor == "_") {
                cursor = "";
            } else {
                cursor = "_";
            }
        }
        ui_text.setString("Density: " + text + cursor);
    }
    window.clear(sf::Color::Black);
    window.draw(sim);
    window.setView(main_view);
    window.draw(ui_text);
    window.setView(sim_view);

    window.display();

    frame_time = delta_clock.restart();
}

GravitySim::~GravitySim() {}
