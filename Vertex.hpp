//talyape123@gmail.com 

#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <string>

class Building; // Forward declaration

class Vertex {
public:
    Vertex(const std::string& id);
    ~Vertex(){};
    const std::string& getId() const;
    Building* getBuilding() const;
    void setBuilding(Building* building);
    bool operator==(const Vertex& other) const;
    bool operator!=(const Vertex& other) const;
    bool isEmpty() const;
    std::string getOwnerBuilding();

private:
    std::string id;
    Building* building;
};

#endif // VERTEX_HPP