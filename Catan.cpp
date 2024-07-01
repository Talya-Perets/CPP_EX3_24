//talyape123@gmail.com 

#include "Catan.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>

// Constructor: Initializes the Catan game with three players, the turn index, and the board
Catan::Catan(Player p1, Player p2, Player p3) : players({p1, p2, p3}), currentPlayerIndex(), board(*new Board())
{
}

// Initializes the game by choosing the starting player, dividing buildings, and starting the building phase
void Catan::init()
{
    initializeDevelopmentDeck();
     chooseStartPlayer();
    StartBuilding();
    distributeInitialResources();

    bool gameOver = false;
    while (!gameOver)
    {
        Player *currentPlayer = getCurrentPlayer();
        playTurn(currentPlayer);
        
        if (currentPlayer->getPoints() >= 10)
        {
            std::cout << currentPlayer->getName() << " has won the game with " 
                      << currentPlayer->getPoints() << " victory points!" << std::endl;
            gameOver = true;
        }
        else
        {
         currentPlayerIndex = (currentPlayerIndex + 1) % players.size();        }
    }
    std::cout << "Game Over!" << std::endl;
    }

// Randomly chooses the starting player
Player *Catan::chooseStartPlayer()
{
    srand(time(0));
    size_t randomNumber = rand() % players.size();
    currentPlayerIndex = randomNumber;
    std::cout << "the fird player is: " << players.at(currentPlayerIndex).getName();
    return &players.at(currentPlayerIndex);
}

void Catan::initializeDevelopmentDeck()
{
    std::map<DevelopmentCard, int> cardCounts = {
        {DevelopmentCard::Knight, 14},
        {DevelopmentCard::RoadBuilding, 2},
        {DevelopmentCard::YearOfPlenty, 2},
        {DevelopmentCard::Monopoly, 2},
        {DevelopmentCard::VictoryPoint, 5}};

    for (const auto &pair : cardCounts)
    {

        for (int i = 0; i < pair.second; ++i)
        {
            developmentDeck.emplace_back(pair.first);
        }
    }

    // Shuffle the deck
    std::srand(unsigned(std::time(0)));
    std::random_shuffle(developmentDeck.begin(), developmentDeck.end());
}

// Advances to the next player in the turn order
Player *Catan::getNextPlayer()
{
    if (currentPlayerIndex == players.size() - 1)
    {
        currentPlayerIndex = 0;
    }
    else
    {
        currentPlayerIndex++;
    }
    return &players.at(currentPlayerIndex);
}

// Manages the initial building phase where each player builds two settlements and two roads
void Catan::StartBuilding()
{
    size_t numPlayers = players.size();

    for (size_t i = 0; i < numPlayers * 2; ++i)
    {
        auto currentPlayer = getCurrentPlayer();
        size_t verNum1;
        size_t road1;

        // Loop for settlement building
        bool validSettlement = false;
        while (!validSettlement)
        {
            std::cout << currentPlayer->getName() << ", please choose a vertex number for settlement: ";
            std::cin >> verNum1;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cout << "Invalid input. Please enter a valid number." << std::endl;
                continue;
            }
            validSettlement = BuildStartSettlement(currentPlayer, verNum1);
        }

        // Loop for road building
        bool validRoad = false;
        while (!validRoad)
        {
            std::cout << currentPlayer->getName() << ", please choose a road number for the RoadSection: ";
            std::cin >> road1;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cout << "Invalid input. Please enter a valid number." << std::endl;
                continue;
            }
            validRoad = BuildStartRoadSec(currentPlayer, road1);
        }

        currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
        currentPlayerIndex = currentPlayerIndex;
    }
}
void Catan::distributeInitialResources() {
    for (Player& player : players) {
        std::cout<<player.getName()<<" get :";
        for (Settlement* settlement : player.getSettlements()) {
            std::vector<int> connectedTiles = getConnectedLandTiles(settlement->getLocation()->getId());
            for (int tileIndex : connectedTiles) {
                Tile& tile = board.getTile(tileIndex);
                if(tile.getResource()!=CatanResurseType::Desert){
                player.addResourceCard(tile.getResource(), 1);
               std::cout <<getResourceName(tile.getResource())<<" ";
            }}
        }
        std::cout<<std::endl;
    }
}

