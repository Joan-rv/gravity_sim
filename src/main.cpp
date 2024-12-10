#include "SFML/Window/Keyboard.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <simulation.hpp>
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

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized: {
                sf::FloatRect visible_area(0, 0, event.size.width,
                                           event.size.height);
                window.setView(sf::View(visible_area));
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
                default:
                    break;
                }
                ui_text.setString("Density: " +
                                  std::to_string(sim.get_density()));
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

        window.clear(sf::Color::Black);
        window.draw(sim);
        window.draw(ui_text);

        window.display();

        frameTime = deltaClock.restart();
    }

    return 0;
}
