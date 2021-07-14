#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <map>
#include <utility>
#include <queue>

using namespace std;

bool DEBUG = true;

map<pair<int,int>,pair<int,int>> parent;
//MOVE finish BACK DOWN TO BFS WHEN DONE TESTING********************************************************
pair<int,int> finish;

//Checks to prevent out of bounds checking, returns (-1,-1) pair if direction would be out of bounds
template<std::size_t rows, std::size_t cols>
pair<int,int> checkLeft(pair<int,int> currentIndex,char (&maze)[rows][cols]) {
    //Make pair to return
    pair<int,int> leftIndex;
    
    //If on left wall
    if(currentIndex.second == 0) {
        leftIndex = make_pair(-1,-1);
    }
    else {
        leftIndex = make_pair(currentIndex.first,currentIndex.second-1);
    }
    return leftIndex;
}

template<std::size_t rows, std::size_t cols>
pair<int,int> checkRight(pair<int,int> currentIndex,char (&maze)[rows][cols]) {
    //Make pair to return
    pair<int,int> rightIndex;
    
    //If on right wall
    if(currentIndex.second == cols-1) {
        rightIndex = make_pair(-1,-1);
    }
    else {
        rightIndex = make_pair(currentIndex.first,currentIndex.second+1);
    }
    return rightIndex;
}

template<std::size_t rows, std::size_t cols>
pair<int,int> checkDown(pair<int,int> currentIndex,char (&maze)[rows][cols]) {
    //Make pair to return
    pair<int,int> downIndex;
    
    //If on bottom wall
    if(currentIndex.first == rows-1) {
        downIndex = make_pair(-1,-1);
    }
    else {
        downIndex = make_pair(currentIndex.first+1,currentIndex.second);
    }
    return downIndex;
}

template<std::size_t rows, std::size_t cols>
pair<int,int> checkUp(pair<int,int> currentIndex,char (&maze)[rows][cols]) {
    //Make pair to return
    pair<int,int> upIndex;
    
    //If on left wall
    if(currentIndex.first == 0) {
        upIndex = make_pair(-1,-1);
    }
    else {
        upIndex = make_pair(currentIndex.first-1,currentIndex.second);
    }
    return upIndex;
}


//Pass in boolean failed: if false: set bombs to visited and avoid if true: set bombs to unvisited and ignore bombs 
template<std::size_t rows, std::size_t cols>
bool BFS(char (&maze)[rows][cols], bool failed) {
    //Clear parents map
    parent.clear();
    
    //Make var to store start index
    pair<int,int> start;
    //Make var to store finish index
    
    //Make map to store visited
    map<pair<int,int>,bool> visited;
    
    //Go through whole 2D array
    for(int i=0; i<rows; i++) {
        for(int j=0; j<cols; j++) {
            //Mark walls as visited
            if(maze[i][j] == 'w') {
                visited[make_pair(i,j)] = true;
            }
            //Mark empties as unvisited
            else if(maze[i][j] == 'e') {
                visited[make_pair(i,j)] = false;
            }
            //Mark start as start and unvisited
            else if(maze[i][j] == 's') {
                start = make_pair(i,j);
                visited[start] = false;
            }
            //Mark finish as finish and unvisited
            else if(maze[i][j] == 'f') {
                finish = make_pair(i,j);
                visited[finish] = false;
            }
            //Mark everything else as unvisited (including gold and bombs)
            else {
                //If the first fun failed
                if(failed) {
                    //Set bombs as unvisited so it'll treat it like empty
                    visited[make_pair(i,j)] = false;
                }
                else {
                    //Set bombs as visited so it'll avoid them
                    visited[make_pair(i,j)] = true;
                }
            }
        }
    } 
    
    //make queue for BFS
    queue<pair<int,int>> searchQueue;
    
    //Push start onto queue
    searchQueue.push(start);
    //Mark start as visited
    visited[start] = true;
    //Make parent of start <999,999>
    parent[start] = make_pair(999,999);
    
    //Var to keep track of current index
    pair<int,int> currentIndex;
    pair<int,int> nextIndex;
    
    
    if(DEBUG) {
        cout << "start is: " << start.first << "," << start.second << endl;
        cout << "finish is: " << finish.first << "," << finish.second << endl;
    }
    
    while(!searchQueue.empty()) {
        //Set current index to front of queue
        currentIndex = searchQueue.front();
        //Pop value off front of queue
        searchQueue.pop();
        
        //Check all 4 directions with functions
        //Check left
        nextIndex = checkLeft(currentIndex,maze);
        if(nextIndex != make_pair(-1,-1) && visited[nextIndex] == false ) {
            searchQueue.push(nextIndex);
            visited[nextIndex] = true;
            parent[nextIndex] = currentIndex;
        }
        //Check right
        nextIndex = checkRight(currentIndex,maze);
        //cout << "Check Right returned " << nextIndex.first << "," << nextIndex.second << endl;
        if(nextIndex != make_pair(-1,-1) && visited[nextIndex] == false ) {
            searchQueue.push(nextIndex);
            visited[nextIndex] = true;
            parent[nextIndex] = currentIndex;
        }
        //Check up
        nextIndex = checkUp(currentIndex,maze);
        //cout << "Check up returned " << nextIndex.first << "," << nextIndex.second << endl;
        if(nextIndex != make_pair(-1,-1) && visited[nextIndex] == false ) {
            searchQueue.push(nextIndex);
            visited[nextIndex] = true;
            parent[nextIndex] = currentIndex;
        }
        //Check down
        //cout << "Check down returned " << nextIndex.first << "," << nextIndex.second << endl;
        nextIndex = checkDown(currentIndex,maze);
        if(nextIndex != make_pair(-1,-1) && visited[nextIndex] == false ) {
            searchQueue.push(nextIndex);
            visited[nextIndex] = true;
            parent[nextIndex] = currentIndex;
        }
    }
    
    if(visited[finish] == true) {
        return true;
    }
    return false;
}