void Catan::distributeResourcesToLastSettlements()
{
    for (Player &player : players)
    {
        // Check if the player has settlements
        if (!player.getSettlements().empty())
        {
            // Get the last settlement of the player
            Settlement *lastSettlement = player.getSettlements().back();

            std::string vertexId = lastSettlement->getLocation()->getId();

            std::vector<int> connectedTiles = getConnectedLandTiles(vertexId);

            // Distribute resources based on connected land tiles
            for (int tileIndex : connectedTiles)
            {
                Tile &tile = board.getTile(tileIndex);
                
                    player.addResourceCard(tile.getResource(),1);
                
            }
        }
    }
}

bool Catan::BuildStartSettlement(Player *player, size_t indexSettelment)
{
    if (!board.isValidVertex(indexSettelment))
    {
        std::cout << "Invalid vertex number or already occupied. Please choose another one." << std::endl;
        return false;
    }

    Vertex *vertexPtr = board.getVerticesArray().at(indexSettelment);

    if (!noBuildingAround(vertexPtr->getId()))
    {
        std::cout << "There is a building too close to the chosen vertex. Please choose another vertex." << std::endl;
        return false;
    }
    Settlement *newSettlement = new Settlement(player, vertexPtr);

    settlements.emplace_back(newSettlement);
    vertexPtr->setBuilding(newSettlement); // שימו לב לשינוי כאן
    player->addSettlement(newSettlement);
    board.addSettlement(newSettlement); // העברת מצביע במקום הפניה
    std::cout << player->getName() << ", the settlement in vertex: " << vertexPtr->getId() << " built successfully\n";
    return true;
}

bool Catan::BuildStartRoadSec(Player *player, size_t indexRoad)
{
    if (!board.isValidRoad(indexRoad))
    {
        std::cout << "Invalid road number. Please choose a valid road number." << std::endl;
        return false;
    }

    Road *roadPtr = board.getRoadArry().at(indexRoad);
    if (roadPtr == nullptr || roadPtr->getId().empty())
    {
        std::cout << "Invalid road. Please choose another road." << std::endl;
        return false;
    }

    if (!RoadconnectToBulding(player, roadPtr->getId()))
    {
        std::cout << "Invalid road. The road needs to be connected to a settlement or road section. Please choose another road." << std::endl;
        return false;
    }

    RoadSection *newRoadSec = new RoadSection(player, roadPtr);
    roadSections.emplace_back(newRoadSec);
    roadPtr->setBuilding(newRoadSec);
    player->addRoad(newRoadSec);
    board.addRoadSection(*newRoadSec);
    std::cout << player->getName() << ", the RoadSection in Road: " << roadPtr->getId() << " built successfully\n";
    return true;
}

/**
 * Simulates rolling two six-sided dice and returns the sum.
 * @return The sum of two dice rolls (2-12)
 */
int Catan::rollDice()
{
    // Create a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 6);

    // Roll two dice
    int dice1 = dis(gen);
    int dice2 = dis(gen);
    int total = dice1 + dice2;

    // Print the result
    std::cout << "Dice roll result: " << dice1 << " + " << dice2 << " = " << total << std::endl;

    return total;
}

/**
 * Manages a player's turn in the game.
 * @param currentPlayer Pointer to the current player
 */
