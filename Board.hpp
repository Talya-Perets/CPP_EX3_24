//talyape123@gmail.com 

#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>
#include <map>
#include <memory>
#include "Settlement.hpp"
#include "City.hpp"
#include "RoadSection.hpp"
#include "Road.hpp"
#include "Vertex.hpp"
#include "Tile.hpp"
#include "Catan_enums.hpp"

class Board
{
private:
    std::vector<Tile> tiles;
    std::vector<Settlement*> settlements;
    std::vector<City*> cities;
    std::vector<RoadSection*> roadsSectionList;
    std::vector<std::vector<size_t>> tileRoadsIndex;
    std::vector<std::vector<size_t>> tileVerticesIndex;
    std::vector<Vertex*> vertices;
    std::vector<Road*> roads;

    void setTileVertices();
    void setTileRoads();
    void settTiles();
    void setRandomResources();
    void setNumTiles();

public:
    Board();
    ~Board();
    void setupBoard();

    Tile& getTile(int index);
    std::vector<Vertex*>& getVerticesArray();
    std::vector<Road*>& getRoadArry();
    void addSettlement(Settlement* settlement);
    void addCity(City& city);
    void addRoadSection(RoadSection& road);

    const std::vector<Settlement *>& getSettlements() const;
    const std::vector<City*>& getCities() const;
    const std::vector<RoadSection*>& getRoadsSection() const;
    const std::vector<std::vector<size_t>>& getTileRoadsIndex() const;
    const std::vector<std::vector<size_t>>& getTileVerticesIndex() const;
    void removeSettlement(Settlement* settlement);
    bool isValidVertex(size_t verNum1);
    bool isValidRoad(size_t road1);
    void printBoard() const ;
};

#endif // BOARD_HPP