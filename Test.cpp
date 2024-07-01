//talyape123@gmail.com 

#include "doctest.h"
#include "Board.hpp"
#include "Player.hpp"
#include "Settlement.hpp"
#include "City.hpp"
#include "RoadSection.hpp"
#include "Catan_enums.hpp"
#include "Catan.hpp"
#include <iostream>
#include <map>
//Vertex Test

TEST_CASE("Vertex building tests") {
    Vertex v1("B3");
    Player p1("tal");
    Player p2("hili");

    CHECK(v1.getId() == "B3");
    CHECK(v1.getBuilding() == nullptr);

    Settlement s1(&p1, &v1); 
    City c1(&p2, &v1);       

    v1.setBuilding(&s1);
    CHECK(v1.getBuilding()->getOwner()->getName() == "tal");

    v1.setBuilding(&c1);
    CHECK(v1.getBuilding()->getOwner()->getName() == "hili");
    CHECK(!v1.isEmpty());

}
//Tile Test
TEST_CASE("checking Tile"){
  Tile t1("A");
  CHECK(t1.getId()=="A");  
  CHECK(t1.getNumber()==0);  
  CHECK(t1.getResource()==CatanResurseType::none);  
  t1.setNumber(4);
  t1.setResource(CatanResurseType::Brick);
  CHECK(t1.getNumber()==4);  
  CHECK(t1.getResource()==CatanResurseType::Brick); 

}

//Buldings Test
TEST_CASE("create city and Settelment"){
    Vertex v1("C3");
     Vertex v2("B3");

    Player p1("tal");
    Player p2("hili");

    Settlement s1(&p1, &v1); 
    City c1(&p2, &v2);       
    CHECK(s1.getLocation()->getId()=="C3");
    CHECK(c1.getLocation()->getId()=="B3");

    CHECK(s1.getOwner()->getName()=="tal");
    CHECK(c1.getOwner()->getName()=="hili");

}

// Board Tests
TEST_CASE("Check resource quantities") {
    Board board;
    board.setupBoard();
    Player p1("p1");
    Settlement s1(&p1,board.getVerticesArray().at(0));
    board.addSettlement(&s1);
    CHECK(board.getSettlements().size()==1);
    board.removeSettlement(&s1);
    CHECK(board.getSettlements().size()==0);   
    std::map<CatanResurseType, int> resourceCount;
    for (size_t i = 0; i < 19; ++i) {
        Tile tile = board.getTile(i);
        CatanResurseType resource = tile.getResource();
        resourceCount[resource]++;
    }

    // Check resource quantities
    REQUIRE(resourceCount[CatanResurseType::Wood] == 4);
    REQUIRE(resourceCount[CatanResurseType::Brick] == 3);
    REQUIRE(resourceCount[CatanResurseType::Sheep] == 4);
    REQUIRE(resourceCount[CatanResurseType::Wheat] == 4);
    REQUIRE(resourceCount[CatanResurseType::Ore] == 3);
    REQUIRE(resourceCount[CatanResurseType::Desert] == 1);

   
}

TEST_CASE("Check number distribution and desert") {
    Board board;
    board.setupBoard();

    std::map<int, int> numberCount;

    for (size_t i = 0; i < 19; ++i) {
        Tile tile = board.getTile(i);
        CatanResurseType resource = tile.getResource();
        int number = tile.getNumber();

        if (resource == CatanResurseType::Desert) {
            REQUIRE(number == 0);
        } else {
            REQUIRE(number != 0);
            numberCount[number]++;
        }


    }
}

TEST_CASE("is Valid Road and vertex") {
    Board board;
    REQUIRE(!board.isValidRoad(-1));
     REQUIRE(board.isValidRoad(0));
    REQUIRE(board.isValidRoad(20));
    REQUIRE(board.isValidRoad(65));
    REQUIRE(!board.isValidRoad(72));

    REQUIRE(!board.isValidVertex(-1));
    REQUIRE(board.isValidVertex(0));
    REQUIRE(board.isValidVertex(22));
    REQUIRE(!board.isValidVertex(54));
    
}

