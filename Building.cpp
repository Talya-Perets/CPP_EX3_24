#include "Building.hpp"

Building::Building(Player* owner, Vertex* location)
  : owner(owner), location(location) {}

Player* Building::getOwner() const {
  return owner;
}

Vertex* Building::getLocation() const {
    if (location == nullptr) {
        std::cerr << "Error: Building has no location" << std::endl;
        return nullptr;
    }
    return location;
}

bool Building::operator==(const Building& other) const {
  return owner == other.owner && location == other.location;
}

bool Building::operator!=(const Building& other) const {
  return !(*this == other);
}