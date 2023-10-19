#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <simulation.hpp>
#include <vecmath.hpp>

#define G 100

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

sf::Vector2f Simulation::handle_collision(Point& p1, Point& p2,
                                          float distance) {
    float m1 = p1.mass;
    float m2 = p2.mass;
    float e = p1.coeff_of_restitution;
    sf::Vector2f v1 = p1.velocity;
    sf::Vector2f v2 = p2.velocity;
    sf::Vector2f vec = p2.get_pos() - p1.get_pos();
    sf::Vector2f normal_vec = vec / vec::length(vec);

    float normal_impulse =
        ((m1 * m2) * (1 + e) * vec::dot((v2 - v1), normal_vec)) / (m1 + m2);
    p1.forces -= vec::dot(normal_vec, p1.forces) * normal_vec;
    return normal_impulse / p1.mass * normal_vec;
}

void Simulation::process_collisions() {
    std::vector<sf::Vector2f> new_vels(points.size(), {0, 0});
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points.size(); j++) {
            if (i == j)
                continue;
            Point& p1 = points[i];
            Point& p2 = points[j];
            float distance = circle_distance(p1, p2);
            if (distance < 0) {
                new_vels[i] += handle_collision(p1, p2, distance);
            }
        }
    }
    for (int i = 0; i < points.size(); i++) {
        points[i].velocity += new_vels[i];
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
    physics_sim();
    process_collisions();
    for (auto& point : points) {
        point.integrate(dt);
    }
    move_screen(dt);
}

void Simulation::add_point(int x, int y) {
    Point new_p(100, 10, 0.8, sf::Vector2f(x, y));
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

void Simulation::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto& point : points) {
        target.draw(point, states);
    }
}
