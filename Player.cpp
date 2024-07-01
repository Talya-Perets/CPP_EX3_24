//talyape123@gmail.com 

#include "Player.hpp"

Player::Player(const std::string& name) : name(name), points(0) ,knightCount(0){
    resourceCards = {
        {CatanResurseType::Wood, 0},
        {CatanResurseType::Brick, 0},
        {CatanResurseType::Sheep, 0},
        {CatanResurseType::Wheat, 0},
        {CatanResurseType::Ore, 0}
    };
    

    // Initialize building counts
    buldings = {
        {BuldingType::roadSection, 15},
        {BuldingType::settlement, 5},
        {BuldingType::city, 4}
    };
}

const std::string& Player::getName() const {
    return name;
}

int Player::getBuldingCount(BuldingType bulding) {
    return buldings.at(bulding);
}

void Player::changeBuldings(BuldingType bulding, int num) {
    buldings[bulding] += num;
}

void Player::addResourceCard(const CatanResurseType card ,int count) {
    resourceCards[card]+=count;
}

void Player::removeDevelopmentCard(int index) {
    if (index >= 0 && static_cast<size_t>(index) < developmentCards.size()) {
        developmentCards.erase(developmentCards.begin() + index);
    }
}

void Player::addDevelopmentCard(const DevelopmentCard& card) {
    developmentCards.push_back(card);
}

void Player::addSettlement(Settlement* settlement) {
    settlements.push_back(settlement);
    buldings[BuldingType::settlement]--;
    points++;
}

void Player::addRoad(RoadSection* roadSection) {
    roadSections.push_back(roadSection);
    buldings[BuldingType::roadSection]--;
}

void Player::addCity(City* city) {
    // Iterate through the settlements vector to find a matching location
    for (std::vector<Settlement*>::size_type i = 0; i < settlements.size(); i++) {
        // Check if the location IDs match
        if (settlements.at(i)->getLocation()->getId() == city->getLocation()->getId()) {
            // Erase the settlement at the matching index
            settlements.erase(settlements.begin() + i);
            break; // Exit the loop after finding the matching settlement
        }
    }
    
    // Add the new city to the cities vector
    cities.push_back(city);

    // Update player points and building counts
    points++;
    buldings[BuldingType::city]--;
    buldings[BuldingType::settlement]++;
}

bool Player::buySettelment(Settlement* settelment) {
    if (resourceCards[CatanResurseType::Brick] >= 1 && resourceCards[CatanResurseType::Sheep] >= 1 &&
        resourceCards[CatanResurseType::Wheat] >= 1 && resourceCards[CatanResurseType::Wood] >= 1) {
        resourceCards[CatanResurseType::Brick]--;
        resourceCards[CatanResurseType::Sheep]--;
        resourceCards[CatanResurseType::Wheat]--;
        resourceCards[CatanResurseType::Wood]--;
        addSettlement(settelment);
        return true;
    }
    std::cout << "Cannot buy settelment . Insufficient resources." << std::endl;
    return false;
}

bool Player::buyCity(City* city) {
    // Check if the player has enough resources to buy a city
    if (resourceCards[CatanResurseType::Wheat] >= 2 && resourceCards[CatanResurseType::Ore] >= 3) {
        // Deduct the required resources from the player's resource cards
        resourceCards[CatanResurseType::Wheat] -= 2;
        resourceCards[CatanResurseType::Ore] -= 3;
        
        // Add the city to the player's list of cities
        addCity(city);
        
        // Return true indicating the city purchase was successful
        return true;
    }

    // Print a message if the player does not have enough resources
    std::cout << "Cannot buy city. Insufficient resources." << std::endl;
    
    // Return false indicating the city purchase was not successful
    return false;
}

bool Player::buyRoadSection(RoadSection *roadSection) {
    if (resourceCards[CatanResurseType::Wood] >= 1 && resourceCards[CatanResurseType::Brick] >= 1) {
        resourceCards[CatanResurseType::Wood]--;
        resourceCards[CatanResurseType::Brick]--;
        addRoad(roadSection);
        return true;
    }
    std::cout << "Cannot buy road section . Insufficient resources." << std::endl;
    return false;
}
bool Player::buyDevelopmentCard(DevelopmentCard developmentCard) {
    
    if (resourceCards[CatanResurseType::Ore] >= 1 &&
        resourceCards[CatanResurseType::Sheep] >= 1 &&
        resourceCards[CatanResurseType::Wheat] >= 1) {
        resourceCards[CatanResurseType::Ore]--;
        resourceCards[CatanResurseType::Sheep]--;
        resourceCards[CatanResurseType::Wheat]--;

        addDevelopmentCard(developmentCard);
        return true;
    } else {
        
        return false;
    }
}

