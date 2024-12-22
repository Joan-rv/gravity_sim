#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <simulation.hpp>
#include <stdexcept>
#include <string>

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 800), "Gravity simulator",
                            sf::Style::Default, settings);

    Simulation sim;

    sf::Clock deltaClock;
    sf::Time frameTime = sf::Time::Zero;

    const double dt = 0.01;
    double accumulator = 0.0;

    sf::Font font;
    if (!font.loadFromFile("resources/JetBrainsMono-Regular.ttf")) {
        return -1;
    }
    sf::Text ui_text;
    ui_text.setFont(font);
    ui_text.setCharacterSize(18);
    ui_text.setString("Density: " + std::to_string(sim.get_density()));

    sf::View main_view = window.getView();
    sf::View sim_view = window.getView();
    sf::Vector2f movement(0, 0);
    std::string text = "";

    while (window.isOpen()) {
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
                case sf::Keyboard::Up:
                    sim_view.zoom(1 / 1.1f);
                    break;
                case sf::Keyboard::Down:
                    sim_view.zoom(1.1f);
                    break;
                default:
                    break;
                }
                ui_text.setString("Density: " +
                                  std::to_string(sim.get_density()));
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
                    break;
                default:
                    break;
                }
                break;
            case sf::Event::TextEntered:
                if (('0' <= event.text.unicode && event.text.unicode <= '9') ||
                    event.text.unicode == '.') {
                    text += event.text.unicode;
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Button::Left) {
                    sim.add_point(window.mapPixelToCoords(sf::Vector2i(
                        event.mouseButton.x, event.mouseButton.y)));
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Button::Left) {
                    sim.consume_point(window.mapPixelToCoords(sf::Vector2i(
                        event.mouseButton.x, event.mouseButton.y)));
                }
                break;
            case sf::Event::MouseMoved: {
                sim.mouse_moved(window.mapPixelToCoords(
                    sf::Vector2i(event.mouseMove.x, event.mouseMove.y)));
                break;
            }

            default:
                break;
            }
        }

        accumulator += frameTime.asSeconds();

        while (accumulator >= dt) {
            sim.update(dt);
            accumulator -= dt;
        }

        if (movement != sf::Vector2f(0, 0)) {
            sim_view.move(movement);
        }
        window.clear(sf::Color::Black);
        window.draw(sim);
        window.setView(main_view);
        window.draw(ui_text);
        window.setView(sim_view);

        window.display();

        frameTime = deltaClock.restart();
    }

    return 0;
}
