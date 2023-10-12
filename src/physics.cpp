#include <vector>
#include <cmath>
#include "physics.hpp"
#include "point.hpp"
#include "vecmath.hpp"

void physics_sim(std::vector<Point>& points) {
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points.size(); j++) {
            if (i==j)
                continue;
            points[i].add_gravity(points[j]);
        }
    }
}

sf::Vector2f handle_collision(Point& p1, Point& p2, float distance) {
    float m1 = p1.mass;
    float m2 = p2.mass;
    float e = p1.coeff_of_restitution;
    sf::Vector2f v1 = p1.velocity;
    sf::Vector2f v2 = p2.velocity;
    sf::Vector2f vec = p2.get_pos() - p1.get_pos();
    sf::Vector2f normal_vec = vec / vec::length(vec);

    float normal_impulse = ((m1*m2)*(1 + e)*vec::dot((v2 - v1), normal_vec))/(m1 + m2);
    return normal_impulse/p1.mass * normal_vec + normal_vec * distance;
}

void process_collisions(std::vector<Point>& points) {
    std::vector<sf::Vector2f> new_vels(points.size(), {0, 0}); 
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points.size(); j++) {
            if (i==j)
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
