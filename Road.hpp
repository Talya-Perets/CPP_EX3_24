//talyape123@gmail.com 

#ifndef ROAD_HPP
#define ROAD_HPP

#include <string>
#include <vector>
#include  "RoadSection.hpp"
class RoadSection; // Forward declaration

class Road {
public:
    Road(const std::string& id); // Constructor to initialize Road with id
    ~Road(){};
    const std::string& getId() const; // Getter for id
    
     RoadSection* getBuilding() const; // Getter for building
    void setBuilding(RoadSection* building); // Setter for buildin
    bool isEmpty();
    bool operator!=(const Road& other) const;
     bool operator==(const Road& other) const;


private:
    std::string id;
    RoadSection* building;
};

#endif // ROAD_HPP#include "Road.hpp"