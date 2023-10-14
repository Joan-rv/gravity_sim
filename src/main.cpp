#include "physics.hpp"
#include "point.hpp"
#include <SFML/Graphics.hpp>

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 800), "Gravity simulator");

    std::vector<Point> points;

    sf::Clock deltaClock;
    const double dt = 0.01;
    double accumulator = 0.0;
    sf::Time frameTime = sf::Time::Zero;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Button::Left) {
                add_point(points, event.mouseButton.x, event.mouseButton.y);
            }
        }

        accumulator += frameTime.asSeconds();

        while (accumulator >= dt) {
            physics_sim(points);
            for (auto &point : points) {
                point.integrate(dt);
            }
            process_collisions(points);
            accumulator -= dt;
        }

        window.clear(sf::Color::Black);
        for (const auto &point : points) {
            window.draw(point);
        }
        window.display();

        frameTime = deltaClock.restart();
    }

    return 0;
}
