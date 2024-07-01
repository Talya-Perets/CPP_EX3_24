//talyape123@gmail.com 

#ifndef CATAN_HPP
#define CATAN_HPP

#include <vector>
#include <string>
#include <random>
#include "Player.hpp"
#include "Board.hpp"
#include "Settlement.hpp"
#include "RoadSection.hpp"
#include "City.hpp"

class Catan {
public:
    Catan(Player p1, Player p2, Player p3);
    ~Catan();

    void init();
    void StartBuilding();
    void playTurn(Player* currentPlayer);
    void buyAndBuildSettelment(Player* player);
    void buyAndBuildCity(Player* player);
    void buyAndBuildRoadSec(Player* player);
    std::string getResourceName(CatanResurseType resource) ;


  // Initialization methods
    Player* chooseStartPlayer();
    void initializeDevelopmentDeck();

    // Turn-based methods
    int rollDice();
    void distributeResources(int diceRoll);
    void distributeResourcesToLastSettlements();
    void handleSeven() ;
    void distributeInitialResources() ;

    // Building methods
    bool BuildStartSettlement(Player* player, size_t indexSettlement);
    bool BuildStartRoadSec(Player* player, size_t indexRoad);
    bool checkToBuySett(Player* player, size_t indexVertex);
    bool checkToBuyCity(Player* player, size_t indexVertex);
    bool checkToBuyRoadSec(Player* player, size_t indexRoad);

    // Trading methods
    void trade(Player* player);
    bool tradeWithPlayer(Player& curr, Player& other, CatanResurseType give, CatanResurseType receive, int giveAmount, int receiveAmount);
    bool tradeWithBank(Player& curr, CatanResurseType giveResource, CatanResurseType receiveResource);

    // Development card methods
    void buyDevelopmentCard(Player* player);
    void chooseDevelopmentCard(Player* player);
    void useDevelopmentCard(Player *player, int choice);
    void useMonopolyCard(Player* player, CatanResurseType resource);
    void useRoadBuildingCard(Player* player);
    void useYearOfPlentyCard(Player* player);
    void useKnightCard(Player* player);
    void useVictoryPointCard(Player* player);

    // Helper methods
    bool noBuildingAround(const std::string& vertexId);
    int getVertexIndexInTile(int tileIndex, const std::string& vertexId);
    int getRoadIndexInTile(int tileIndex, const std::string& roadId);
    std::vector<int> getConnectedLandTiles(const std::string& locationId) const;
    bool RoadconnectToBulding(Player* player, const std::string& roadId);
    bool RoadconnectToRodSec(Player* player, const std::string& roadId);
    bool SettekmentconnectToRodSec(Player* player, const std::string& vertexId);
    bool buildRoadForFree(Player* player, size_t indexRoad);
    void checkAndUpdateLargestArmy(Player* player);
    std::string getDevelopmentCardName(DevelopmentCard card);

    // Getters
    Board* getBoard() const;
    Player* getCurrentPlayer();
    Player* getNextPlayer();
private:
    // Game components
    std::vector<Player> players;
    size_t currentPlayerIndex;
    Board& board;
    std::vector<DevelopmentCard> developmentDeck;
    std::vector<Settlement*> settlements;
    std::vector<City*> cities;
    std::vector<RoadSection*> roadSections;
    Player* largestArmyOwner;

  
};

#endif // CATAN_HPP