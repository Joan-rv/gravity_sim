#include "SFML/Graphics/RenderWindow.hpp"
#include <simulation.hpp>
class GravitySim {
public:
    GravitySim();
    ~GravitySim();
    void loop();
    bool is_running();

private:
    sf::RenderWindow window;
    Simulation sim;
    sf::Clock delta_clock;
    sf::Time frame_time;
    const double dt;
    double accumulator;
    sf::Font font;
    sf::Text ui_text;
    sf::View main_view;
    sf::View sim_view;
    sf::Vector2f movement;
    std::string text;
};
