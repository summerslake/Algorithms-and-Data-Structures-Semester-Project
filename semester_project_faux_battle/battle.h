#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <sstream>

using namespace std;


bool DEBUG = false;

//Global variable to store bestAgainst data of each UNIT_TYPE
unordered_map<string,vector<string>> typeBestAgainst;
//Global variable to store class of each UNIT_TYPE
unordered_map<string,string> typeClass;

struct Unit {
    string ID;
    string unitClass;
    double currentHP;
    int speed;
    double power;
    tuple<int,int,int> currentLocation;
    int range;
    
    Unit(string id, string cl, double chp, int spd, double pwr, tuple<int,int,int> curloc, int rng) : 
    ID(id), unitClass(cl), currentHP(chp), speed(spd), power(pwr), currentLocation(curloc), range(rng) {}
};

struct Order {
    vector<string> actions; // contains consecutive moves up to the value of speed
    vector<char> moveSequence; // if moving, contains 'L','R','U','D' to give in what direction to move
    vector<string> targetIDs; // a vector giving the ID values of each unit to be targeted (in order).
    // to target the same unit until destruction, then move on, put the first unit's targetID X times,
    // where X is your speed; then put the next unit's targetID.
    string targetClass;  // if no targetIDs (or if all targetIDs destroyed), unit will target whatever
    // unit class is specified here until they are all destroyed.
};

