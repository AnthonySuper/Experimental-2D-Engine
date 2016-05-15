#include <iostream>
#include <SDL.h>
#include <mruby.h>
#include <mruby/data.h>
#include <chrono>

#include "polygon.hpp"


int main() {
    int itcount = 0;
    std::vector<NM::Point> pas, pbs;
    for(double i = 0; i < M_PI_2; i += 0.0001) {
        pas.emplace_back(std::cos(i), std::sin(i));
        if(itcount % 1 == 0){
            pbs.emplace_back(std::cos(i) + 0.9, std::sin(i) + 0.9);
        }
        itcount++;
    }
    
    NM::Polygon circle_a(pas);
    NM::Polygon circle_b(pbs);
    auto begin = std::chrono::high_resolution_clock::now();
    std::cout << NM::inspect(circle_a.preciseCollision(circle_b)) << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
    begin = std::chrono::high_resolution_clock::now();
    std::cout << NM::inspect(circle_b.preciseCollision(circle_a)) << std::endl;
    end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
}
