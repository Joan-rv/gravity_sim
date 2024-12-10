#ifndef POINT_H
#define POINT_H
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

class Point : public sf::Drawable {
public:
    Point(float mass, float radius, float coeff_of_restitution,
          const sf::Vector2f& pos, sf::Color color);

    sf::Vector2f get_pos() const;
    void move(const sf::Vector2f& vec);
    float get_radius() const;
    void set_radius(float radius);
    void add_gravity(Point p);

    void integrate(float time_delta);
    float distance(const Point& p) const;
    float distance(const sf::Vector2f& vec) const;

    const float mass;
    const float coeff_of_restitution;

    sf::Vector2f velocity = {0, 0};
    sf::Vector2f forces = {0, 0};

private:
    sf::CircleShape circle;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
#endif