// Splits a string on a delimiter
vector<string> split(const string& s, char delimiter) {
    //Re-used from function supplied in lab 3
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while (getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

//Prep function
//Passed in a file of the stats of each UNIT_TYPE
void readStats(string statFile) {
    //Open input
    ifstream input(statFile);
    
    string UNIT_TYPE;
    string CLASS;
    int HP;
    int SPEED;
    double POWER;
    int RANGE;
    string BEST_AGAINST;
    
    
    while(!input.eof()) {
        //Store data for units
        input >> UNIT_TYPE;
        input >> CLASS;
        input >> HP;
        input >> SPEED;
        input >> POWER;
        input >> RANGE;
        input >> BEST_AGAINST;
        
        //Store which class the unit type is
        typeClass[UNIT_TYPE] = CLASS;
        
        //Split BEST_AGAINST into a vector with delimiter ","
        vector<string> bestAgainstVector = split(BEST_AGAINST,',');
        
        //Store which classes the unit type is
        typeBestAgainst[UNIT_TYPE] = bestAgainstVector;
        
        if (DEBUG) {
            //Print unit that was just added
            cout << "UNIT_TYPE: " << UNIT_TYPE << " added" << endl;
            cout << "Class: " << typeClass[UNIT_TYPE] << endl;
            cout << "Best Against: ";
            for(int i=0; i<typeBestAgainst[UNIT_TYPE].size(); i++) {
                cout << typeBestAgainst[UNIT_TYPE].at(i) << " ";
            }
            cout << endl << endl;
        }
    }
}

//Comparator for Unit to sort by HP
bool compareByHP(const Unit &a, const Unit &b)
{
    return a.currentHP > b.currentHP;
}

//Comparator for Unit to sort by power
bool compareByPower(const Unit &a, const Unit &b)
{
    return a.power > b.power;
}

//Execution function
//Passed in a vector of units for you and your opponent
//Return ID's and what you want it to do
unordered_map<string,Order> fight(vector<Unit> yourUnits, vector<Unit> theirUnits) {
    //Create map to store orders
    unordered_map<string,Order> yourUnitsOrders;
    
    //Sort yourUnits by power (highest first)
    sort(yourUnits.begin(),yourUnits.end(),compareByPower);
    //Sort theirUnits by HP (highest first)
    sort(theirUnits.begin(),theirUnits.end(),compareByHP);
    
    //For each yourUnit, attack the most bestAgainst classes possible
    for(int i=0; i<yourUnits.size();i++) {
        
        if (DEBUG) {
            cout << "\n\nWorking with yourunitID: " << yourUnits.at(i).ID << endl;
            cout << "Class: " << typeClass[yourUnits.at(i).unitClass] << endl;
            cout << "Best against: " << typeBestAgainst[yourUnits.at(i).unitClass].at(0) << endl << endl;
        }
        
        //For each bestAgainst
        for(int k=0; k<typeBestAgainst[yourUnits.at(i).unitClass].size(); k++) {
            //Search theirUnits for bestAgainst class
            for(int j=0;j<theirUnits.size();j++) {
                
                if (DEBUG) { cout << "Checking theirunitID: " << theirUnits.at(j).ID << endl;}
                
                    //While their class is the first one your unit is best against, their HP > 0, and you have
                    //moves left (action vector size < speed), attack
                    while(typeClass[theirUnits.at(j).unitClass] == typeBestAgainst[yourUnits.at(i).unitClass].at(k) &&
                        theirUnits.at(j).currentHP > 0 &&
                        yourUnitsOrders[yourUnits.at(i).ID].actions.size() < yourUnits.at(i).speed) {
                            //Note action and targetID in unordered_map for return
                            yourUnitsOrders[yourUnits.at(i).ID].actions.push_back("fight");
                            yourUnitsOrders[yourUnits.at(i).ID].targetIDs.push_back(theirUnits.at(j).ID);
                            
                            //Set targetClass to what unit type it is best against
                            yourUnitsOrders[yourUnits.at(i).ID].targetClass = typeBestAgainst[yourUnits.at(i).unitClass].at(k);
                            
                            if (DEBUG) {
                                //Print action
                                cout << yourUnits.at(i).ID << " took action: ";
                                cout << yourUnitsOrders[yourUnits.at(i).ID].actions.back();
                                cout << "   on " << theirUnits.at(j).ID << endl;
                                cout << yourUnits.at(i).ID << "'s action vector size is now: " << yourUnitsOrders[yourUnits.at(i).ID].actions.size() << endl;
                            }
                            
                            //Update currentHP of unit you just attacked
                            theirUnits.at(j).currentHP -= 2 * yourUnits.at(i).power;
                            
                            if (DEBUG) {
                                //Print remaining HP
                                cout << theirUnits.at(j).ID << "'s HP is now: " << theirUnits.at(j).currentHP << "\n\n";
                            }
                    }
                
                //If you don't have moves left, break the for loop searching for theirUnit with bestAgainst
                if(yourUnitsOrders[yourUnits.at(i).ID].actions.size() == yourUnits.at(i).speed) {
                    if (DEBUG) { cout << "Breaking the loop" << endl << endl; }
                    break;
                }
            }
            //If you don't have moves left, break the loop going through each bestAgainst of the yourUnit
            if(yourUnitsOrders[yourUnits.at(i).ID].actions.size() == yourUnits.at(i).speed) {
                    if (DEBUG) { cout << "Breaking the loop" << endl << endl; }
                    break;
            }
        }
    }

    if (DEBUG) { cout << "\nDone with best against, re-sorting and moving on to highest HP\n\n" << endl; };
    //Sort theirUnits by HP (highest first) again
    sort(theirUnits.begin(),theirUnits.end(),compareByHP);
    
    //For each yourUnit, attack the highest HP thing possible
    for(int i=0; i<yourUnits.size();i++) {
        //Find the next thing with health
        for(int j=0;j<theirUnits.size();j++) {
            //While theirUnit has HP > 0, and you have moves left (action vector size < speed), attack
            while(theirUnits.at(j).currentHP > 0 &&
                yourUnitsOrders[yourUnits.at(i).ID].actions.size() < yourUnits.at(i).speed) {
                    //Note action and targetID in unordered_map for return
                    yourUnitsOrders[yourUnits.at(i).ID].actions.push_back("fight");
                    yourUnitsOrders[yourUnits.at(i).ID].targetIDs.push_back(theirUnits.at(j).ID);
                    
                    if (DEBUG) {
                        //Print action
                        cout << yourUnits.at(i).ID << " took action: ";
                        cout << yourUnitsOrders[yourUnits.at(i).ID].actions.back();
                        cout << "   on " << theirUnits.at(j).ID << endl;
                        cout << yourUnits.at(i).ID << "'s action vector size is now: " << yourUnitsOrders[yourUnits.at(i).ID].actions.size() << endl;
                    }
                    
                    //Update currentHP of unit you just attacked
                    theirUnits.at(j).currentHP -= yourUnits.at(i).power;
                    
                    if (DEBUG) {
                        //Print remaining HP
                        cout << theirUnits.at(j).ID << "'s HP is now: " << theirUnits.at(j).currentHP << "\n\n";
                    }
            }
            //If you don't have moves left, break the inner for loop and move to the next yourUnit
            if(yourUnitsOrders[yourUnits.at(i).ID].actions.size() == yourUnits.at(i).speed) {
                //cout << "Breaking the loop" << endl << endl;
                break;
            }
        }
        //If moves left and can't attack anything else, move
    }
    
    if (DEBUG) {
        //Print off yourUnitsOrders for testing
        for(int i=0; i<yourUnits.size(); i++) {
            cout << "yourUnit " << yourUnits.at(i).ID <<" took actions: " << endl;
            for(int j=0; j<yourUnitsOrders[yourUnits.at(i).ID].actions.size(); j++) {
                cout << yourUnitsOrders[yourUnits.at(i).ID].actions.at(j) << " on ";
                cout << yourUnitsOrders[yourUnits.at(i).ID].targetIDs.at(j) << endl;
            }
            cout << "yourUnit's target class was: " << yourUnitsOrders[yourUnits.at(i).ID].targetClass << endl;
            cout << endl;
        }
    }
    
    return yourUnitsOrders;
}







