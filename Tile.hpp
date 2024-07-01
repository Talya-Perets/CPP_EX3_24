//talyape123@gmail.com 

#ifndef TILE_HPP
#define TILE_HPP
#include "Catan_enums.hpp"
#include <string>

class Tile {
public:

Tile()=default;

    Tile(const std::string& id);
    void setResource(const CatanResurseType resource);
    void setNumber(int number);

    CatanResurseType getResource() const;
    int getNumber() const;
    const std::string& getId() const;

private:
    std::string id;
    CatanResurseType resource;
    int number;
};

#endif // TILE_HPP