void Player::removeSettlement(Settlement* settlement) {
    auto it = std::find(settlements.begin(), settlements.end(), settlement);
    if (it != settlements.end()) {
        settlements.erase(it);
    }
    }

  void Player::removeResource(CatanResurseType type, int amount) {
    if (resourceCards[type] >= amount) {
        resourceCards[type] -= amount;
    
    }}

bool Player::trade(Player& other, CatanResurseType give, CatanResurseType receive, int giveAmount, int receiveAmount) {
    this->printResourceCards();
    other.printResourceCards();
    if(resourceCards[give]>=giveAmount && other.getResourceCardsAt(receive)>=receiveAmount){        // Add received resources
       
         this->addResourceCard(receive,receiveAmount);
        
        
        // Add given resources to the other player
         other.addResourceCard(give,giveAmount);
      
        // Restore resources if trade failed
        this->removeResource(give, giveAmount);
      
        other.removeResource(receive, receiveAmount);
        std::cout <<"the trad with "<<other.getName()<<" was completed successfully"<<std::endl;
    return true;
    }else{
            std::cout << "Trade failed: insufficient resources" << std::endl;
            return false;

}}

bool Player::tradeWithBank(CatanResurseType give, CatanResurseType receive)
{
    if (resourceCards[give] >= 4)
    {
        removeResource(give, 4);
        addResourceCard(receive,1);
        return true;
    }
    return false;
}

void Player::removeDevelopmentCard(size_t index) {
        if (index < developmentCards.size()) {
            developmentCards.erase(developmentCards.begin() + index);
        }
    }

void Player::incrementKnightCount(){
    knightCount++;
}
    void Player::addPoints(int amount){
        points+=amount;
    }

void Player::discardHalfResources() {
    int totalResources = resourceCards.size();
    if (totalResources > 7) {
        int cardsToDiscard = totalResources / 2;
        std::cout << getName() << " needs to discard " << cardsToDiscard << " cards." << std::endl;
        
        for (int i = 0; i < cardsToDiscard; i++) {
            // Choose a random resource to discard
            CatanResurseType resourceToDiscard = getRandomResource();
            removeResource(resourceToDiscard, 1);
            std::cout << "Discarded 1 " << getResourceName(resourceToDiscard) << std::endl;
        }
    }
}

// Get a random resource that the player has
CatanResurseType Player::getRandomResource() {
    std::vector<CatanResurseType> availableResources;
    for (int i = 0; i < 5; i++) {
        if ((resourceCards.at(CatanResurseType(i))) > 0) {
            availableResources.push_back(static_cast<CatanResurseType>(i));
        }
    }
    if (availableResources.empty()) {
        return CatanResurseType::Wood; // Or any other default value
    }
    return availableResources[rand() % availableResources.size()];
}

// Convert resource type to string for output
std::string Player::getResourceName(CatanResurseType resource) {
    switch(resource) {
        case CatanResurseType::Wood: return "Wood";
        case CatanResurseType::Brick: return "Brick";
        case CatanResurseType::Sheep: return "Sheep";
        case CatanResurseType::Wheat: return "Wheat";
        case CatanResurseType::Ore: return "Ore";
        default: return "Unknown";
    }
}

void Player::printResourceCards() {
    std::cout << getName() << "'s Resource Cards:" << std::endl;
    for (const auto& pair : resourceCards) {
        std::cout << getResourceName(pair.first) << ": " << pair.second << std::endl;
    }
}

 int Player::getKnightCount ()const{
return knightCount;
 }

const std::map<CatanResurseType, int> Player::getResourceCards() const {
    return resourceCards;
}

int Player::getResourceCardsAt(CatanResurseType type) {
    return resourceCards.at(type);
}

const std::vector<DevelopmentCard>& Player::getDevelopmentCards() const {
    return developmentCards;
}

const std::vector<Settlement*>& Player::getSettlements() const {
    return settlements;
}

const std::vector<RoadSection*>& Player::getRoads() const {
    return roadSections;
}

const std::vector<City*>& Player::getCities() const {
    return cities;
}
int Player::getPoints(){
        return points;
    }

   bool Player::operator==(const Player& other) const{
      return(name==other.getName());
   }
     


   