//talyape123@gmail.com 

#ifndef SETTLEMENT_HPP
#define SETTLEMENT_HPP

#include "Building.hpp"

class Settlement : public Building {
public:
    Settlement(Player* owner, Vertex* location);
    std::string getType() const override ;
};

#endif // SETTLEMENT_HPP