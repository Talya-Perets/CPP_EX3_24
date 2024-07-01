//talyape123@gmail.com 

#include "Tile.hpp"

Tile::Tile(const std::string& id)
    : id(id), resource(CatanResurseType::none), number(0) {}

void Tile::setResource(const CatanResurseType resource) {
    this->resource = resource;
}

void Tile::setNumber(int number) {
    this->number = number;
}

CatanResurseType Tile::getResource() const {
    return resource;
}

int Tile::getNumber() const {
    return number;
}

const std::string& Tile::getId() const {
    return id;
}