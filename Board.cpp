//talyape123@gmail.com 
#include "Board.hpp"
#include <string>
#include <algorithm>
#include <iomanip>

const std::string roadsId[72] = {
    "A23", "A34", "B23", "B34", "C23", "C34",                     // 6 (0-5)
    "A12", "AB", "BC", "C4",                                     // 4(6-9)
    "D23", "AD", "AE", "BE", "BF", "CF", "CG", "G4",             // 8 (10-17)
    "D12", "DE", "EF", "FG", "G45",                               // 5(18-22)
    "H23", "DH", "DI", "EI", "EJ", "FJ", "FK", "GK", "GL", "L34", // 10 (23-32)
    "H12", "HI", "IJ", "JK", "KL", "L45",                         // 6 (33-38)
    "H16", "HM", "IM", "IN", "JN", "JO", "KO", "KP", "LP", "L56", // 10 (39-48)
    "M16", "MN", "NO", "OP", "P45",                               // 5 (49-53)
    "M12", "MQ", "NQ", "NR", "OR", "OS", "PS", "P56",             // 8 (54-61)
    "Q12", "QR", "RS", "S45",                                     // 4 (62-65)
    "Q16", "Q56", "R16", "R56", "R16", "S65"};                    // 6

const std::string vertexId[54] = {
    "A3", "B3", "C3",                       // 3 (0-2)
    "A2", "AB", "BC", "C4",                 // 4 (3-6)
    "AD", "ABE", "BCF", "CG",               // 4 (7-10)
    "D2", "ADE", "BEF", "CFG", "G4",        // 5 (11-15)
    "DH", "DEI", "EFJ", "FGK", "GL",        // 5 (16-20)
    "H2", "DHI", "EIJ", "FJK", "GKL", "L4", // 6 (21-26)
    "H1", "HIM", "IJN", "JKO", "KLP", "L5", // 6 (27-32)
    "HM", "IMN", "JNO", "KOP", "LP",        // 5 (33-37)
    "M1", "MNQ", "NOR", "OPS", "P5",        // 5 (38-42)
    "MQ", "NQR", "ORS", "PS",               // 4 (43-46)
    "Q1", "QR", "RS", "S5",                 // 4 (47-50)
    "Q6", "R6", "S6"};                      // 3 (51-53)


    Board::Board()
{
    setupBoard();
}

void Board::setupBoard() {
    settTiles();
    setTileRoads();
    setTileVertices();
    setRandomResources();
    setNumTiles();
}

// Destructor
Board::~Board() {
    // שחרור הזיכרון של Vertex objects
    for (auto vertex : vertices) {
        delete vertex;
    }
    vertices.clear();

    // שחרור הזיכרון של Road objects
    for (auto road : roads) {
        delete road;
    }
    roads.clear();

    // שחרור הזיכרון של Settlement objects

}


void Board::settTiles()
{
    tiles = {
        Tile("A"),
        Tile("B"),
        Tile("C"),
        Tile("D"),
        Tile("E"),
        Tile("F"),
        Tile("G"),
        Tile("H"),
        Tile("I"),
        Tile("J"),
        Tile("K"),
        Tile("L"),
        Tile("M"),
        Tile("N"),
        Tile("O"),
        Tile("P"),
        Tile("Q"),
        Tile("R"),
        Tile("S")};
}

void Board::setTileRoads() {
    for (const auto& id : roadsId) {
        roads.push_back(new Road(id));
    }


   tileRoadsIndex = {
        {6, 0, 1, 7, 12, 11},     // Tile 0
        {7, 2, 3, 8, 14, 13},     // Tile 1
        {8, 4, 5, 9, 16, 15},     // Tile 2
        {18, 10, 11, 19, 25, 24}, // Tile 3
        {19, 12, 13, 20, 27, 26}, // Tile 4
        {20, 14, 15, 21, 29, 28}, // Tile 5
        {21, 16, 17, 22, 31, 30}, // Tile 6
        {33, 23, 24, 34, 40, 39}, // Tile 7
        {34, 25, 26, 35, 42, 41}, // Tile 8
        {35, 27, 28, 36, 44, 43}, // Tile 9
        {36, 29, 30, 37, 46, 45}, // Tile 10
        {37, 31, 32, 38, 48, 47}, // Tile 11
        {49, 40, 41, 50, 55, 54}, // Tile 12
        {50, 42, 43, 51, 57, 56}, // Tile 13
        {51, 44, 45, 52, 59, 58}, // Tile 14
        {52, 46, 47, 53, 61, 60}, // Tile 15
        {62, 55, 56, 63, 67, 66}, // Tile 16
        {63, 57, 58, 64, 69, 68}, // Tile 17
        {64, 59, 60, 65, 71, 70}  // Tile 18
    };

}



void Board::setTileVertices() {
    for (const auto& id : vertexId) {
        vertices.push_back(new Vertex(id));
    }
    tileVerticesIndex = {
        {7, 3, 0, 4, 8, 12},
        {8, 4, 1, 5, 9, 13},
        {9, 5, 2, 6, 10, 14},
        {16, 11, 7, 12, 17, 22},
        {17, 12, 8, 13, 18, 23},
        {18, 13, 9, 14, 19, 24},
        {19, 14, 10, 15, 20, 25},
        {27, 21, 16, 22, 28, 33},
        {28, 22, 17, 23, 29, 34},
        {29, 23, 18, 24, 30, 35},
        {30, 24, 19, 25, 31, 36},
        {31, 25, 20, 26, 32, 37},
        {38, 33, 28, 34, 39, 43},
        {39, 34, 29, 35, 40, 44},
        {40, 35, 30, 36, 41, 45},
        {41, 36, 31, 37, 42, 46},
        {47, 43, 39, 44, 48, 51},
        {48, 44, 40, 45, 49, 52},
        {49, 45, 41, 46, 50, 53}
    };

}


