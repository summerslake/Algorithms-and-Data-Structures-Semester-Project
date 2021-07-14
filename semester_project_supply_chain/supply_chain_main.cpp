#include "logistics.h"

int main() {
    vector<pair<string,double>> start;
    start.push_back(make_pair("reno",12));
    //start.push_back(make_pair("s2",12));
    
    vector<pair<string,double>> end;
    end.push_back(make_pair("sanFrancisco",12));
    //end.push_back(make_pair("e2",12));
    
    unordered_map<int,double> finalCargoAmounts;
    finalCargoAmounts = organizeLogistics(start,end);
    
    cout << "Final amounts\n";
    cout << "-------------\n";
    //Loop through to get final answer
    for(auto n: finalCargoAmounts) {
        cout << "ID " << n.first << ": " << finalCargoAmounts[n.first] << endl;
    }
}