template<std::size_t rows, std::size_t cols>
vector<char> pathfinder(char (&maze)[rows][cols]) {
    //Vector to return moves
    vector<char> moves;
    
    bool failed = false;
    
    //If the BFS fails
    if(!BFS(maze,failed)) {
        failed = true;
        //Run BFS again taking bombs into account
        //If BFS fails again, return the null character
        if(!BFS(maze,failed)) {
            if (DEBUG) { cout << "Path not found\n";}
            //Add null character
            moves.push_back('\0');
            //Return with just null character
            return moves;
        }
        if (DEBUG) {cout << "Path found on second try" << endl;} 
    }
    
    if (DEBUG) { cout << endl; }
    
    
    
    pair<int,int> index;
    index = make_pair(finish.first,finish.second);
    while(parent[index] != make_pair(999,999)) {
        maze[index.first][index.second] = '#';
        
        //If previous index was above, then action was down
        if(parent[index].first == index.first-1) {
            //Put at beginning of moves vector
            moves.insert(moves.begin(),'D');
        }
        
        //If previous index was below, then action was up
        if(parent[index].first == index.first+1) {
            //Put at beginning of moves vector
            moves.insert(moves.begin(),'U');
        }
        
        //If previous index was left, then action was right
        if(parent[index].second == index.second-1) {
            //Put at beginning of moves vector
            moves.insert(moves.begin(),'R');
        }
        
        //If previous index was right, then action was left
        if(parent[index].second == index.second+1) {
            //Put at beginning of moves vector
            moves.insert(moves.begin(),'L');
        }
        
        //Update the index to keep scrubbing through the parent map
        index = make_pair(parent[index].first,parent[index].second);
        
        
    }
    
    
    if (DEBUG) { 
        cout << "Moves is: ";
        for(int i=0; i<moves.size(); i++) {
            cout << moves.at(i) << ",";
        }
        cout << "\n\n";
    }
    
    return moves;
}

template<std::size_t rows, std::size_t cols>
string printArray(char (&maze)[rows][cols]) {
    string outputString = "";
    
    //Print the array with spaces between
    for(int i=0; i<rows; i++) {
        for(int j=0; j<cols; j++) {
            //Commented out for testing in Visual Studio terminal (colors not same as linux terminal)
            /*
            if(maze[i][j] == 'w') {
                outputString+="\033[1;36mw\033[0m";
            }
            else if(maze[i][j] == 'e') {
                outputString+="\033[1;32me\033[0m";
            }
            else if(maze[i][j] == 'g') {
                outputString+="\033[1;33mg\033[0m";
            }
            else if(maze[i][j] == 'b') {
                outputString+="\033[1;31mb\033[0m";
            }
            else {
                outputString+= maze[i][j];
            }
            */
            outputString+=maze[i][j];
            outputString+= ' ';
        }
        outputString+="\n";
    }
    return outputString;
}