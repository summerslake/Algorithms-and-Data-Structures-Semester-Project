#pragma once

#include <vector>
#include <utility>
#include <unordered_map>
#include <string>
#include <map>
#include <utility>
#include <iostream>
#include <fstream>
#include <set>
#include <queue>

using namespace std;

bool DEBUG = false;

//Set to store all cities
set<string> allCities;

//Map with key startCity,endCity and value capacity,ID
map<pair<string,string>,pair<double,int>> citytoCapandID;

//Map of city to parentCity
map<string,string> parent;

void readEdges(string worldMap) {
    //Open input
    ifstream input(worldMap);
    
    //Variables to scan things in
    int ID;
    string startCity;
    string endCity;
    double capacity;
    double cost;
    
    //Read until the end of the file
    while(!input.eof()) {
        //Take in all input
        input >> ID >> startCity >> endCity >> capacity >> cost;
        
        //Put cities into allCities
        allCities.insert(startCity);
        allCities.insert(endCity);
    
        //Put into citiesToIDandCapacity
        citytoCapandID[make_pair(startCity,endCity)] = make_pair(capacity,ID);
        
        if (DEBUG) {
            //Print what was just added
            cout << startCity << " and " << endCity << " mapped to: ";
            cout << capacity << "," << ID << endl;
        }
    }
    
    if (DEBUG) {
        //Print citytoCapandID for testing also
        for (auto it=citytoCapandID.begin(); it!=citytoCapandID.end(); ++it) {
            cout << it->first.first << "," << it->first.second << " => ";
            cout << it->second.first << "," << it->second.second << endl;
        }
        cout << endl;
    }
}

//Breadth first search that alters the parent map to give a path
//Returns true if path is available, returns false if path is not available
bool checkForPathBFS(string startCity, string endCity) {
    //Reset parents
    parent.clear();
    
    //Map for storing if the city was visited
    map<string,bool> cityVisited;
    
    //Go through all cities and set to false (unvisited)
    for(auto it=allCities.begin(); it!=allCities.end(); it++) {
        cityVisited[*it] = false;
    }
    
    //Make queue for keeping track of which cities need to be visited
    queue<string> searchQueue;
    
    //Add the start city to the queue
    searchQueue.push(startCity);
    cityVisited[startCity] = true;
    parent[startCity] = "source";
    
    //Keep track of which city you are currently at in the BFS
    string currentCity;
    
    //Keep going until all nodes visited possible
    while(!searchQueue.empty()) {
        currentCity = searchQueue.front();
        searchQueue.pop();
        //Go through all cities
        for(auto nextCity=allCities.begin(); nextCity!=allCities.end(); nextCity++) {
            //If not visited and the edge exists and the capacity of the edge is > 0
            if(cityVisited[*nextCity] == false &&
            citytoCapandID.count(make_pair(currentCity,*nextCity)) &&
            citytoCapandID[make_pair(currentCity,*nextCity)].first > 0) {
                //Add the next city to the queue
                searchQueue.push(*nextCity);
                //Mark the next city as visited
                cityVisited[*nextCity] = true;
                //Set the parent of the next city to the current city
                parent[*nextCity] = currentCity;
            }
        }
    }
    
    //If the endCity was found
    if(cityVisited[endCity]) {
        return true;
    }
    return false;
    
}

