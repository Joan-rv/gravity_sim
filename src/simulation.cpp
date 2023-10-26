#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cmath>
#include <math.hpp>
#include <optional>
#include <simulation.hpp>

#define G 100
#define MAX_RADIUS 1000
#define START_VEL_RECT_WIDTH 2

void Simulation::physics_sim() {
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points.size(); j++) {
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

std::pair<sf::Vector2f, sf::Vector2f>
Simulation::handle_collision(Point& p1, Point& p2, float distance) {
    float m1 = p1.mass;
    float m2 = p2.mass;
    float e = p1.coeff_of_restitution;
    sf::Vector2f v1 = p1.velocity;
    sf::Vector2f v2 = p2.velocity;
    sf::Vector2f vec = p2.get_pos() - p1.get_pos();
    sf::Vector2f normal_vec = vec / vec::length(vec);

    float normal_impulse =
        ((m1 * m2) * (1 + e) * vec::dot((v2 - v1), normal_vec)) / (m1 + m2);
    sf::Vector2f vel = normal_impulse / p1.mass * normal_vec;
    sf::Vector2f force = vec::dot(normal_vec, p1.forces) * normal_vec;
    return std::pair{vel, force};
}

#include <iostream>
void Simulation::process_collisions() {
    std::vector<sf::Vector2f> new_vels(points.size(), {0, 0});
    std::vector<sf::Vector2f> new_forces(points.size(), {0, 0});
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points.size(); j++) {
            if (i == j)
                continue;
            Point& p1 = points[i];
            Point& p2 = points[j];
            float distance = p1.distance(p2);
            if (distance < 0) {
                auto [vel, force] = handle_collision(p1, p2, distance);

                new_vels[i] += vel;
                new_forces[i] -= force;
            }
        }
    }
    for (int i = 0; i < points.size(); i++) {
        points[i].velocity += new_vels[i];
        points[i].forces += new_forces[i];
    }
}

void Simulation::move_screen(float dt) {
    sf::Vector2f move_vec = {0, 0};
    static float move_speed = 200;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        move_vec += {0, 1};
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        move_vec += {1, 0};
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        move_vec += {0, -1};
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        move_vec += {-1, 0};
    }

    if (move_vec != sf::Vector2(0.0f, 0.0f)) {
        move_vec = move_vec / vec::length(move_vec);
        move_speed += 40 * dt;
        for (auto& point : points) {
            point.move(move_vec * move_speed * dt);
        }
    } else {
        move_speed = 200;
    }
}

void Simulation::update(double dt) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (new_p) {
            if (new_p->shape.getRadius() < new_p->max_radius) {
                new_p->shape.setRadius(new_p->shape.getRadius() + 2 * dt);
                float new_radius = new_p->shape.getRadius();
                new_p->shape.setOrigin(new_radius, new_radius);
            }
        }
        return;
    }
    physics_sim();
    process_collisions();
    for (auto& point : points) {
        point.integrate(dt);
    }
    move_screen(dt);
}

void Simulation::add_point(int x, int y) {
    constexpr float radius = 10;
    Point p(100, radius, 0.8, sf::Vector2f(x, y));
    sf::RectangleShape rect({0, 0});
    rect.setPosition(x, y);
    rect.setOrigin(0, START_VEL_RECT_WIDTH / 2.0);
    rect.setFillColor(sf::Color(39, 133, 255));

    float min_distance = MAX_RADIUS;
    for (const auto& point : points) {
        if (p.distance(point) < min_distance) {
            min_distance = p.distance(point);
        }
    }
    if (min_distance < 0)
        return;

    new_p.emplace(
        NewPoint{sf::CircleShape(radius), rect, radius + min_distance});
    new_p->shape.setPosition(x, y);
    new_p->shape.setOrigin(radius, radius);
}

void Simulation::consume_point(float density, int x, int y) {
    if (new_p) {
        sf::Vector2f vec = sf::Vector2f(x, y) - new_p->shape.getPosition();
        sf::Vector2f vel_normal = vec / vec::length(vec);
        float vel_lenth = vec::length(vec) - new_p->shape.getRadius();
        sf::Vector2f start_vel = {0, 0};
        if (vel_lenth > 0) {
            start_vel = vel_lenth * vel_normal;
        }
        float radius = new_p->shape.getRadius();
        float mass = density * PI * radius * radius;
        points.push_back(Point(mass, radius, 0.8, new_p->shape.getPosition()));
        points.back().velocity = start_vel;
        new_p = std::nullopt;
    }
}

void Simulation::mouse_moved(int x, int y) {
    if (new_p) {
        sf::Vector2f click(x, y);
        sf::Vector2f pos = new_p->shape.getPosition();
        float radius = new_p->shape.getRadius();

        sf::Vector2f vec = click - pos;
        sf::Vector2f vel_normal = vec / vec::length(vec);
        float vec_length = vec::length(vec) - radius;
        float angle = 180.0 / PI * atan2f(vel_normal.y, vel_normal.x);

        if (vec_length > 0) {
            new_p->rect.setSize({vec_length, START_VEL_RECT_WIDTH});
            new_p->rect.setRotation(angle);
        } else {
            new_p->rect.setSize({0, 0});
        }
    }
}

void Simulation::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (new_p) {
        target.draw(new_p->shape, states);
        target.draw(new_p->rect, states);
    }
    for (auto& point : points) {
        target.draw(point, states);
    }
}
