#ifndef PHYSICS_H
#define PHYSICS_H
#include <vector>
#include "point.hpp"

void physics_sim(std::vector<Point>& points);

// TODO: Add interpolation between physics states

void process_collisions(std::vector<Point>& points);
#endif