unordered_map<int,double> organizeLogistics(vector<pair<string,double>> start, vector<pair<string,double>> end) {
    //Read in all edges
    readEdges("worldmap.txt");
    
    //Make unordered_map to keep track of output
    unordered_map<int,double> IDtoCargoAmount;
    
    //Set maxCapacity to an large double
    double maxCapacity = 9999999;
    
    //Vector of usedEdges
    vector<pair<string,string>> usedEdges;
    
    
    //BONUS CODE
    
    //Add a node pointing to each start city with essentially infinite edge capacity
    //This way, they are not the limiting factor when doing multiple start and end locations
    //Set the ID of all these edges to an arbitrary thing so we know not to add them to the final map
    int multipleStartID = 123789;
    //Set the capacity of all these edges to essentially infinite edge capacity
    double multipleStartCapacity = 8888888;
    //Set the names of the cities to something arbitrary
    string multipleStartCity = "multipleStart";
    string multipleEndCity = "multipleEnd";
    
    //Add to allCities
    allCities.insert(multipleStartCity);
    allCities.insert(multipleEndCity);
    
    //Make a pair for easier addition to citiesToIDandCapacity
    pair<double,int> multipleStartIDandCap = make_pair(multipleStartCapacity,multipleStartID);
    
    //Add a start node and point to all starting cities
    //Go through all cities and add edges for start and end
    for(int i=0; i<start.size(); i++) {
        citytoCapandID[make_pair(multipleStartCity,start.at(i).first)] = multipleStartIDandCap;
        citytoCapandID[make_pair(end.at(i).first,multipleEndCity)] = multipleStartIDandCap;
    }
    
    
    //While a path is available from the multipleStartCity to multipleEndCity
    while(checkForPathBFS(multipleStartCity,multipleEndCity)) {
        //Set maxCapacity back to large double
        maxCapacity = 9999999;
        
        //Reset vector of used edges
        usedEdges.clear();
        
        if (DEBUG) {
            cout << "A path of edges is:\n";
        }
        //Get that path and max capacity along that path
        string city = multipleEndCity;
        while(parent[city]!="source") {
            if (DEBUG) {
                //Print edge
                cout << parent[city] << "," << city;
                //Print ID of edge
                cout << " which is ID: " << citytoCapandID[make_pair(parent[city],city)].second << endl;
            }
            
            
            //Add to vector of used edges unless the edges we added for bonus
            usedEdges.push_back(make_pair(parent[city],city));
            
            //Keep track of least capacity between edges
            if(citytoCapandID[make_pair(parent[city],city)].first < maxCapacity) {
                maxCapacity = citytoCapandID[make_pair(parent[city],city)].first;
            }
            
            //Move to next thing in map
            city = parent[city];
            
        }
        
        
        if (DEBUG) {
            cout << endl;
            //Print maximumCapacity
            cout << "Which means maximum throughput is: " << maxCapacity << endl;
        }
        
        if (DEBUG) {
            //Print edges used and amount of cargo added per edge
            cout << "So edge used and amount of cargo is: " << endl;
        }
        for(int i=0; i<usedEdges.size(); i++) {
            if (DEBUG) {
                cout << "ID "<< citytoCapandID[usedEdges.at(i)].second << ": ";
                cout << maxCapacity << " added" << endl;
            }
            
            if(citytoCapandID[usedEdges.at(i)].second != 123789) {
                //Add maxCapacity to all used edges
                IDtoCargoAmount[citytoCapandID[usedEdges.at(i)].second]+=maxCapacity;
            }
            
            //Added if it's not one of the synthetic edges because accessing for printing was causing
            //it to be added to the final map
            if (DEBUG && citytoCapandID[usedEdges.at(i)].second != 123789) {
                //Updated ID amount
                cout << "Current cargo amount of ID " << citytoCapandID[usedEdges.at(i)].second;
                cout << ": " << IDtoCargoAmount[citytoCapandID[usedEdges.at(i)].second] << endl;
            }
        
            //Update citytoCapandID with capacity used by
            //subtracting maxCapacity of that path from capacity available
            citytoCapandID[usedEdges.at(i)].first-=maxCapacity;
            
        }
        
        if (DEBUG) {    
            //Print citytoCapandID for testing also
            cout << "\nUpdated capacities :" << endl;
            for (auto it=citytoCapandID.begin(); it!=citytoCapandID.end(); ++it) {
                cout << it->first.first << "," << it->first.second << " => ";
                cout << it->second.first << "," << it->second.second << endl;
            }
            cout << endl;
        }
    }
    if (DEBUG) {
        cout << "Final amounts\n";
        cout << "-------------\n";
        //Loop through to get final answer
        for(auto n: IDtoCargoAmount) {
            cout << "ID " << n.first << ": " << IDtoCargoAmount[n.first] << endl;
        }
    }
    return IDtoCargoAmount;
    
}

