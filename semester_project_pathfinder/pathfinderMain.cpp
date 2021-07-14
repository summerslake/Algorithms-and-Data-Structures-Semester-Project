#include "pathfinder.h"

int main() {
    
    char maze[15][20] ={{'s','w','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
                        {'e','w','e','e','e','e','e','e','e','b','e','e','e','e','e','e','e','e','e','e'},
                        {'e','w','e','w','w','w','w','w','w','w','w','w','w','w','w','w','e','w','w','w'},
                        {'e','w','e','w','b','e','e','e','e','e','e','e','e','e','e','e','e','w','g','g'},
                        {'e','w','e','w','w','w','w','w','w','w','w','w','w','w','w','w','e','w','g','g'},
                        {'e','w','e','w','e','e','e','e','e','e','e','e','e','e','e','e','e','w','g','g'},
                        {'e','w','e','w','e','w','w','w','w','w','w','w','e','e','e','e','e','w','g','g'},
                        {'e','w','e','w','e','e','e','e','e','e','e','w','e','w','w','w','w','w','w','e'},
                        {'e','w','e','w','e','e','b','e','e','e','w','w','e','w','e','e','e','e','w','e'},
                        {'e','w','e','w','e','g','b','f','e','e','w','e','e','w','e','w','w','e','w','e'},
                        {'e','w','e','w','e','e','b','e','g','g','w','e','e','w','e','e','w','e','w','e'},
                        {'e','w','e','w','e','e','e','e','e','e','w','w','e','w','e','e','w','e','e','e'},
                        {'e','w','e','w','e','e','e','e','e','e','e','w','e','w','e','e','w','e','e','e'},
                        {'e','e','e','w','e','e','e','e','e','e','e','w','e','w','w','w','w','e','e','e'},
                        {'e','w','e','w','e','e','e','e','e','e','w','e','e','e','e','e','e','e','e','e'},};
    
    cout << printArray(maze) << "\n\n";
    
    
    
    vector<char> path;
    path = pathfinder(maze);
    
    /*
    cout << "Moves is: ";
    for(int i=0; i<path.size(); i++) {
        cout << path.at(i) << ",";
    }
    cout << endl;
    */
    
    cout <<printArray(maze) << endl << endl;
}