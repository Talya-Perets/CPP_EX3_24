//talyape123@gmail.com 

#include "Board.hpp"
#include "Catan.hpp"

#include <iostream>

int main(){
    
   Player p1("tal");
   Player p2("nevo");
   Player p3("hila");
   Catan catan(p1, p2, p3);
   catan.getBoard()->printBoard();
   catan.initializeDevelopmentDeck();
   catan.chooseStartPlayer();
   
  //simulate the first building

   //place at vertex "A3"
   catan.BuildStartSettlement(&p1,0);
   //place in road "A23"
   catan.BuildStartRoadSec(&p1,0);
//try to build in vertex too close from "A3"
  catan.BuildStartSettlement(&p2,7);//shuld print eror message
  catan.BuildStartSettlement(&p2,12); //BUILD IN "AD"
 //TRYING TO BUILD ROAD SECTION THAT NOT CONNECT TO THE SETTELMENT
  catan.BuildStartRoadSec(&p2,26);//shuld print eror message
  catan.BuildStartRoadSec(&p2,10);//BUILD IN "A22"
   //build sett in "B3" and road section "B34"
   catan.BuildStartSettlement(&p3,1);
   catan.BuildStartRoadSec(&p3,3);

    // give the player's the resurces
    catan.distributeResources(catan.rollDice());
    //p1 trying to trade if the trade faulde we get eror message
    catan.tradeWithPlayer(p1,p2,CatanResurseType::Brick,CatanResurseType::Sheep,1,1);
     catan.checkToBuyRoadSec(&p2,18);

    
    catan.init();


    return 0;
}