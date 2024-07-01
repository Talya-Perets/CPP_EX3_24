//talyape123@gmail.com 

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>
#include "Vertex.hpp"
#include "Settlement.hpp"
#include "RoadSection.hpp"
#include "City.hpp"
#include "Tile.hpp"
#include "Building.hpp"
#include <map>
#include <algorithm>
#include "Catan_enums.hpp"

class Vertex;
class Settlement;
class RoadSection;
class City;
class Building;

class Player {
private:
    std::string name;
    int points;
    std::map<CatanResurseType, int> resourceCards;
    std::vector<DevelopmentCard> developmentCards;
    std::vector<Settlement*> settlements;
    std::vector<RoadSection*> roadSections;
    std::vector<City*> cities;
    std::map<BuldingType, int> buldings;
    int knightCount;

public:
    Player(const std::string& name);
    ~Player(){}
    const std::string& getName() const;
    void changeBuldings(BuldingType bulding, int num);
    int getBuldingCount(BuldingType bulding);
    bool buySettelment(Settlement* settlement);
    void discardHalfResources() ;
    CatanResurseType getRandomResource();
    std::string getResourceName(CatanResurseType resource);
    bool buyCity(City* city);
    bool buyRoadSection(RoadSection* roadSection);
    bool buyDevelopmentCard(DevelopmentCard ) ;
   void incrementKnightCount();
    void addResourceCard(const CatanResurseType developmentCard , int count);
    void removeResourceCard(const CatanResurseType card);
    void removeSettlement(Settlement* settlement);
    void addDevelopmentCard(const DevelopmentCard& card);
    void removeDevelopmentCard(int index);
    void addSettlement(Settlement* settlement);
    void addRoad(RoadSection* roadSection);
    void addCity(City* city);
    bool tradeWithBank(CatanResurseType give, CatanResurseType receive);
void removeDevelopmentCard(size_t index) ;
    void addPoints(int amount);
    void removeResource(CatanResurseType type, int amount);
    bool trade(Player& other, CatanResurseType give, CatanResurseType receive, int giveAmount, int receiveAmount);
    const std::map<CatanResurseType, int> getResourceCards() const;
    int getResourceCardsAt(CatanResurseType type);
    const std::vector<DevelopmentCard>& getDevelopmentCards() const;
    const std::vector<Settlement*>& getSettlements() const;
    const std::vector<RoadSection*>& getRoads() const;
    const std::vector<City*>& getCities() const;
   bool operator==(const Player& other) const;
 int getKnightCount ()const;
    int getPoints();
    void printResourceCards();
};

#endif // PLAYER_HPP