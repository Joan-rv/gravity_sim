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
                Point new_p(
                    100, 10, 0.8,
                    sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                bool add_p = true;
                for (const auto& p : points) {
                    if (circle_distance(p, new_p) <= 0) {
                        add_p = false;
                        break;
                    }
                }
                if (add_p) {
                    points.push_back(new_p);
                }
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