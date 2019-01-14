#pragma once
#include "lines.h"
#include <map>
#include <list>
#include <vector>
#include <iostream>
#include "points.h"
#include <utility>
#include <algorithm>

// CLASS IS USED FOR ADDING NODES VERTICES AND EDGES
// AS WELL AS PATHFINDING

class Graph {
private:
	int V;
	std::map<int, std::vector<int>> adj;
public:
	Graph();
	void addEdge(int , int );
	void changeNoOfVertices(int );
	bool checkForConnection(int , int );
	std::vector<int> getADJ(int);
	void removeEdge(int , int );
	void findNext(int,std::vector<int>&, std::map<int, std::vector<int>>);
	std::vector<std::vector<int>> findLoop(int , std::vector<std::vector<int>>&, std::map<int, std::vector<int>>, std::vector<int>&);
	void branchVector(int , std::vector<int>& , std::vector<std::vector<int>>& , std::vector<int>& );
	std::vector<std::vector<int>> BFS();
	void removeLines(int , int , std::map<int, std::vector<int>>& );

};