#ifndef ARROW_H
#define ARROW_H
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

class Arrow : public sf::Drawable {
public:
    Arrow(float len, float width, float head_width, sf::Vector2f pos = {0, 0});
    void set_length(float len);
    void set_rotation(float angle);
    void set_color(sf::Color color);

private:
    sf::RectangleShape rect;
    sf::ConvexShape triangle;

    float len;
    float width;
    float head_width;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
#endif
