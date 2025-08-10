#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>

/*
 * Returns a const ref to a static vector so no per-call copies are made.
 * Keep all circuit waypoints OUT of BaseApp.
 */
inline const std::vector<sf::Vector2f>& getCircuitWaypoints() {
    static const std::vector<sf::Vector2f> kCircuit = {
      {885,885},{1280,872},{1348,796},{1306,725},{1225,700},{1185,650},
      {1198,578},{1280,545},{1346,476},{1314,400},{1044,400},{1015,486},
      {932,552},{860,515},{860,240},{1118,240},{1174,154},{1114,52},
      {582,68},{515,86},{515,345},{562,372},{654,408},{654,525},
      {554,544},{512,620},{512,838},{621,874},{928,874}
    };
    return kCircuit;
}