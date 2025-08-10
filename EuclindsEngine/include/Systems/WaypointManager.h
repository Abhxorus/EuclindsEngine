#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>

class
    WaypointManager {
public:
    void
        setWaypoints(const std::vector<sf::Vector2f>& pts) {
        waypoints_ = pts;
    }
    const std::vector<sf::Vector2f>& waypoints() const { return waypoints_; }
private:
    std::vector<sf::Vector2f> waypoints_;
};