void Catan::playTurn(Player *currentPlayer)
{
    std::cout << "It's " << currentPlayer->getName() << "'s turn." << std::endl;

    // Roll the dice
    int diceResult = rollDice();

    if (diceResult == 7) {
        handleSeven();
    } else {
        distributeResources(diceResult);
    }
    // Player's action phase
    bool endTurn = false;
    while (!endTurn)
    {
        std::cout << "Choose an action: (1)buy and build roadSection, (2)buy and build settelmet, (3)buy and build city, (4) Trade, (5) Use development card, (6) buyDevelopmentCard ,(7)printResourceCards,(8) End turn" << std::endl;
        int choice;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            buyAndBuildRoadSec(currentPlayer);
            break;
        case 2:
            buyAndBuildSettelment(currentPlayer);
            break;
        case 3:
            buyAndBuildCity(currentPlayer);
            break;

        case 4:
            trade(currentPlayer);
            break;
        case 5:
            chooseDevelopmentCard(currentPlayer);
            endTurn=true;
            break;
        case 6:
            buyDevelopmentCard(currentPlayer);
            break;
        case 7:
            currentPlayer->printResourceCards();
            break;
        case 8:
            endTurn = true;
            std::cout << currentPlayer->getName() << "'s turn has ended." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}

void Catan::handleSeven() {
    for (Player& player : players) {
        if (player.getResourceCards().size() > 7) {
            player.discardHalfResources();
        }
    }
}

// Helper functions

void Catan::distributeResources(int diceRoll)
{
    std::cout << "Distributing resources for dice roll: " << diceRoll << std::endl;

    // Iterate through settlements and distribute resources
    for (size_t i = 0; i < board.getSettlements().size(); i++)
    {
        std::string idVer = board.getSettlements().at(i)->getLocation()->getId();
        std::vector<int> landINd = getConnectedLandTiles(idVer);

        // Iterate through connected land tiles
        for (size_t j = 0; j < landINd.size(); j++)
        {
            if (board.getTile(landINd[j]).getNumber() == diceRoll)
            {
                board.getSettlements().at(i)->getOwner()->addResourceCard(board.getTile(landINd[j]).getResource(),1);
                std::cout<<"player "<<board.getSettlements().at(i)->getOwner()->getName()<<" get "<<getResourceName(board.getTile(landINd[j]).getResource())<<std::endl;
            }
        }
    }

    // Iterate through cities and distribute resources
    for (size_t i = 0; i < board.getCities().size(); i++)
    {
        std::string idVer = board.getCities().at(i)->getLocation()->getId();
        std::vector<int> landINd = getConnectedLandTiles(idVer);

        // Iterate through connected land tiles
        for (size_t j = 0; j < landINd.size(); j++)
        {
            if (board.getTile(landINd[j]).getNumber() == diceRoll)
            {
                board.getCities().at(i)->getOwner()->addResourceCard(board.getTile(landINd[j]).getResource(),2);
            }
        }
    }
}

void Catan::buyAndBuildRoadSec(Player *player)
{
    size_t road1;
    std::cout << player->getName() << " is building..." << std::endl;
    bool validRoad = false;

    while (!validRoad)
    {
        std::cout << player->getName() << ", please choose a road number for the RoadSection: ";
        std::cin >> road1;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cout << "Invalid input. Please enter a valid number." << std::endl;
            continue;
        }
        validRoad = checkToBuyRoadSec(player, road1);
    }
}

bool Catan::checkToBuyRoadSec(Player *player, size_t indexRoad)
{
    if (!board.isValidRoad(indexRoad))
    {
        std::cout << "Invalid road number. Please choose a valid road number." << std::endl;
        return false;
    }

    Road *roadPtr = board.getRoadArry().at(indexRoad);
    if (roadPtr == nullptr || roadPtr->getId().empty())
    {
        std::cout << "Invalid road. The road needs to be connected to a settlement. Please choose another road." << std::endl;
        return false;
    }

    if (!RoadconnectToBulding(player, roadPtr->getId()) && !RoadconnectToRodSec(player, roadPtr->getId()))
    {
        std::cout << "Invalid road. The road needs to be connected to a settlement. Please choose another road." << std::endl;
        return false;
    }

    RoadSection *newRoadSec = new RoadSection(player, roadPtr);
    roadSections.emplace_back(newRoadSec);

    if (!player->buyRoadSection(newRoadSec))
    {
        std::cout << "Cannot buy this road section. Please choose another road." << std::endl;
        roadSections.pop_back();
        delete newRoadSec;
        return true;
    }

    roadPtr->setBuilding(newRoadSec);
    board.addRoadSection(*newRoadSec);

    std::cout << player->getName() << ", the RoadSection in Road: " << roadPtr->getId() << " built successfully\n";
    return true;
}

void Catan::buyAndBuildSettelment(Player *player)
{
    int verNum;
    bool validSettlement = false;
    while (!validSettlement)
    {
        std::cout << player->getName() << ", please choose a vertex number for settlement: ";
        std::cin >> verNum;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cout << "Invalid input. Please enter a valid number." << std::endl;
            continue;
        }
        validSettlement = checkToBuySett(player, verNum);
    }
}

bool Catan::checkToBuySett(Player *player, size_t indexVar)
{
    if (!board.isValidVertex(indexVar))
    {
        std::cout << "Invalid vertex number or already occupied. Please choose another one." << std::endl;
        return false;
    }
    Vertex *vertexPtr = board.getVerticesArray().at(indexVar);

    if (!noBuildingAround(vertexPtr->getId()) || !SettekmentconnectToRodSec(player, vertexPtr->getId()))
    {
        std::cout << "There is a building too close to the chosen vertex or not connect to your road section. Please choose another vertex." << std::endl;
        return false;
    }

    Settlement *newSettlement = new Settlement(player, vertexPtr);
    settlements.emplace_back(newSettlement);

    if (!player->buySettelment(newSettlement))
    {
        settlements.pop_back();
        delete newSettlement;
        return true;
    }

    vertexPtr->setBuilding(newSettlement);
    board.addSettlement(newSettlement);
    std::cout << player->getName() << ", the settlement in vertex: " << vertexPtr->getId() << " built successfully\n";
    return true;
}

