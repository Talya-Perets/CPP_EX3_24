//talyape123@gmail.com 

#include "RoadSection.hpp"
#include "Road.hpp"

RoadSection::RoadSection(Player *owner, Road* location) : owner(owner),location(location) {} // Constructor with owner
     
      Player* RoadSection::getOwner() const{
       return owner;
     }
         Road* RoadSection::getLocation() const{
            return location;
         }

    bool RoadSection::operator==(const RoadSection& other) const{
         return owner == other.owner && *location == *(other.location);    }

    bool RoadSection::operator!=(const RoadSection& other) const{
          return !(*this == other);
  
    }



