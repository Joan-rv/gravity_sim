#ifndef POINT_H
#define POINT_H
#include <SFML/Graphics.hpp>
#include <vector>

class Point : public sf::Drawable {
    public:
        Point(float mass, float radius, float coeff_of_restitution, const sf::Vector2f& pos);

        sf::Vector2f get_pos() const;
        void add_gravity(Point p);
        void integrate(float time_delta);

        const float mass;
        const float radius;
        const float coeff_of_restitution;

        sf::Vector2f velocity = {0, 0};
        sf::Vector2f forces = {0, 0};
 
    private:
        sf::CircleShape circle;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

float circle_distance(const Point& p1, const Point& p2);
void add_point(std::vector<Point>& points, int x, int y);
#endif
