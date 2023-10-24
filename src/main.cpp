#include <SFML/Graphics.hpp>
#include <iostream>
#include <simulation.hpp>
#include <string>

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 800), "Gravity simulator", sf::Style::Default, settings);

    Simulation sim;

    sf::Clock deltaClock;
    sf::Time frameTime = sf::Time::Zero;

    const double dt = 0.01;
    double accumulator = 0.0;

    float density = 10;

    sf::Font font;
    if (!font.loadFromFile("resources/JetBrainsMono-Regular.ttf")) {
        return -1;
    }
    sf::Text ui_text;
    ui_text.setFont(font);
    ui_text.setCharacterSize(18);
    ui_text.setString("Density: " + std::to_string(density));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                if (event.key.code == sf::Keyboard::E) {
                    density += 1;
                }
                if (event.key.code == sf::Keyboard::Q) {
                    if (density > 1) {
                        density -= 1;
                    }
                }
                ui_text.setString("Density: " + std::to_string(density));
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Button::Left) {
                    sim.add_point(event.mouseButton.x, event.mouseButton.y);
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Button::Left) {
                    sim.consume_point(density, event.mouseButton.x, event.mouseButton.y);
                }
                break;
            case sf::Event::MouseMoved:
                sim.mouse_moved(event.mouseMove.x, event.mouseMove.y);
                break;

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
