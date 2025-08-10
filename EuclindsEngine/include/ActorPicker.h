#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "ECS/Actor.h"
#include "CShape.h"

/// @brief Utility class to pick/select actors with mouse in world space.
class
    ActorPicker {
public:
    /**
     * Picks an actor under the mouse position.
     * @param windowPtr Pointer to the SFML RenderWindow.
     * @param actors List of shared pointers to actors.
     * @return Index of the picked actor, or -1 if none.
     */
    static int
        pickActorUnderMouse(sf::RenderWindow* windowPtr, const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*windowPtr);
        sf::Vector2f mouseWorld = windowPtr->mapPixelToCoords(mousePos);

        for (int i = 0; i < actors.size(); ++i) {
            auto shape = actors[i]->getComponent<CShape>();
            if (shape) {
                auto shapePtr = shape->getShapePtr();
                if (shapePtr && shapePtr->getGlobalBounds().contains(mouseWorld)) {
                    return i;
                }
            }
        }
        return -1;
    }
};