//Player Test
TEST_CASE("Player operations") {
    Player player("Test Player");

    SUBCASE("Initial state") {
        CHECK(player.getName() == "Test Player");
        CHECK(player.getBuldingCount(BuldingType::settlement) == 5);
        CHECK(player.getBuldingCount(BuldingType::city) == 4);
        CHECK(player.getBuldingCount(BuldingType::roadSection) == 15);
        
        // Check initial resources
        for (const auto& resource : {CatanResurseType::Wood, CatanResurseType::Brick, CatanResurseType::Sheep, CatanResurseType::Wheat, CatanResurseType::Ore}) {
            CHECK(player.getResourceCardsAt(resource) == 0);
        }
    }}

TEST_CASE("Adding resources") {
    Player player("Test Player");
    player.addResourceCard(CatanResurseType::Wood,1);
    player.addResourceCard(CatanResurseType::Brick,1);
    CHECK(player.getResourceCardsAt(CatanResurseType::Wood) == 1);
    CHECK(player.getResourceCardsAt(CatanResurseType::Brick) == 1);
}

TEST_CASE("Buying and adding settlement and city") {
    Player player("Test Player");
    Vertex vertex("A3");
    Settlement settlement(&player ,&vertex);
    // Try to buy without resources
    bool bought = player.buySettelment(&settlement);
    CHECK(bought == false);

    // Add required resources
    player.addResourceCard(CatanResurseType::Wood,1);
    player.addResourceCard(CatanResurseType::Brick,1);
    player.addResourceCard(CatanResurseType::Sheep,1);
    player.addResourceCard(CatanResurseType::Wheat,1);

    bought = player.buySettelment(&settlement);
    CHECK(bought == true);

    CHECK(player.getResourceCardsAt(CatanResurseType::Wood) == 0);
    CHECK(player.getResourceCardsAt(CatanResurseType::Brick) == 0);
    CHECK(player.getResourceCardsAt(CatanResurseType::Sheep) == 0);
    CHECK(player.getResourceCardsAt(CatanResurseType::Wheat) == 0);
    CHECK(player.getSettlements().size() == 1);
    CHECK(player.getCities().size() == 0);
    CHECK(player.getBuldingCount(BuldingType::settlement) == 4);
    CHECK(player.getPoints()==1);
    
    City city(&player, &vertex);
    
    CHECK(player.getResourceCardsAt(CatanResurseType::Wheat) == 0);
    CHECK(player.getResourceCardsAt(CatanResurseType::Ore) == 0);

    // Try to buy without resources
    bool aaa = player.buyCity(&city);
    CHECK(aaa == false);

    // Add required resources
    player.addResourceCard(CatanResurseType::Wheat,2);
    player.addResourceCard(CatanResurseType::Ore,3);
    

    aaa = player.buyCity(&city);
    CHECK(aaa == true);
    CHECK(player.getResourceCardsAt(CatanResurseType::Wheat) == 0);
    CHECK(player.getResourceCardsAt(CatanResurseType::Ore) == 0);
    CHECK(player.getCities().size() == 1);
    CHECK(player.getBuldingCount(BuldingType::city) == 3);
    CHECK(player.getPoints()==2);
    CHECK(player.getCities().size() == 1);

}

TEST_CASE("Buying and adding road") {
    Player player("Test Player");
    Road road("AB");
    RoadSection roadSection(&player, &road);

    // Try to buy without resources
    bool bought = player.buyRoadSection(&roadSection);
    CHECK(bought == false);

    // Add required resources
    player.addResourceCard(CatanResurseType::Wood,1);
    player.addResourceCard(CatanResurseType::Brick,1);

    bought = player.buyRoadSection(&roadSection);
    CHECK(bought == true);
    CHECK(player.getResourceCardsAt(CatanResurseType::Wood) == 0);
    CHECK(player.getResourceCardsAt(CatanResurseType::Brick) == 0);
    CHECK(player.getRoads().size() == 1);
    CHECK(player.getBuldingCount(BuldingType::roadSection) == 14);

}

TEST_CASE("Updating building counts") {
    Player player("Test Player");
    player.changeBuldings(BuldingType::settlement, 1);
    CHECK(player.getBuldingCount(BuldingType::settlement) == 6);
    player.changeBuldings(BuldingType::city, -2);
    CHECK(player.getBuldingCount(BuldingType::city) == 2);
}

