//talyape123@gmail.com 

#include "Vertex.hpp"
#include <iostream>
#include "Building.hpp"

Vertex::Vertex(const std::string& id)
    : id(id), building(nullptr) {}

const std::string& Vertex::getId() const {
    return id;
}

Building* Vertex::getBuilding() const {
    return building;
}

void Vertex::setBuilding(Building* newBuilding) {
    if (newBuilding == nullptr) {
        std::cout << "Warning: Attempting to set a null building" << std::endl;
    }
    this->building = newBuilding;
}

bool Vertex::operator==(const Vertex& other) const {
    return id == other.getId();
}
    

bool Vertex::operator!=(const Vertex& other) const {
    return !(*this == other);
}

bool Vertex::isEmpty() const {
    return building == nullptr;
}
