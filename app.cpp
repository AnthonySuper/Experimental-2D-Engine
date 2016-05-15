#include <iostream>
#include <SDL.h>
#include <mruby.h>
#include <mruby/data.h>
#include <chrono>

#include "polygon.hpp"


int main() {
    std::vector<NM::Point> pas, pbs;
    for(double i = 0; i < M_PI_2; i += 0.0001) {
        pas.emplace_back(std::cos(i), std::sin(i));
        pbs.emplace_back(std::cos(i) + 0.9, std::sin(i) + 0.9);
    }
    NM::Polygon circle_a(pas);
    NM::Polygon circle_b(pbs);

    std::cout << NM::inspect(circle_a.preciseCollision(circle_b));
    
}