void Catan::buyAndBuildCity(Player *player)
{
    int verNum;
    bool valiCity = false;
    while (!valiCity)
    {
        std::cout << player->getName() << ", please choose a vertex number for city: ";
        std::cin >> verNum;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid number." << std::endl;
            continue;
        }
        valiCity = checkToBuyCity(player, verNum);
        if (valiCity)
        {
            std::cout << "City built successfully or cannot buy city. Exiting city building process." << std::endl;
        }
    }
}

bool Catan::checkToBuyCity(Player *player, size_t indexVar)
{
    if (indexVar >= board.getVerticesArray().size())
    {
        std::cout << "Invalid vertex number. Please choose a valid vertex number." << std::endl;
        return false;
    }

    Vertex *vertexPtr = board.getVerticesArray().at(indexVar);
    if (vertexPtr->isEmpty())
    {
        std::cout << "This vertex is empty. You need to build a settlement before a city." << std::endl;
        return false;
    }

    Building *existingBuilding = vertexPtr->getBuilding();
    if (existingBuilding->getOwner() != player)
    {
        std::cout << "This vertex has a building that another player built. Please choose another one." << std::endl;
        return false;
    }

    if (existingBuilding->getType() != "Settlement")
    {
        std::cout << "You can only upgrade a settlement to a city." << std::endl;
        return false;
    }

    City *newCity = new City(player, vertexPtr);
    if (!player->buyCity(newCity))
    {
        std::cout << player->getName() << " cannot buy city\n";
        delete newCity;
        return true; // Return true to exit the loop even if the player can't afford it
    }

    // Remove the settlement from the player's list and the game board
    player->removeSettlement(static_cast<Settlement *>(existingBuilding));
    board.removeSettlement(static_cast<Settlement *>(existingBuilding));

    // Add the new city
    cities.push_back(newCity);
    vertexPtr->setBuilding(newCity);
    board.addCity(*newCity);

    std::cout << player->getName() << ", the city in vertex: " << vertexPtr->getId() << " built successfully\n";
    return true;
}

void Catan::trade(Player *player)
{
    std::cout << player->getName() << " is trading..." << std::endl;

    std::cout << "Select trade type: (1) Player trade, (2) Bank trade: ";
    int tradeType;
    std::cin >> tradeType;

    if (tradeType == 1)
    {
    std::cout << player->getName() << " is trading..." << std::endl;

    // Select player to trade with
    std::cout << "Select a player to trade with (0-" << players.size() - 1 << "): ";
    size_t otherPlayerIndex;
    std::cin >> otherPlayerIndex;
    Player &otherPlayer = players[otherPlayerIndex];
    std::cout << "is trading whith "<<otherPlayer.getName()<<std::endl;
    std::cout<< "his resurce card : ";
    otherPlayer.printResourceCards();
    // Select resource to give
    std::cout << "Select resource to give (1:Wood, 2:Brick, 3:Sheep, :Wheat, 5:Ore): ";
    int giveResourceInt;
    std::cin >> giveResourceInt;
    CatanResurseType giveResource = static_cast<CatanResurseType>(giveResourceInt);
    std::cout<<getResourceName(giveResource)<<" the give resurce\n";

    // Amount to give
    std::cout << "Enter amount to give: ";
    int giveAmount;
    std::cin >> giveAmount;

    // Select resource to receive
    std::cout << "Select resource to receive (1:Wood, 2:Brick, 3:Sheep, :Wheat, 5:Ore):: ";
    int receiveResourceInt;
    std::cin >> receiveResourceInt;
    CatanResurseType receiveResource = static_cast<CatanResurseType>(receiveResourceInt);

    // Amount to receive
    std::cout << "Enter amount to receive: ";
    int receiveAmount;
    std::cin >> receiveAmount;
    if (!tradeWithPlayer(*player, otherPlayer, giveResource, receiveResource, giveAmount, receiveAmount))
    {
        std::cout << "trade's faild\n";
    }
    else
    {
        std::cout << "the trade was secsecc\n";
    }
}
else if (tradeType == 2)
    {
        // Bank trade
        std::cout << "Select resource to give (0:Wood, 1:Brick, 2:Sheep, 3:Wheat, 4:Ore): ";
        int giveResourceInt;
        std::cin >> giveResourceInt;
        CatanResurseType giveResource = static_cast<CatanResurseType>(giveResourceInt);

        std::cout << "Select resource to receive (0:Wood, 1:Brick, 2:Sheep, 3:Wheat, 4:Ore): ";
        int receiveResourceInt;
        std::cin >> receiveResourceInt;
        CatanResurseType receiveResource = static_cast<CatanResurseType>(receiveResourceInt);

        if (tradeWithBank(*player,giveResource, receiveResource))
        {
            std::cout << "Bank trade successful" << std::endl;
        }
        else
        {
            std::cout << "Bank trade failed. Make sure you have 4 of the resource you want to trade." << std::endl;
        }
    }
    else
    {
        std::cout << "Invalid trade type" << std::endl;
    }
    }