//catanTest:
TEST_CASE("valid bulding"){
Player p1("p1");
Player p2("p2");
Player p3("p3");
Catan catan(p1,p2,p3);

/*start buldingg, chack that settelment not build 1 road far from enother and show that 
in the start bulding you can build settlment that not connect to your road section*/

bool isValid_s1_p1 =catan.BuildStartSettlement(&p1,13);//id:"BEF"

CHECK(isValid_s1_p1==true);
//the settelment not connect to settelment or road section the settelment is far.
bool isValid_r1_p1 =catan.BuildStartRoadSec(&p1,0);//ID: "A23"
CHECK(isValid_r1_p1==false);
//the roat index is in the sane tile but not connect to settelment.
isValid_r1_p1=catan.BuildStartRoadSec(&p1,27);//ID: "EJ"
CHECK(isValid_r1_p1==false);
//valid road , connect the settelment .
isValid_r1_p1=catan.BuildStartRoadSec(&p1,20);//ID: "BF"
CHECK(isValid_r1_p1==true);
//the var is not empty
bool isValid_s1_p2 =catan.BuildStartSettlement(&p2,13);////ID: "BCF"
CHECK(isValid_s1_p2==false);
//tp close to enother settelment
isValid_s1_p2 =catan.BuildStartSettlement(&p2,9);////ID: "BCF"
CHECK(isValid_s1_p2==false);
//valid settelment.
isValid_s1_p2 =catan.BuildStartSettlement(&p2,0);//ID: "A3"
CHECK(isValid_s1_p2==true);

CHECK(p1.getSettlements().size()==1);
CHECK(p2.getSettlements().size()==1);
CHECK(p1.getRoads().size()==1);

/*/buy and bulding*/


bool p2_roadSec1 =catan.checkToBuyRoadSec(&p2,27);
CHECK(p2_roadSec1==false);

p1.addResourceCard(CatanResurseType::Brick,1);
p1.addResourceCard(CatanResurseType::Wood,1);

//road 27 connect to roadSection that p1 owner 
 p2_roadSec1 =catan.checkToBuyRoadSec(&p2,27);
CHECK(p2_roadSec1==false);

bool p1_roadSec1 =catan.checkToBuyRoadSec(&p1,27);
CHECK(p1_roadSec1==true);
//check to buy settelment whithout resource card
 bool p1_sett5 =catan.checkToBuySett(&p1,23);
CHECK(p1_sett5==true);
REQUIRE(catan.getBoard()->getRoadArry().at(23)->isEmpty());

p1.addResourceCard(CatanResurseType::Brick,1);
p1.addResourceCard(CatanResurseType::Wood,1);
p1.addResourceCard(CatanResurseType::Sheep,1);
p1.addResourceCard(CatanResurseType::Wheat,1);

p2.addResourceCard(CatanResurseType::Brick,1);
p2.addResourceCard(CatanResurseType::Wood,1);
p2.addResourceCard(CatanResurseType::Sheep,1);
p2.addResourceCard(CatanResurseType::Wheat,1);
//check if another player can build settelment whithput roadSection connect.
bool p2_settelment =catan.checkToBuySett(&p2,23);
CHECK(p2_settelment==false);
bool p1_settelment =catan.checkToBuySett(&p1,23);
CHECK(p1_settelment==true);
p2.addResourceCard(CatanResurseType::Wheat,2);
p2.addResourceCard(CatanResurseType::Ore,3);

//check if you cant build city in vertex  whit settelment that another player owner
bool p2_city=catan.checkToBuyCity(&p2,23);
CHECK(p2_city==false);
p1.addResourceCard(CatanResurseType::Wheat,2);
//check if you can buy city whithout all the resurse:
 bool p1_city=catan.checkToBuyCity(&p1,23);
CHECK(p1_city==true);
CHECK(catan.getBoard()->getVerticesArray().at(23)->getBuilding()->getType()=="Settlement");
p1.addResourceCard(CatanResurseType::Wheat,2);
p1.addResourceCard(CatanResurseType::Ore,3);

size_t numsett =p1.getSettlements().size();
p1_city=catan.checkToBuyCity(&p1,23);
 CHECK(p1_city==true);
 CHECK(catan.getBoard()->getVerticesArray().at(23)->getBuilding()->getType()=="City");
CHECK(p1.getSettlements().size()==numsett-1);
int cityNmP1 =p1.getCities().size();
int settNumP1 =p1.getSettlements().size();
CHECK(2*cityNmP1+settNumP1==p1.getPoints());

int cityNmP2 =p2.getCities().size();
int settNumP2 =p2.getSettlements().size();
CHECK(2*cityNmP2+settNumP2==p2.getPoints());
}

