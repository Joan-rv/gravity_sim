#include "math.hpp"
#include <SFML/System/Vector2.hpp>
#include <cstdlib>
#include <point.hpp>
#include <vector>

struct Collision {
    Point& p1;
    Point& p2;
    sf::Vector2f normal;
};

void position_contraints(std::vector<Collision>& collisions) {
    for (auto& collision : collisions) {
        float distance = collision.p1.distance(collision.p2);
        collision.p1.move(0.2f * distance * collision.normal);
        collision.p2.move(-0.2f * distance * collision.normal);
    }
}

void velocity_contraints(std::vector<Collision>& collisions) {
    for (auto& collision : collisions) {
        float inv_mass1 = 1 / collision.p1.mass;
        float inv_mass2 = 1 / collision.p2.mass;
        sf::Vector2f relative_vel =
            collision.p2.velocity - collision.p1.velocity;
        float impulse = -2.0f * vec::dot(relative_vel, collision.normal) /
                        (inv_mass1 + inv_mass2);
        impulse = -std::fmax(impulse, 0);

        collision.p1.velocity += inv_mass1 * collision.p1.coeff_of_restitution *
                                 impulse * collision.normal;
        collision.p2.velocity -= inv_mass2 * collision.p2.coeff_of_restitution *
                                 impulse * collision.normal;
    }
}

void handle_collisions(std::vector<Point>& points) {
    std::vector<Collision> collisions;
    for (size_t i = 0; i < points.size(); i++) {
        auto& p1 = points[i];
        for (size_t j = i + 1; j < points.size(); j++) {
            auto& p2 = points[j];
            float distance = p1.distance(p2);
            if (distance < 0) {
                sf::Vector2f vec = p2.get_pos() - p1.get_pos();
                sf::Vector2f normal = vec / vec::length(vec);
                collisions.push_back(Collision{p1, p2, normal});
            }
        }
    }
    // iterate every constraint 10 times
    for (int i = 0; i < 10; i++) {
        position_contraints(collisions);
        velocity_contraints(collisions);
    }
}