unordered_map<int,double> organizeLogisticsWithCargoAmount(vector<pair<string,double>> start, vector<pair<string,double>> end) {
    
    //Read in all edges
    readEdges("testGraph.txt");
    
    //Make remaining cargo variable
    double remainingCargo = start.at(0).second;
    
    //Make unordered_map to keep track of output
    unordered_map<int,double> IDtoCargoAmount;
    
    //Set maxCapacity to an large double
    double maxCapacity = 9999999;
    //Vector of usedEdges
    vector<pair<string,string>> usedEdges;
    
    //While a path is available
    //while(checkForPath(start.at(0).first,end.at(0).first)) {
    while(checkForPathBFS(start.at(0).first,end.at(0).first)) {
        //Set maxCapacity back to large double
        maxCapacity = 9999999;
        
        //Reset vector of used edges
        usedEdges.clear();
        
        if (DEBUG) {
            cout << "A path of edges is:\n";
        }
        
        //Get that path and max capacity along that path
        string city = end.at(0).first;
        while(parent[city]!="source") {
            if (DEBUG) {
                //Print edge
                cout << parent[city] << "," << city;
                //Print ID of edge
                cout << " which is ID: " << citytoCapandID[make_pair(parent[city],city)].second << endl;
            }
            
            //Add to vector of used edges
            usedEdges.push_back(make_pair(parent[city],city));
            
            //Keep track of least capacity between edges
            if(citytoCapandID[make_pair(parent[city],city)].first < maxCapacity) {
                maxCapacity = citytoCapandID[make_pair(parent[city],city)].first;
            }
            
            //Move to next thing in map
            city = parent[city];
            
        }
        
        
        
        if (DEBUG) {
            cout << endl;
            //Print maximumCapacity
            cout << "Which means maximum throughput is: " << maxCapacity << endl;
        }
        
        //Update maxCapacity if it is greater than remaining cargo
        if(maxCapacity > remainingCargo) {
            if (DEBUG) {
                cout <<"However, since we don't have that amount left, we'll";
                cout << " just send the remaining cargo through: ";
            }
            maxCapacity = remainingCargo;
        }
        
        if (DEBUG) {
            //Print edges used and amount of cargo added per edge
            cout << "So edge used and amount of cargo is: " << endl;
        }
        for(int i=0; i<usedEdges.size(); i++) {
            if (DEBUG) {
                cout << "ID "<< citytoCapandID[usedEdges.at(i)].second << ": ";
                cout << maxCapacity << " added" << endl;
            }
            
            //Add maxCapacity to all used edges
            IDtoCargoAmount[citytoCapandID[usedEdges.at(i)].second]+=maxCapacity;
            
            if (DEBUG) {
            //Updated ID amount
            cout << "Current cargo amount of ID " << citytoCapandID[usedEdges.at(i)].second;
            cout << ": " << IDtoCargoAmount[citytoCapandID[usedEdges.at(i)].second] << endl;
            }
            
            //Update citytoCapandID with capacity used by
            //subtracting maxCapacity of that path from capacity available
            citytoCapandID[usedEdges.at(i)].first-=maxCapacity;
            
        }
        
        if (DEBUG) {    
            //Print citytoCapandID for testing also
            cout << "Updated capacities :" << endl;
            for (auto it=citytoCapandID.begin(); it!=citytoCapandID.end(); ++it) {
                cout << it->first.first << "," << it->first.second << " => ";
                cout << it->second.first << "," << it->second.second << endl;
            }
        }
        
        //Update the remaining cargo
        remainingCargo-=maxCapacity;
        if (DEBUG) {
            cout << endl << "Amount of product remaining: " << remainingCargo << endl << endl;
        }
        
        //Break out of the while loop if there is no cargo remaining
        if(remainingCargo == 0) {
            if (DEBUG) {
                cout << "No cargo remaining so breaking while loop" << endl;
            }
            break;
        }
    }
    
    return IDtoCargoAmount;
    
}