bool Catan::tradeWithPlayer(Player &curr, Player &other, CatanResurseType give, CatanResurseType receive, int giveAmount, int receiveAmount)
{
    if (curr.trade(other, give, receive, giveAmount, receiveAmount))
    {
        return true;
    }
    return false;
}
   
bool Catan::tradeWithBank(Player& curr,CatanResurseType giveResource, CatanResurseType receiveResource){

if (curr.tradeWithBank(giveResource, receiveResource))
    {
         std::cout << "Bank trade successful" << std::endl;
        

        return true;
    }
    return false;
}

void Catan::chooseDevelopmentCard(Player* player) {
  if(player->getDevelopmentCards().size()==0){
 std::cout<<player->getName()<<" You have no development cards to use"<<std::endl;
 return;
  }
  std::cout << player->getName() << ", choose a development card to use:" << std::endl;
    const std::vector<DevelopmentCard>& cards = player->getDevelopmentCards();
    
    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << i + 1 << ". " << getDevelopmentCardName(cards[i]) << std::endl;
    }
    
    int choice;
    std::cout << "Enter your choice (0-" << cards.size() << "): ";
    std::cin >> choice;
  useDevelopmentCard(player ,choice)  ;
} 

void Catan::useDevelopmentCard(Player *player, int choice)
{
    const std::vector<DevelopmentCard>& cards = player->getDevelopmentCards();
    int index = choice ; // Convert to 0-based index

    if (index < 0 || index >= static_cast<int>(cards.size())) {
        std::cout << "Invalid choice. No development card was used." << std::endl;
        return;
    }

    DevelopmentCard card = cards[index];
    std::cout << player->getName() << " is using a " << getDevelopmentCardName(card) << " card." << std::endl;

    switch(card) {
        case DevelopmentCard::Knight:
            useKnightCard(player);
            break;
        case DevelopmentCard::RoadBuilding:
            useRoadBuildingCard(player);
            break;
        case DevelopmentCard::YearOfPlenty:
            useYearOfPlentyCard(player);
            break;
        case DevelopmentCard::Monopoly:
          std::cout << player->getName() << " is using a Monopoly card." << std::endl;
    
        std::cout << "Choose a resource type ,Wood 1, Brick 2, Sheep 3,  Wheat 4, Ore 5,"<<std::endl;;
    int  resourceType;
    std::cin >> resourceType;
            useMonopolyCard(player,CatanResurseType(resourceType));
            break;
        case DevelopmentCard::VictoryPoint:
            useVictoryPointCard(player);
            break;
        default:
            std::cout << "Unknown card type. No action taken." << std::endl;
            return;
    }

    player->removeDevelopmentCard(index);
    std::cout << "The " << getDevelopmentCardName(card) << " card has been used and removed from " 
              << player->getName() << "'s hand." << std::endl;
}

void Catan::useMonopolyCard(Player* player,CatanResurseType chosenResource) {
    std::cout << player->getName() << " is using a Monopoly card." << std::endl;
    
    int totalCollected = 0;
    
    // Collect the chosen resource from all other players
    for (Player& otherPlayer : players) {
        if (&otherPlayer != player) {
            int amount = otherPlayer.getResourceCardsAt(chosenResource);
            otherPlayer.removeResource(chosenResource, amount);
            player->addResourceCard(chosenResource, amount);
            totalCollected += amount;
        }
    }
    
    std::cout << player->getName() << " collected " << totalCollected  << " from other players." << std::endl;
}
void Catan::useRoadBuildingCard(Player* player) {
    std::cout << player->getName() << " is using a Road Building card." << std::endl;
    
    for (int i = 0; i < 2; ++i) {
        std::cout << "Building road " << (i+1) << " of 2:" << std::endl;
        size_t road1;
    std::cout << player->getName() << " is building..." << std::endl;
    bool validRoad = false;

    while (!validRoad)
    {
        std::cout << player->getName() << ", please choose a road number for the RoadSection: ";
        std::cin >> road1;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cout << "Invalid input. Please enter a valid number." << std::endl;
            continue;
        }
        validRoad =  buildRoadForFree(player,road1);
    }
    }
}

