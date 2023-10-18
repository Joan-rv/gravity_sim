#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <point.hpp>
#include <vector>

class Simulation : public sf::Drawable {
public:
    void update(double dt);
    void add_point(int x, int y);

private:
    std::vector<Point> points;

    void physics_sim();
    sf::Vector2f handle_collision(Point& p1, Point& p2, float distance);
    void process_collisions();
    void move_screen(float dt);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
