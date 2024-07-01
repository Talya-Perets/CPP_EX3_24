#include "Road.hpp"
#include <iostream>


Road::Road(const std::string& id) : id(id) ,building(nullptr){} // Constructor implementation
 


const std::string& Road::getId() const { // Getter for id implementation
    return id;
}

RoadSection* Road::getBuilding() const { // Getter for building implementation
    return building;
}

void Road::setBuilding(RoadSection* roadsec) { // Setter for building implementation

    this->building = roadsec;
}
bool Road::operator==(const Road& other) const{
return(id==other.getId());

}
    bool Road::isEmpty(){
        if(building==nullptr){
        return true;
        }
        return false;
    }


bool Road::operator!=(const Road& other) const{
return(id!=other.getId());
}