bool Catan::buildRoadForFree(Player* player,size_t indexRoad ){
   if (!board.isValidRoad(indexRoad))
    {
        std::cout << "Invalid road number. Please choose a valid road number." << std::endl;
        return false;
    }

    Road *roadPtr = board.getRoadArry().at(indexRoad);
    if (roadPtr == nullptr || roadPtr->getId().empty())
    {
        std::cout << "Invalid road. The road needs to be connected to a settlement. Please choose another road." << std::endl;
        return false;
    }

    if (!RoadconnectToBulding(player, roadPtr->getId()) && !RoadconnectToRodSec(player, roadPtr->getId()))
    {
        std::cout << "Invalid road. The road needs to be connected to a settlement. Please choose another road." << std::endl;
        return false;
    }

    RoadSection *newRoadSec = new RoadSection(player, roadPtr);
    roadSections.emplace_back(newRoadSec);
    player->addRoad(newRoadSec);
    roadPtr->setBuilding(newRoadSec);
    board.addRoadSection(*newRoadSec);

    std::cout << player->getName() << ", the RoadSection in Road: " << roadPtr->getId() << " built successfully\n";
    return true;
}

void Catan::useYearOfPlentyCard(Player* player) {
    std::cout << player->getName() << " is using a Year of Plenty card." << std::endl;
    
    for (int i = 0; i < 2; ++i) {
        std::cout << "Choose resource " << (i+1) << " ,Wood 1, Brick 2, Sheep 3,  Wheat 4, Ore 5,"<<std::endl;
        int  resourceType;
        std::cin >> resourceType;
        CatanResurseType chosenResource = CatanResurseType(resourceType);
        player->addResourceCard(chosenResource, 1);
        std::cout << "Added 1 " << resourceType << " to " << player->getName() << "'s hand." << std::endl;
    }
}
void Catan::useKnightCard(Player* player) {
    std::cout << player->getName() << " is using a Knight card." << std::endl;
    
    player->incrementKnightCount();
    // Check for largest army
    checkAndUpdateLargestArmy(player);
}

void Catan::checkAndUpdateLargestArmy(Player* player) {
    if (player->getKnightCount() >= 3 && player->getKnightCount() > largestArmyOwner->getKnightCount()) {
        if (largestArmyOwner != nullptr) {
            largestArmyOwner->addPoints(-2);
        }
        largestArmyOwner = player;
        player->addPoints(2);
        std::cout << player->getName() << " now has the largest army!" << std::endl;
    }
}
void Catan::useVictoryPointCard(Player* player) {
    std::cout << player->getName() << " is revealing a Victory Point card." << std::endl;
    player->addPoints(1);
    std::cout << player->getName() << " gained 1 victory point!" << std::endl;
}

void Catan::buyDevelopmentCard(Player *player)
{

    if(developmentDeck.empty()){
        std::cout<<"the development deck is empty\n";
    }

    if (player->buyDevelopmentCard(developmentDeck.front()))
    {
        developmentDeck.erase(developmentDeck.begin());
        std::cout << player->getName() << " buy development card"<<std::endl;
    }
    else{
    std::cout << player->getName() << " you can't buy DevelopmentCard" << std::endl;
}}

// Checks if there are no buildings around the specified vertex
bool Catan::noBuildingAround(const std::string& vertexId)
{
    std::vector<int> connectedIndices = getConnectedLandTiles(vertexId);

    for (size_t i = 0; i < connectedIndices.size(); i++)
    {
        int indexTile = connectedIndices.at(i);
        int indexVerinTile = getVertexIndexInTile(indexTile, vertexId);

        int prevIndexTile = ((indexVerinTile - 1 + 6) % 6);
        int nextIndexTile = ((indexVerinTile + 1) % 6);
        size_t prevInd = board.getTileVerticesIndex().at(indexTile).at(prevIndexTile);
        size_t nextInd = board.getTileVerticesIndex().at(indexTile).at(nextIndexTile);
        size_t currINd = board.getTileVerticesIndex().at(indexTile).at(indexVerinTile);

        if (!board.getVerticesArray().at(currINd)->isEmpty())
        {
            return false;
        }
        if (!board.getVerticesArray().at(nextInd)->isEmpty())
        {
            return false;
        }
        if (!board.getVerticesArray().at(prevInd)->isEmpty())
        {
            return false;
        }
    }
    return true;
}

