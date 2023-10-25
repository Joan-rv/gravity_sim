#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <arrow.hpp>

Arrow::Arrow(float len, float width, float head_width, sf::Vector2f pos) : len(len), width(width), head_width(head_width) {
    // isoceles triangle with base and hight "triangle_size"
    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(0, 0));
    triangle.setPoint(1, sf::Vector2f(head_width, head_width/2));
    triangle.setPoint(2, sf::Vector2f(0, head_width));
    triangle.setOrigin(-len, head_width/2);
    triangle.setPosition(pos);

    rect.setSize(sf::Vector2f(len, width));
    rect.setOrigin(0, width/2);
    rect.setPosition(pos);
}

void Arrow::set_length(float len) {
    rect.setSize(sf::Vector2f(len, width));
    triangle.setOrigin(-len, head_width/2);
}

void Arrow::set_rotation(float angle) {
    rect.setRotation(angle);
    triangle.setRotation(angle);
}

void Arrow::set_color(sf::Color color) {
    rect.setFillColor(color);
    triangle.setFillColor(color);
}

void Arrow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(triangle, states);
    target.draw(rect, states);
}