void Board::setRandomResources()
{
    srand(time(0));

    std::map<CatanResurseType, int> Resurses = {
        {CatanResurseType::Wood, 4},
        {CatanResurseType::Brick, 3},
        {CatanResurseType::Sheep, 4},
        {CatanResurseType::Wheat, 4},
        {CatanResurseType::Ore, 3},
        {CatanResurseType::Desert, 1}
    };

    std::vector<CatanResurseType> availableResources;
    for (const auto& pair : Resurses)
    {
        for (int i = 0; i < pair.second; ++i)
        {
            availableResources.push_back(pair.first);
        }
    }

    for (size_t i = 0; i < tiles.size(); ++i)
    {
        int index = rand() % availableResources.size();
        CatanResurseType randomResource = availableResources.at(index);
        tiles.at(i).setResource(randomResource);
        availableResources.erase(availableResources.begin() + index);
    }
}
void Board::setNumTiles()
{
srand(time(0));
            std::map<int, int> numTiles = {
            {2, 1}, {3, 2}, {4, 2}, {5, 2}, {6, 2},  {8, 2}, {9, 2}, {10, 2}, {11, 2}, {12, 1}
        };

        for (size_t i = 0; i < tiles.size(); ++i) {
            if(tiles.at(i).getResource()!=CatanResurseType::Desert){
            int index = rand() % numTiles.size();
            auto it = numTiles.begin();
            advance(it, index);
            int randomNumber = it->first;
 
            tiles.at(i).setNumber(randomNumber);

            if (it->second == 1) {
                numTiles.erase(it);
            } else {
                it->second--;
            }
        }
    }}

void Board::removeSettlement(Settlement* settlement) {
    auto it =find(settlements.begin(), settlements.end(), settlement);
    if (it != settlements.end()) {
        settlements.erase(it);
    }
}

Tile &Board::getTile(int index)
{
    return tiles.at(index);
}

   std::vector<Vertex*>& Board::getVerticesArray() {
    return vertices;
}

   std::vector<Road*>& Board::getRoadArry(){
    return roads;
   }


void Board::addSettlement(Settlement* settlement)
{
    settlements.emplace_back(settlement);
}

void Board::addCity( City &city)
{
    cities.push_back(&city);
}

void Board::addRoadSection(RoadSection &road)
{
    roadsSectionList.push_back(&road);
}

const std::vector<Settlement*> &Board::getSettlements() const
{
    return settlements;
}

const std::vector<City*> &Board::getCities() const
{
    return cities;
}

const std::vector<RoadSection*> &Board::getRoadsSection() const
{
    return roadsSectionList;
}
const std::vector<std::vector<size_t>>& Board::getTileRoadsIndex() const {
    return tileRoadsIndex;
}

const std::vector<std::vector<size_t>>& Board::getTileVerticesIndex() const {
    return tileVerticesIndex;
}
bool Board::isValidVertex(size_t verNum1) {
    if (verNum1 >= vertices.size()) {
        std::cout << "Invalid vertex number. Please choose a valid vertex number." << std::endl;
        return false;
    }

    Vertex* vertexPtr = vertices.at(verNum1);

    if (vertexPtr->getBuilding() != nullptr) {
        std::cout << "There is already a building constructed at this vertex. Please choose another one." << std::endl;
        return false;
    }

    return true;
}

bool Board::isValidRoad(size_t road1) {
    if (road1 >= roads.size()) {
        std::cout << "Invalid road number. Please choose a valid road number." << std::endl;
        return false;
    }
    Road* roadPtr = roads.at(road1);
    if (roadPtr->getBuilding() != nullptr) {
        std::cout << "There is already a road constructed at this road section. Please choose another one." << std::endl;
        return false;
    }
    return true;
}
std::string resourceToString(CatanResurseType resource) {
    switch (resource) {
        case CatanResurseType::Wood: return "Wood";
        case CatanResurseType::Brick: return "Brick";
        case CatanResurseType::Sheep: return "Sheep";
        case CatanResurseType::Wheat: return "Wheat";
        case CatanResurseType::Ore: return "Ore";
        case CatanResurseType::Desert: return "Desert";
        default: return "Unknown";
    }
}

void Board::printBoard() const {
    // מספק את מספר האריחים בכל שורה
    int rows[] = { 3, 4, 5, 4, 3 };

    int index = 0;
    for (int row = 0; row < 5; ++row) {
        std::cout << std::setw((5 - rows[row]) * 4) << "";

        for (int tile = 0; tile < rows[row]; ++tile) {
            const Tile& t = tiles[index];
            std::string resource = resourceToString(t.getResource());
            int number = t.getNumber();
            
            std::cout << "[" << resource.substr(0, 3) << " " << number << "]";

            if (tile < rows[row] - 1) {
                std::cout << "   "; 
            }

            ++index;
        }
        std::cout << std::endl;
    }
}