bool Catan::SettekmentconnectToRodSec(Player* player, const std::string& vertexId)
{
    bool connect = false; // Initialize connect as false
    std::vector<int> connectedIndices = getConnectedLandTiles(vertexId);

    for (size_t i = 0; i < connectedIndices.size(); i++)
    {
        int indexTile = connectedIndices.at(i);
        int indexVer = getVertexIndexInTile(indexTile, vertexId);

        int prevRoad = (indexVer - 1 + 6) % 6;
        int nextRoad = indexVer;

        size_t indexInTilePrevRoad = board.getTileRoadsIndex().at(indexTile)[prevRoad];
        size_t indexInTileNextRoad = board.getTileRoadsIndex().at(indexTile)[nextRoad];

        // Check previous road
        if (indexInTilePrevRoad < board.getRoadArry().size() &&
            !board.getRoadArry().at(indexInTilePrevRoad)->isEmpty() &&
            (board.getRoadArry().at(indexInTilePrevRoad)->getBuilding()->getOwner() == player))
        {

            return true;
        }

        // Check next road
        if (indexInTileNextRoad < board.getRoadArry().size() &&
            !board.getRoadArry().at(indexInTileNextRoad)->isEmpty() &&
            board.getRoadArry().at(indexInTileNextRoad)->getBuilding()->getOwner() == player)
        {
            return true;
        }
    }

    return connect;
}

// Gets the index of the specified vertex on the specified tile
int Catan::getVertexIndexInTile(int tileIndex, const std::string& vertexId)
{
    // Validate the tile index
    if (tileIndex < 0 || static_cast<size_t>(tileIndex) >= board.getTileVerticesIndex().size())
    {
        std::cerr << "Invalid tile index!" << std::endl;
        return -1;
    }

    // Iterate through the vertices of the tile to find the matching vertex ID
    size_t i = 0;
    while (i < 6)
    {
        size_t vertexIndex = board.getTileVerticesIndex().at(tileIndex).at(i);
        if (
            board.getVerticesArray().at(vertexIndex)->getId() == vertexId)
        {
            return static_cast<int>(i);
        }
        ++i;
    }

    // Return -1 if the vertex ID is not found
    return -1;
}
// Gets the index of the specified road on the specified tile
int Catan::getRoadIndexInTile(int tileIndex, const std::string& roadId)
{
    // Validate the tile index
    if (tileIndex < 0 || static_cast<size_t>(tileIndex) >= board.getTileVerticesIndex().size())
    {
        std::cerr << "Invalid tile index!" << std::endl;
        return -1;
    }

    size_t i = 0;
    // Iterate through the roads in the specified tile
    while (i < 6)
    {
        size_t roadIndextile = board.getTileRoadsIndex().at(tileIndex).at(i);

        // Check if the road ID matches the specified road ID
        if (board.getRoadArry().at(roadIndextile)->getId() == roadId)
        {
            return static_cast<int>(i);
        }
        ++i;
    }

    // Return -1 if the road is not found
    return -1;
}
// Gets the indices of the tiles connected to the specified vertex or road
std::vector<int> Catan::getConnectedLandTiles(const std::string &id) const
{
    std::vector<int> connectedTiles;

    if (id.empty())
    {
        return connectedTiles;
    }

    for (char letter : id)
    {
        if (!std::isupper(letter))
        {
            continue;
        }

        int index = letter - 'A';
        connectedTiles.emplace_back(index);
    }

    return connectedTiles;
}

