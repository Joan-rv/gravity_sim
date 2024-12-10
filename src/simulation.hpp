#ifndef SIMULATION_H
#define SIMULATION_H
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <arrow.hpp>
#include <optional>
#include <point.hpp>
#include <vector>

struct NewPoint {
    sf::CircleShape shape;
    Arrow arrow;
    float max_radius;
};

class Simulation : public sf::Drawable {
public:
    void update(double dt);
    void add_point(sf::Vector2f mouse);
    void consume_point(sf::Vector2f mouse);
    void mouse_moved(sf::Vector2f mouse);
    void set_density(double density);
    double get_density();

private:
    std::vector<Point> points;
    std::optional<NewPoint> new_p;
    double density = 10;

    void physics_sim();
    void move_screen(float dt);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
#endif
