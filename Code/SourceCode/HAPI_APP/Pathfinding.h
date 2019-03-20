#pragma once
#include <iostream>
#include <stack>
#include <set>
#include <vector>

class Map;
struct Tile;

typedef std::pair<int, int> Pair;

typedef std::pair<double, std::pair<int, int>> pPair;

struct cell
{
	int parent_i, parent_j;
	double f, g, h;
};

class Pathfinding
{
public:
	Pathfinding();
	~Pathfinding();
	bool isValid(int row, int col);
	bool isUnBlocked(Map &map, Pair coord);
	bool isDestination(int row, int col, Pair dest);
	double calculateHeuristicValue(int row, int col, Pair dest);
	void tracePath(std::vector<std::vector<cell>> cellDetails, Pair dest);
	void aStarSearch(Map &map, Pair src, Pair dest);
	void findAvailableTiles(Pair src, Map &map, int depth);
	std::vector<Pair> m_path;
	std::vector<Pair> m_range;
private:
	int m_size;
	
};

