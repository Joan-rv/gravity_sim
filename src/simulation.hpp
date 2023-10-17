#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>
#include <point.hpp>

class Simulation : public sf::Drawable {
    public:
        void update(double dt);
        void add_point(int x, int y);
    private:
        std::vector<Point> points;
        virtual void draw(sf::RenderTarget&, sf::RenderStates states) const;
};