TEST_CASE("trade"){
    std::cout<<"trade test :"<<std::endl;
Player p1("p1");
Player p2("p2");
Player p3("p3");
Catan catan(p1, p2, p3);
p1.addResourceCard(CatanResurseType::Brick,1);
p2.addResourceCard(CatanResurseType::Wood,1);
bool trade1=catan.tradeWithPlayer(p2,p1,CatanResurseType::Brick,CatanResurseType::Wood,1,1);
REQUIRE(!trade1);
trade1=catan.tradeWithPlayer(p1,p2,CatanResurseType::Brick,CatanResurseType::Wood,2,1);
REQUIRE(!trade1);
trade1=catan.tradeWithPlayer(p1,p2,CatanResurseType::Brick,CatanResurseType::Sheep,1,1);
REQUIRE(!trade1);
CHECK(p1.getResourceCardsAt(CatanResurseType::Brick)==1);
CHECK(p1.getResourceCardsAt(CatanResurseType::Wood)==0);
CHECK(p2.getResourceCardsAt(CatanResurseType::Brick)==0);
CHECK(p2.getResourceCardsAt(CatanResurseType::Wood)==1);
bool trade2=catan.tradeWithPlayer(p1,p2,CatanResurseType::Brick,CatanResurseType::Wood,1,1);
REQUIRE(trade2);
CHECK(p1.getResourceCardsAt(CatanResurseType::Brick)==0);
CHECK(p1.getResourceCardsAt(CatanResurseType::Wood)==1);
CHECK(p2.getResourceCardsAt(CatanResurseType::Brick)==1);
CHECK(p2.getResourceCardsAt(CatanResurseType::Wood)==0);

bool tradeWhithBank1=catan.tradeWithBank(p1,CatanResurseType::Wood,CatanResurseType::Sheep);
REQUIRE(!tradeWhithBank1);
CHECK(p1.getResourceCardsAt(CatanResurseType::Wood)==1);
CHECK(p1.getResourceCardsAt(CatanResurseType::Sheep)==0);

p1.addResourceCard(CatanResurseType::Wood,3);
 tradeWhithBank1=catan.tradeWithBank(p1,CatanResurseType::Wood,CatanResurseType::Sheep);
REQUIRE(tradeWhithBank1);
CHECK(p1.getResourceCardsAt(CatanResurseType::Wood)==0);
CHECK(p1.getResourceCardsAt(CatanResurseType::Sheep)==1);
}

TEST_CASE("develop card"){
    std::cout<<"develop card test: "<<std::endl;
    Player p1("p1");
    Player p2("p2");
    Player p3("p3");
    Catan catan(p1,p2,p3);
    catan.initializeDevelopmentDeck();
    p1.addResourceCard(CatanResurseType::Ore,1);
    p1.addResourceCard(CatanResurseType::Wheat,1);
    //whithout all the resurce
     catan.buyDevelopmentCard(&p1);
    CHECK(p1.getDevelopmentCards().size()==0);
    p1.addResourceCard(CatanResurseType::Sheep,1);
    catan.buyDevelopmentCard(&p1);
    CHECK(p1.getDevelopmentCards().size()==1);
    catan.useDevelopmentCard(&p1,0);
    CHECK(p1.getDevelopmentCards().size()==0);
    p2.addResourceCard(CatanResurseType::Ore,1);
    p2.addResourceCard(CatanResurseType::Wheat,1);
    p2.addResourceCard(CatanResurseType::Sheep,1);
    catan.buyDevelopmentCard(&p2);
}