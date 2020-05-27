#include<vector>
#include<queue>
#include<set>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"


/*
The "Graph" class is an abstract for an input graph. The main structure is described as follow:
Attributes:
    vert_num: Store the total number of vertices;
    adjacency_list: Store the input graph with a form of adjancy list;
    edges: Store the edge information for computing minimum vertex cover.
Functions:
    Generate(): Generate the adjacency list with an input vector which stores vertices as pairs;
    ShortestPath(): Compute the shortest path between two given vertices.
    MinVertexCover(): Compute the minimum vertex cover for the input graph.
*/
template <class T>
class Graph {
    public:
        Graph();
        int vert_num;
        std::vector<std::vector<T>> adjacency_list;
        std::vector<std::vector<T>> edges;
        ~Graph() { adjacency_list.clear(); }
        void Generate(std::vector<std::vector<T>> vertex);
        std::vector<T> ShortestPath(T src, T dst);
        void MinVertexCover();
};

template <class T>
Graph<T>::Graph() {
    vert_num = 0;
    adjacency_list.resize(1);
    edges.resize(1);
}

template <class T>
void Graph<T>::Generate(std::vector<std::vector<T>> vertex) {
    // If the list is not empty, then clear the list to regenerate the graph.
    if(adjacency_list.size() != 0) {
        adjacency_list.clear();
        edges.clear();
    }
    adjacency_list.resize(vert_num);
    edges = vertex;
    for(unsigned i = 0; i < vertex.size(); ++i) {
        T v1 = vertex[i][0];
        T v2 = vertex[i][1];
        if(v1 >= vert_num || v2 >= vert_num) {
            std::cerr << "Error: Vertex in E doesn't exist in V!\n";
            adjacency_list.clear();
            break;
        }
        adjacency_list[v1].push_back(v2);
        adjacency_list[v2].push_back(v1);
    }
}

template <class T>
std::vector<T> Graph<T>::ShortestPath(T src, T dst) {
    std::vector<T> path;
    if(adjacency_list.size() == 0) {
        std::cerr << "Error: No graph data!\n";
        return path;
    }
    if(static_cast<unsigned>(src) < 0 || static_cast<unsigned>(src) >= adjacency_list.size()) {
        std::cerr << "Error: Source point is out of range!\n";
        return path;
    }
    if(static_cast<unsigned>(dst) < 0 || static_cast<unsigned>(dst) >= adjacency_list.size()) {
        std::cerr << "Error: Destination point is out of range!\n";
        return path;
    }
    // Using BFS to traverse all the vertices.
    std::queue<T> que;
    std::set<T> seen;
    que.push(src);
    seen.insert(src);
    std::vector<T> parent(adjacency_list.size(), -1);
    while(!que.empty()) {
        unsigned size = que.size();
        for(unsigned i = 0; i < size; ++i) {
            T curr = que.front();
            que.pop();
            for(unsigned j = 0; j < adjacency_list[curr].size(); ++j) {
                T child = adjacency_list[curr][j];
                if(seen.find(child) == end(seen)) {
                    que.push(child);
                    seen.insert(child);
                    parent[child] = curr;
                }
            }
        }
    }
    // Back track the parent list to collect vertices on the path.
    T pointer = dst;
    while(pointer != -1) {
        path.push_back(pointer);
        pointer = parent[pointer];
    }
    if(path.size() == 1) {
        if(path[0] != src) {
            std::cerr << "Error: No path exists!\n";
        }    
        return path;
    }
    // Reverse the path to begin from source to destination.
    for(unsigned i = 0; i < path.size() / 2; ++i) {
        T tmp = path[i];
        path[i] = path[path.size() - 1 - i];
        path[path.size() - 1 - i] = tmp;
    }
    return path;
}

template <class T>
void Graph<T>::MinVertexCover() {
    for(int k = 1; k <= vert_num; ++k) {
        Minisat::Solver solver;
        Minisat::Lit prop[vert_num][k];
        Minisat::vec<Minisat::Lit> clause;

        // Initialize n x k atomic propositions.
        for(int i = 0; i < vert_num; ++i) {
            for(int j = 0; j < k; ++j) {
                prop[i][j] = Minisat::mkLit(solver.newVar());
            }
        }
        // At least one vertex is the ith vertex in the vertex cover.
        for(int j = 0; j < k; ++j) {    
            for(int i = 0; i < vert_num; ++i) {
                clause.push(prop[i][j]);
            }
            solver.addClause(clause);
            clause.clear();
        }
        // No one vertex can appear twice in a vertex cover.
        for(int m = 0; m < vert_num; ++m) {
            for(int p = 0; p < k - 1; ++p) {
                for(int q = p + 1; q < k; ++q) {
                    solver.addClause(~prop[m][p], ~prop[m][q]);
                }
            }
        }
        // No more than one vertex appears in the mth position of the vertex cover.
        for(int m = 0; m < k; ++m) {
            for(int p = 0; p < vert_num - 1; ++p) {
                for(int q = p + 1; q < vert_num; ++q) {
                    solver.addClause(~prop[p][m], ~prop[q][m]);
                }
            }
        }
        // Every edge is incident to at least one vertex in the vertex cover.
        for(unsigned e = 0; e < edges.size(); ++e) {
            int i = edges[e][0];
            int j = edges[e][1];
            for(int x = 0; x < k; ++x) {
                clause.push(prop[i][x]);
                clause.push(prop[j][x]);
            }
            solver.addClause(clause);
            clause.clear();
        }
        // If there is a solution then print the result and terminate.
        if(solver.solve()) {
            for(int i = 0; i < vert_num; ++i) {
                for(int j = 0; j < k; ++j) {
                    if(solver.modelValue(prop[i][j]) == Minisat::l_True) {
                        std::cout << i << " ";
                    }
                }
            }
            std::cout << "\n";
            return;
        }
    }
    
}
