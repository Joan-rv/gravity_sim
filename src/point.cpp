#include <cmath>
#include <vector>
#include <point.hpp>
#include <vecmath.hpp>

#define G 100

Point::Point(float mass, float radius, float e, const sf::Vector2f& pos) : mass(mass), radius(radius), coeff_of_restitution(e) {
    circle = sf::CircleShape(radius);
    circle.setOrigin(radius, radius);
    circle.setPosition(pos);
}

sf::Vector2f Point::get_pos() const {return circle.getPosition();}

void Point::integrate(float time_delta) {
    velocity += (forces / mass) * time_delta;
    circle.move(velocity * time_delta);
    forces = {0, 0};
}

void Point::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(circle, states);
}

float circle_distance(const Point& p1, const Point& p2) {
    sf::Vector2f vec = p1.get_pos() - p2.get_pos();
    float distance = vec::length(vec);
    return distance - (p1.radius + p2.radius);
}
