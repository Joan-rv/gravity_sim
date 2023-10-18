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
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
