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
    void add_point(int x, int y);
    void consume_point(float density, int x, int y);
    void mouse_moved(int x, int y);

private:
    std::vector<Point> points;
    std::optional<NewPoint> new_p;

    void physics_sim();
    sf::Vector2f handle_collision(Point& p1, Point& p2);
    void process_collisions();
    void move_screen(float dt);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
#endif
