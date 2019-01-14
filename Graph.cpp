#include "Graph.h"

Graph::Graph() {
}

void Graph::removeEdge(int p1,int p2) {
	removeLines(p1, p2, adj);
}

void Graph::findNext(int point,std::vector<int>& vectors, std::map<int, std::vector<int>>arraytoFind){
	vectors = arraytoFind.at(point);
}

std::vector<std::vector<int>> Graph::findLoop(int currentKey, std::vector<std::vector<int>>& vectorlist, std::map<int, std::vector<int>> checkingList, std::vector<int>& visitedList)
{
	if (std::find(visitedList.begin(), visitedList.end(), currentKey) == visitedList.end()) {
		std::vector<int> adjlist;
		findNext(currentKey, adjlist, checkingList);
		branchVector(currentKey, adjlist, vectorlist, visitedList);
		visitedList.push_back(currentKey);
		for (int i : adjlist) {
			findLoop(i, vectorlist, checkingList, visitedList);
		}
	}
	return std::vector<std::vector<int>>();
}


void Graph::addEdge(int i1 , int i2) {
	if (adj.find(i1) == adj.end()) {
		std::vector<int> pIDlist = {};
		adj.insert(std::pair<int, std::vector<int>>(i1, pIDlist));

	}
	if (adj.find(i2) == adj.end()) {
		std::vector<int> pIDlist = {};
		adj.insert(std::pair<int, std::vector<int>>(i2, pIDlist));
	}
	adj.at(i1).push_back(i2);
	adj.at(i2).push_back(i1);
}



void Graph::changeNoOfVertices(int numberOfVertices) {
	V = numberOfVertices;

}

bool Graph::checkForConnection(int i, int i2) {
	std::vector<int> connected = adj.at(i);;
	if (std::find(connected.begin(), connected.end(), i2) != connected.end())
		return true;
	return false;
}

void Graph::removeLines(int p1, int p2, std::map<int, std::vector<int>>& arraytoRemove)
{
	for (auto &key : arraytoRemove) {
		LOOP:
		if (key.first == p1 || key.first == p2) {
			for (int& key2 : key.second) {
				if ((key2 == p1 || key2 == p2)) {
					key.second.erase(std::remove(key.second.begin(), key.second.end(), key2), key.second.end());
					goto LOOP;
				}

				//	}
			}
		}
		
	}
	loop2:
	for (auto &key : arraytoRemove) {
		if (key.second.size() == 0) {
			arraytoRemove.erase(key.first);
			goto loop2;
		}
	}
}
void Graph::branchVector(int currentKey, std::vector<int>& adjlist, std::vector<std::vector<int>>& vectorlist, std::vector<int>& visitedList) {
	if (vectorlist.empty()) {
		vectorlist.insert(vectorlist.end(), { currentKey });
	}	startLoop:

	std::vector<std::vector<int>> tempvector = vectorlist;
	for (std::vector<int>& v : vectorlist) {
		if (v.back() == currentKey) {
			for (int i : adjlist) {
				if (std::find(visitedList.begin(), visitedList.end(), i) == visitedList.end()) {
					std::vector<int> temp = v;
					temp.push_back(i);
					tempvector.push_back(temp);
				}
			}
		}
	}
	vectorlist = tempvector;

}

std::vector<std::vector<int>> Graph::BFS() {
	
	std::map<int, std::vector<int>> tempadj = adj;
	bool removeFlag = true;
	while (removeFlag) {
		removeFlag = false;
	EXITLOOP:
		for (auto& key : tempadj) {
			if (key.second.size() <= 1) {
				removeLines(key.first, key.second[0], tempadj);

				removeFlag = true;
				goto EXITLOOP;
			}
		}
	}

	
	std::vector<std::vector<int>> vectorlist;
	std::vector<int> visitedlist;
	for (auto& key : tempadj) {
		std::vector<std::vector<int>> tempvectorlist;
		findLoop(key.first, tempvectorlist, tempadj, visitedlist);
		vectorlist.insert(vectorlist.end(), tempvectorlist.begin(), tempvectorlist.end());
		visitedlist.clear();
		for (auto&key2 : tempadj) {
			if (key2.first <= key.first) {
				visitedlist.push_back(key2.first);
			}
		}

	}
	vectorlist.erase(std::remove_if(vectorlist.begin(), vectorlist.end(), [&](std::vector<int> p) {
		if (p.size() < 3) {
			return true;
		}
		return !(checkForConnection(p.front(), p.back()));
	}), vectorlist.end());


	return vectorlist;
}

