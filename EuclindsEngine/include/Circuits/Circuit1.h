#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>

/*
 * Returns a const ref to a static vector so no per-call copies are made.
 * Keep all circuit waypoints OUT of BaseApp.
 */
inline const std::vector<sf::Vector2f>& getCircuitWaypoints() {
    static const std::vector<sf::Vector2f> kCircuit = {
      {1680,885},{1300,485},{800,650},{800,500},{890,300},{1300,600},{1400,500},
      {1300,100},{950,0},{850,0},{700,500},{400,300},{500,1200},{850,900},{850,700}
    };
    return kCircuit;
}