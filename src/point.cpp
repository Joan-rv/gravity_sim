#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <math.hpp>
#include <point.hpp>
#include <vector>

#define G 100

Point::Point(float mass, float radius, float e, const sf::Vector2f& pos)
    : mass(mass), coeff_of_restitution(e) {
    circle = sf::CircleShape(radius);
    circle.setOrigin(radius, radius);
    circle.setPosition(pos);
}

sf::Vector2f Point::get_pos() const { return circle.getPosition(); }
void Point::move(const sf::Vector2f& vec) { circle.move(vec); }
float Point::get_radius() const { return circle.getRadius(); }
void Point::set_radius(float radius) {
    circle.setRadius(radius);
    circle.setOrigin(radius, radius);
}

void Point::integrate(float time_delta) {
    velocity += (forces / mass) * time_delta;
    circle.move(velocity * time_delta);
    forces = {0, 0};
}

void Point::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(circle, states);
}

float Point::distance(const Point& p) const {
    sf::Vector2f vec = get_pos() - p.get_pos();
    float distance = vec::length(vec);
    return distance - (get_radius() + p.get_radius());
}

float Point::distance(const sf::Vector2f& vec) const {
    sf::Vector2f dis_vec = get_pos() - vec;
    return vec::length(dis_vec);
}
