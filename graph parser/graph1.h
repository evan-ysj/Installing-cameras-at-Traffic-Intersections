#include<vector>
#include<queue>
#include<set>

/*
The "Graph" class is an abstract for an input graph. The main structure is described as follow:
Attributes:
    vert_num: Store the total number of vertices;
    adjacency_list: Store the input graph with a form of adjancy list;
Functions:
    Generate(): Generate the adjacency list with an input vector which stores vertices as pairs;
    ShortestPath(): Compute the shortest path between two given vertices.
*/
template <class T>
class Graph {
    public:
        Graph();
        int vert_num;
        std::vector<std::vector<T>> adjacency_list;
        ~Graph() { adjacency_list.clear(); }
        void Generate(std::vector<std::vector<T>> vertex);
        std::vector<T> ShortestPath(T src, T dst);
};

template <class T>
Graph<T>::Graph() {
    vert_num = 0;
    adjacency_list.resize(1);
}

template <class T>
void Graph<T>::Generate(std::vector<std::vector<T>> vertex) {
    // If the list is not empty, then clear the list to regenerate the graph.
    if(adjacency_list.size() != 0) {
        adjacency_list.clear();
    }
    adjacency_list.resize(vert_num);
    for(int i = 0; i < vertex.size(); ++i) {
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
    if(static_cast<int>(src) < 0 || static_cast<int>(src) >= adjacency_list.size()) {
        std::cerr << "Error: Source point is out of range!\n";
        return path;
    }
    if(static_cast<int>(dst) < 0 || static_cast<int>(dst) >= adjacency_list.size()) {
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
        int size = que.size();
        for(int i = 0; i < size; ++i) {
            T curr = que.front();
            que.pop();
            for(int j = 0; j < adjacency_list[curr].size(); ++j) {
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
    for(int i = 0; i < path.size() / 2; ++i) {
        T tmp = path[i];
        path[i] = path[path.size() - 1 - i];
        path[path.size() - 1 - i] = tmp;
    }
    return path;
}
