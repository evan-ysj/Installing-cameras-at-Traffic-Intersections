// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include "graph3.h"


// This function is used to parse the input start with "V": 
// Return the total vertex numbers;
int ParseInputV(std::vector<std::string> command) {
    if(command.size() == 2) {
        int v_n = atoi(command[1].c_str());
        if(v_n >= 0) {
            return v_n;
        }
        else {
            std::cerr << "Error: Input error!\n";
            return 0;
        }
    }
    else {
       std::cerr << "Error: Input error!\n";
        return 0; 
    }
}

// This function is used to parse the input start with "E": 
// Parse the edges and store vertices into "vertex" as pairs and return;
std::vector<std::vector<int>> ParseInputE(std::vector<std::string> command) {
    std::vector<std::vector<int>> vertex;
    if(command.size() == 2) {
        std::string edge = command[1];
        // try to parse the edges to vertices and store into "vertex"
        try {
            for(unsigned i = 0; i < edge.length(); ++i) {
                if(edge.at(i) == '<') {
                    int n1, n2;
                    unsigned j = i + 1;
                    while(edge.at(j) != ',') j++;
                    n1 = atoi(edge.substr(i+1, j++).c_str());
                    i = j;
                    while(edge.at(j) != '>') j++;
                    n2 = atoi(edge.substr(i, j).c_str());
                    i = j;
                    std::vector<int> pair(2);                       
                    pair[0] = n1;
                    pair[1] = n2;
                    vertex.push_back(pair);
                }
            }
        }catch(std::exception& e) {
            std::cerr << "Error: Parsing input error:" << e.what() << "\n";
        }
    }
    else {
       std::cerr << "Error: Input error!\n";
    }
    return vertex;
}

// This function is used to parse the input start with "s": 
// Return the query source and destination vertices as a vector. 
std::vector<int> ParseInputS(std::vector<std::string> command) {
    std::vector<int> src_dst(2);
    if(command.size() == 3) {
        src_dst[0] = atoi(command[1].c_str());
        src_dst[1] = atoi(command[2].c_str());
    }
    else {
       std::cerr << "Error: Input error!\n"; 
    }
    return src_dst;
}


int main(int argc, char **argv) {
    // Define a Graph object to store data and call the class function to compute shortest path.
    auto graph = new Graph<int>();
    // Continue to get input until seeing "eof".
    while(!std::cin.eof()) {
        std::string line;
        getline(std::cin, line);
        std::istringstream input(line);
        std::vector<std::string> command;
        // Read the input string by stream.
        while (!input.eof()) {
            std::string s;
            input >> s;
            if (input.fail()) {
                break;
            }
            else {
                command.push_back(s);
            }
            if (input.eof()) {
                break;
            }
        }
        // Parse inputs in different situations.
        if(command.size() == 0) {
            continue;
        }
        if(command[0] == "V") {
            graph->vert_num = ParseInputV(command);
            continue;
        }
        if(command[0] == "E") {
            std::vector<std::vector<int>> vertex = ParseInputE(command);
            graph->Generate(vertex);
            graph->MinVertexCover();
            continue;
        }
        if(command[0] == "s") {
            std::vector<int> src_dst = ParseInputS(command);
            int src = src_dst[0];
            int dst = src_dst[1];
            // Get the shortest path and print.
            std::vector<int> res = graph->ShortestPath(src, dst);
            if(res.size() <1 || res[0] != src) {
                continue;
            }
            else if(res.size() == 1) {
                std::cout << res[0] << "-" << res[0] << "\n";
                continue;
            }
            for(unsigned i = 0; i < res.size(); ++i) {
                if(i == res.size() - 1) {
                    std::cout << res[i] << "\n";
                }
                else {
                    std::cout << res[i] << "-";
                }   
            }
            continue;
        }
        std::cerr << "Error: Input error!\n";    
    }
    graph->~Graph();
    return 0;
}
