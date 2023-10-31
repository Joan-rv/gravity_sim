#include "math.hpp"
#include <SFML/System/Vector2.hpp>
#include <algorithm>
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

        collision.p1.velocity += inv_mass1 * impulse * collision.normal;
        collision.p2.velocity -= inv_mass2 * impulse * collision.normal;
    }
    /*
    for (auto& collision : collisions) {
        // Calculate relative velocity of p1 and p2 along the collision normal
        sf::Vector2f relativeVelocity =
            collision.p1.velocity - collision.p2.velocity;
        float relativeSpeed = vec::dot(relativeVelocity, collision.normal);

        // Check if objects are moving toward each other
        if (relativeSpeed > 0) {
            // Calculate the impulse (change in momentum) required to resolve
            // the collision
            float impulse = -2.0f * relativeSpeed /
                            (1 / collision.p1.mass + 1 / collision.p2.mass);

            // Update velocities of p1 and p2 based on the impulse and normal
            sf::Vector2f impulseVector = impulse * collision.normal;
            collision.p1.velocity += (1 / collision.p1.mass) * impulseVector;
            collision.p2.velocity -= (1 / collision.p2.mass) * impulseVector;
        }
    }
    */
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
