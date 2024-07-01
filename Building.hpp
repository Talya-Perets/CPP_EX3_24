#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <string>
#include <iostream>

class Vertex;  // Forward declaration
class Player;  // Forward declaration

class Building {
public:
    Building(Player* owner, Vertex* location);
    virtual ~Building() = default;
    
    Player* getOwner() const;
    Vertex* getLocation() const;
    bool operator==(const Building& other) const;
    bool operator!=(const Building& other) const;
virtual std::string getType() const = 0;
private:
    Player* owner;
    Vertex* location;
};

#endif // BUILDING_HPP