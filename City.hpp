//talyape123@gmail.com 

#ifndef CITY_HPP
#define CITY_HPP
#include "Building.hpp"

class City : public Building {
public:
  City(Player* owner, Vertex* location);
      std::string getType() const override ;
    
};
#endif