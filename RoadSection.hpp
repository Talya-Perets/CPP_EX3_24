//talyape123@gmail.com 

#ifndef ROADSECTION_HPP
#define ROADSECTION_HPP

#include <string>

// Forward declaration of Road class
class Road;
class Player;

class RoadSection {
public:
    RoadSection(Player *owner, Road* location);
    ~RoadSection() = default;
    Player* getOwner() const;
    Road* getLocation() const;

    bool operator==(const RoadSection& other) const;
    bool operator!=(const RoadSection& other) const;

private:
    Player* owner;
    Road* location;
};

#endif // ROADSECTION_HPP