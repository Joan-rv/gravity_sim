#include <SFML/System/Vector2.hpp>

namespace vec {

template <typename T>
T length(const sf::Vector2<T>& vec){
    return std::sqrt(vec.x*vec.x + vec.y*vec.y);
}
template <typename T>
T dot(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2) {
    return v1.x*v2.x + v1.y*v2.y;
}

} // namespace vec