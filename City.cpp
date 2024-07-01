//talyape123@gmail.com 

#include "City.hpp"

City::City(Player* owner, Vertex* location)
    : Building(owner, location) {}
        std::string City::getType() const  {
        return "City";
    }