#include "battle.h"

int main() {
    tuple<int,int,int> location (0,0,0);
    readStats("stats.txt");
    vector<Unit> yourUnits;
    yourUnits.push_back(Unit("Tank1","tanks",32,10,8,location,4));
    yourUnits.push_back(Unit("yourInfantry1","infantry",10,2,2,location,2));
    vector<Unit> theirUnits;
    theirUnits.push_back(Unit("mechanizedInfantry1","mechanizedInfantry",80,10,4,location,2));
    theirUnits.push_back(Unit("theirInfantry1","infantry",10,2,2,location,2));
    
    
    unordered_map<string,Order> results = fight(yourUnits,theirUnits);
}