// Checks if the specified road is connected to a building owned by the current player
bool Catan::RoadconnectToBulding(Player* player, const std::string& roadId)
{
    std::vector<int> connectedTiles = getConnectedLandTiles(roadId);

    for (size_t i = 0; i < connectedTiles.size(); i++)
    {
        int indexTile = connectedTiles.at(i);
        int indexRoad = getRoadIndexInTile(indexTile, roadId);

        if (indexTile >= 0 && static_cast<size_t>(indexTile) < board.getTileVerticesIndex().size())
        {
            if (indexRoad >= 0 && static_cast<size_t>(indexRoad) < 6)
            {
                int nextIndexTile = (indexRoad + 1) % 6;
                int prevIndexTile = indexRoad;
                int nextIndexVarArry = board.getTileVerticesIndex()[indexTile][nextIndexTile];
                int prevIndexVarArry = board.getTileVerticesIndex()[indexTile][prevIndexTile];

                if (static_cast<size_t>(nextIndexVarArry) < board.getVerticesArray().size() && static_cast<size_t>(prevIndexVarArry) < board.getVerticesArray().size())
                {
                    Vertex *nextVertex = board.getVerticesArray().at(nextIndexVarArry);
                    Vertex *prevVertex = board.getVerticesArray().at(prevIndexVarArry);

                    if (nextVertex != nullptr && !nextVertex->isEmpty() && nextVertex->getBuilding()->getOwner() == player)
                    {
                        return true;
                    }

                    if (prevVertex != nullptr && !prevVertex->isEmpty() && prevVertex->getBuilding()->getOwner() == player)
                    {
                        return true;
                    }
                }
            }
            else
            {
                std::cout << "Invalid road index: " << indexRoad << std::endl;
            }
        }
        else
        {
            std::cout << "Invalid tile index: " << indexTile << std::endl;
        }
    }

    return false;
}
// Checks if the specified road is connected to another road section owned by the same player
bool Catan::RoadconnectToRodSec(Player* player, const std::string& roadId)
{
    // Get the list of land tiles connected to the given road
    std::vector<int> connectedTiles = getConnectedLandTiles(roadId);

    for (size_t i = 0; i < connectedTiles.size(); i++)
    {
        int indexTile = connectedTiles.at(i);
        int indexRoad = getRoadIndexInTile(indexTile, roadId);

        // Check if the tile index is valid
        if (indexTile >= 0 && static_cast<size_t>(indexTile) < board.getTileRoadsIndex().size())
        {
            // Check if the road index is valid
            if (indexRoad >= 0 && static_cast<size_t>(indexRoad) < 6)
            {
                // Calculate the indices of the adjacent road sections
                int nextIndex = board.getTileRoadsIndex().at(indexTile).at((indexRoad + 1) % 6);
                int prevIndex = board.getTileRoadsIndex().at(indexTile).at((indexRoad + 5) % 6); // Using +5 instead of -1 to avoid negative index

                // Check the next road section
                if (static_cast<size_t>(nextIndex) < board.getRoadArry().size() &&
                    board.getRoadArry().at(nextIndex)->getBuilding() != nullptr &&
                    board.getRoadArry().at(nextIndex)->getBuilding()->getOwner() == player)
                {
                    std::cout << "Next road segment is connected and owned by current player." << std::endl;
                    return true;
                }

                // Check the previous road section
                if (static_cast<size_t>(prevIndex) < board.getRoadArry().size() &&
                    board.getRoadArry().at(prevIndex)->getBuilding() != nullptr &&
                    board.getRoadArry().at(prevIndex)->getBuilding()->getOwner() == player)
                {
                    std::cout << "Previous road segment is connected and owned by current player." << std::endl;
                    return true;
                }
            }
            else
            {
                std::cout << "Invalid road index: " << indexRoad << std::endl;
            }
        }
        else
        {
            std::cout << "Invalid tile index: " << indexTile << std::endl;
        }
    }

    // Return false if no connected road segments are found
    return false;
}
std::string Catan::getResourceName(CatanResurseType resource) {
    switch(resource) {
        case CatanResurseType::Wood: return "Wood";
        case CatanResurseType::Brick: return "Brick";
        case CatanResurseType::Sheep: return "Sheep";
        case CatanResurseType::Wheat: return "Wheat";
        case CatanResurseType::Ore: return "Ore";
        default: return "Unknown";
    }
}
// Returns the current board state
Board *Catan::getBoard() const
{
    return &board;
}

// Returns the current player
Player *Catan::getCurrentPlayer()
{
    return &players.at(currentPlayerIndex);
}
std::string Catan::getDevelopmentCardName(DevelopmentCard card) {
    switch(card) {
        case DevelopmentCard::Knight: return "Knight";
        case DevelopmentCard::RoadBuilding: return "Road Building";
        case DevelopmentCard::YearOfPlenty: return "Year of Plenty";
        case DevelopmentCard::Monopoly: return "Monopoly";
        case DevelopmentCard::VictoryPoint: return "Victory Point";
        default: return "Unknown";
    }
}

Catan::~Catan()
{
    for (auto settelment : settlements)
    {
        delete settelment;
    }
    settlements.clear();

    for (auto city : cities)
    {
        delete city;
    }
    cities.clear();

    for (auto roadsec : roadSections)
    {
        delete roadsec;
    }
    roadSections.clear();
    delete &board;
}