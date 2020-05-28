# Installing Cameras at Traffic Intersections

> The project is to help the local police department with their installation of security cameras at traffic intersections. It solves a particular kind of optimization problem, called the Vertex Cover problem, in this context. The idea is for the police to be able to minimize the number of cameras they need to install, and still be as effective as possible with their monitoring. 

## Basic Idea ##
The project implements three ways to solve the problem with multithread:
1. Pick a vertex of highest degree (most incident edges). Add it to the vertex cover and throw away all edges incident on that vertex. Repeat till no edges remain. We will call this algorithm APPROX-VC-1. 
2. Pick an edge ⟨u,v⟩, and add both u and v to the vertex cover. Throw away all edges attached to u and v. Repeat till no edges remain. We will call this algorithm APPROX-VC-2. 
3. Create a polynomial reduction of the decision version of VERTEX COVER to CNF-SAT. Implement the reduction and use minisat as a library to solve the minimum VERTEX COVER problem for the graphs. We will call this algorithm CNF-SAT-VC. 

By doing so, we can analyze how efficient each approach is, for various inputs. An input is characterized by the number of vertices. “Efficient” is characterized in one of two ways: (1) running time, and (2) approximation ratio. We characterize the approximation ratio as the ratio of the size of the computed vertex cover to the size of an optimal (minimum-sized) vertex cover. For measuring the running time, we use `pthread_getcpuclockid()`. For measuring the approximation ratio, we compare it to the output of CNF-SAT-VC, which is guaranteed to be optimal. The program records all the results and exports them to a `.csv` file. 

## Compile ##
The program is compiled by CMake with a `CMakeLists.txt` file. The build sequence is as follow:
```bash
cd ROOT_DIR
mkdir build
cd build
cmake ..
```

## Inputs ## 
As input, uses the output of `rgen.cpp` file. That program generates graphs with the same number of edges for a particular number of vertices and edges. The input example is as follow:
```
V5
E {<2,1>,<2,0>,<2,3>,<1,4>,<4,3>}
```

## Output ##
Given a graph as input, The program outputs the vertex cover computed by each approach in sorted order. The output example is as follow:
```
CNF-SAT-VC: 2,4
APPROX-VC-1: 2,4
APPROX-VC-2: 0,2,3,4
```

