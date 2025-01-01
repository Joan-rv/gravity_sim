#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cmath>
#include <collisions.hpp>
#include <math.hpp>
#include <optional>
#include <simulation.hpp>

#define G 100
#define MAX_RADIUS 1000

void Simulation::physics_sim() {
    for (std::size_t i = 0; i < points.size(); i++) {
        for (std::size_t j = 0; j < points.size(); j++) {
            if (i == j)
                continue;
            Point& p1 = points[i];
            Point& p2 = points[j];
            sf::Vector2f vec = p2.get_pos() - p1.get_pos();
            float distance = vec::length(vec);
            p1.forces += G * p1.mass * p2.mass * (vec / distance) /
                         (distance * distance);
        }
    }
}

void Simulation::update(double dt) {
    if (paused && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (new_p) {
            if (new_p->shape.getRadius() < new_p->max_radius) {
                new_p->shape.setRadius(new_p->shape.getRadius() + 2 * dt);
                float new_radius = new_p->shape.getRadius();
                new_p->shape.setOrigin(new_radius, new_radius);
            }
        }
    } else if (!paused) {
        physics_sim();
        handle_collisions(points);
        for (auto& point : points) {
            point.integrate(dt);
        }
    }
}

void Simulation::add_point(sf::Vector2f mouse) {
    constexpr float radius = 10;
    Point p(100, radius, 0.8, mouse, sf::Color::White);
    Arrow arrow(0, 2, 10, mouse);
    arrow.set_color(sf::Color(25, 125, 255));

    float min_distance = MAX_RADIUS;
    for (const auto& point : points) {
        if (p.distance(point) < min_distance) {
            min_distance = p.distance(point);
        }
    }
    if (min_distance < 0)
        return;

    new_p.emplace(
        NewPoint{sf::CircleShape(radius), arrow, radius + min_distance});
    new_p->shape.setPosition(mouse);
    new_p->shape.setFillColor(sf::Color(255, 255 - density, 255 - density));
    new_p->shape.setOrigin(radius, radius);
}

void Simulation::consume_point(sf::Vector2f mouse) {
    if (new_p) {
        sf::Vector2f mouse_p_vec = mouse - new_p->shape.getPosition();
        sf::Vector2f vel_normal = mouse_p_vec / vec::length(mouse_p_vec);
        float vel_length = vec::length(mouse_p_vec) - new_p->shape.getRadius();
        sf::Vector2f start_vel = {0, 0};
        if (vel_length > 0) {
            start_vel = vel_length * vel_normal;
        }
        float radius = new_p->shape.getRadius();
        float mass = density * PI * radius * radius;
        points.push_back(Point(mass, radius, 0.8, new_p->shape.getPosition(),
                               new_p->shape.getFillColor()));
        points.back().velocity = start_vel;
        new_p = std::nullopt;
    }
}

void Simulation::mouse_moved(sf::Vector2f mouse) {
    if (new_p) {
        sf::Vector2f pos = new_p->shape.getPosition();
        float radius = new_p->shape.getRadius();

        sf::Vector2f mouse_p_vec = mouse - pos;
        float vel_length = vec::length(mouse_p_vec) - radius;
        float angle = 180.0 / PI * atan2f(mouse_p_vec.y, mouse_p_vec.x);

        if (vel_length > 0) {
            new_p->arrow.set_length(vel_length);
            new_p->arrow.set_rotation(angle);
        } else {
            new_p->arrow.set_length(0);
        }
    }
}

void Simulation::set_density(double new_density) {
    if (new_density > 0 && new_density <= 255) {
        density = new_density;
    }
    if (new_p) {
        // change color
        new_p->shape.setFillColor(sf::Color(255, 255 - density, 255 - density));
    }
}
double Simulation::get_density() { return density; }

void Simulation::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (new_p) {
        target.draw(new_p->shape, states);
        target.draw(new_p->arrow, states);
    }
    for (auto& point : points) {
        target.draw(point, states);
    }
}
