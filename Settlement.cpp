//talyape123@gmail.com 

#include "Settlement.hpp"

Settlement::Settlement(Player* owner, Vertex* location)
    : Building(owner, location) {}
     
     std::string Settlement::getType() const {
        return "Settlement";
    }