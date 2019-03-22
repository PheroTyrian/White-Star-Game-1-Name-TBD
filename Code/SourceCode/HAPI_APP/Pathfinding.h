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
	int parent_i, parent_j = 0;
	double f, g, h = 0.0;
};

class Pathfinding
{
public:
	Pathfinding();
	~Pathfinding();
	bool isValid(int row, int col)const;
	bool isUnBlocked(Map &map, Pair coord)const;
	bool isDestination(int row, int col, Pair dest)const;
	double calculateHeuristicValue(int row, int col, Pair dest)const;
	void tracePath(const std::vector<std::vector<cell>>& cellDetails, Pair dest);
	void aStarSearch(Map &map, Pair src, Pair dest);
	void findAvailableTiles(Pair src, Map &map, int depth);
	std::vector<Pair> getPathTrace() { return m_path; };
	std::vector<Pair> getMovementRange() { return m_range; };
private:
	std::vector<Pair> m_path;
	std::vector<Pair> m_range;
	int m_size;
	
};

