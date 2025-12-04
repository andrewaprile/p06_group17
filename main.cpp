#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "Utility.h"
//#include "List.h"
#include "Digraph.h"

using namespace std;

/*
 * Handle command line error
 */
struct CommandLineException
{
   CommandLineException()
   {
      cout << endl;
      cout << "Incorrect command line arguments." << endl;
   }
};

/*
 * Handle exception when accessing a file/folder
 */
struct FileException {
  FileException(const string file) {
    cout << endl << "'" << file << "'" << " could not be accessed or contains corrupted content." << endl;
  }
};

/*
 * Handle exception when the input data is invalid
 */
struct FileContentException {
  FileContentException() {
    cout << "Input data could not be correctly parsed." << endl;
  }
};

int string_to_int(const string& s){
   istringstream ss(s);
   int n;
   ss >> n;
   return n;
}

bool isNumber(const string& str)
{
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return !str.empty();
}

void query(ifstream &in1, ifstream &in2, ofstream &o) {
    // get input graph
    // get number of cities
    string first;
    in1 >> first;
    if (!isNumber(first))
        throw FileContentException();
    int size = string_to_int(first);
   // cout << "Number of cities: " << size << endl;

    // get the names of the cities
    string name;
    // because of the limitation of the adjacency matrix, we have to do the mapping here
    // adjacncy list or linked list based implementation can integrate city information
    // to make things easier, we use int type for Vertex in Digraph 
    map<string, int> cities;    // map each city to a vertex (with an index)
    for (int i = 0; i < size && in1 >> name; i++) {
        cities[name] = i;
        //cout << "City " << i << ": " << name << endl;
    }   


    // read the weights
    int count = 0;
    string weight;
    int** adjTime = new int *[size];
    for(int i = 0; i < size; i++)
        adjTime[i] = new int[size];

    int** adjCost = new int *[size];
    for(int i = 0; i < size; i++)
        adjCost[i] = new int[size];

    string line;
    while (in1 >> weight && count < size * size) {
        //Find the delimiter '|' to separate time and cost
        size_t pos = weight.find('|');
        //split the weight string into time and cost entries
        string timeEntryString = weight.substr(0,pos);
        string costEntryString = weight.substr(pos + 1);
        //make integer versions of the entries
        int timeEntryInt = string_to_int(timeEntryString); 
        int costEntryInt = string_to_int(costEntryString);

        // next line is for debugging output
        //cout << timeEntryString << ", " << costEntryString << endl;

        // Insert Time Entries into Time Adjacency Matrix
        if (timeEntryString == "INF") {  // no connection between two vertices
            adjTime[count / size][count % size] = numeric_limits<int>::max();
        }
        else if (!isNumber(timeEntryString)) {
            cout << "Invalid weight: " << timeEntryString << endl;
            throw FileContentException();
        }
        else {
            adjTime[count / size][count % size] = timeEntryInt;
        }
       
        // Insert Cost Entries into Cost Adjacency Matrix
        if (costEntryString == "INF") {  // no connection between two vertices
            adjCost[count / size][count % size] = numeric_limits<int>::max();
        }
        else if (!isNumber(costEntryString)) {
            cout << "Invalid weight: " << costEntryString << endl;
            throw FileContentException();
        }
        else {
            adjCost[count / size][count % size] = costEntryInt;
        }
        count++;
    }
    cout << endl;
    
    //Uncomment to see adjacency matrices for debugging
    /*
     cout << "Adjacency Matrix for Time" << endl;
        for (int i=0; i<size; i++) {
            for (int j=0; j<size; j++) {
                if (adjTime[i][j] == numeric_limits<int>::max()) {
                    cout << "inf" << " ";
                }
                else {
                    cout << adjTime[i][j] << " ";
                }
                cout << "\t";
            }
            cout << endl;
        }
    cout << endl;
    cout << "Adjacency Matrix for Cost" << endl;
        for (int i=0; i<size; i++) {
            for (int j=0; j<size; j++) {
                if (adjCost[i][j] == numeric_limits<int>::max()) {
                    cout << "inf" << " ";
                }
                else{
                    cout << adjCost[i][j] << " ";
                }
                cout << "\t";
            }
            cout << endl;
        }
    cout << endl;
    */


    if (count != size * size)
        throw FileContentException();
    
    // Make two separate digraphs;  one for time and one for cost
    Digraph<int> graphTime(adjTime, size);
    Digraph<int> graphCost(adjCost, size);

    // get queries
    // one query per line
    for (string line; getline(in2, line);) {
        istringstream ss(line);
        string src, dst, mode;
        ss >> src;
        ss >> dst;
        ss >> mode;
        // next line is for debugging output
        if (mode == "T"){
            cout << src << " to " << dst << " by " << mode << "ime: ";
        }
        else if(mode == "C"){
            cout << src << " to " << dst << " by " << mode << "ost: ";
        }
        // cout << src << " to " << dst << " by " << mode << endl;

        if (cities.find(src) == cities.end() || cities.find(dst) == cities.end()) {
            cout << "Invalid query: the queried city is not in the map." << endl;
        }
        else {
            // use if statement to choose to use either time or cost graph
            if (mode == "T"){
                int result = graphTime.get_shortest_path(cities[src], cities[dst]);
                if (result == numeric_limits<int>::max()) {
                    o << "No available path" << endl;
                    cout << "No available path" << endl;
                }
                else {
                    o << result << endl;
                    if (mode == "T"){
                        cout << result << " minutes" << endl;
                    }
                    else{
                        cout << "$" << result << endl;
                    }
                }
            } 
            else if (mode == "C"){
                int result = graphCost.get_shortest_path(cities[src], cities[dst]);
                if (result == numeric_limits<int>::max()) {
                    o << "No available path" << endl;
                    cout << "No available path" << endl;
                }
                else {
                    o << result << endl;
                    if (mode == "T"){
                        cout << result << " minutes" << endl;
                    }
                    else{
                        cout << "$" << result << endl;
                    }                    
                }
            }
        }
    }

    // clean up dynamic memory
    for (int i = 0; i < size; i++) 
        delete[] adjTime[i];
    for (int i = 0; i < size; i++)
        delete[] adjCost[i];
    delete[] adjTime;
    delete[] adjCost;
}

int main(int argc, char* argv[])
{
    try {
    // handle command line input
    if (argc != 4) {
        throw CommandLineException();
    }

    ifstream input_file1, input_file2;
    ofstream output_file;
    input_file1.open(argv[1]);  // adjacency matrix for the graph
    input_file2.open(argv[2]);  // queries
    output_file.open(argv[3]);  // query results
    if (!input_file1) {
        throw FileException(argv[1]);
    }
    if (!input_file2) {
        throw FileException(argv[2]);
    }
    if (!output_file) {
        throw FileException(argv[3]);
    }

    query(input_file1, input_file2, output_file);

    // clean up
    input_file1.close();
    input_file2.close();
    output_file.close();
  }
  catch(...)
  {
      cout << "Program ended." << endl;
      exit(EXIT_FAILURE);
  }
  return 0;
}
