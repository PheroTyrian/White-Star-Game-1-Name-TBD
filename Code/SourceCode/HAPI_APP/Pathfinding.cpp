#include "Pathfinding.h"
#include "Map.h"


Pathfinding::Pathfinding()
{
}


Pathfinding::~Pathfinding()
{
}

bool Pathfinding::isValid(int row, int col)const
{
	//returns true if the row and col are in range
	return (row >= 1) && (row < m_size) &&
		(col >= 1) && (col < m_size);
}

bool Pathfinding::isUnBlocked(Map &map, Pair coord)const
{
	
	if (map.getTile(coord) != nullptr)
		return true;
	else
		return false;
}


bool Pathfinding::isDestination(int row, int col, Pair dest)const
{
	if (row == dest.first && col == dest.second)
		return true;
	else
		return false;
}

double Pathfinding::calculateHeuristicValue(int row, int col, Pair dest)const
{
	//calculated using Euclidean Distance
	return((double)sqrt((row - dest.first)*(row - dest.first) + (col - dest.second) * (col - dest.second)));
}

void Pathfinding::tracePath(const std::vector<std::vector<cell> >& cellDetails, Pair dest)
{

	int row = dest.first;
	int col = dest.second;

	//std::stack <Pair> path;

	while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col))
	{
		m_path.push_back(std::make_pair(row, col));
		int tempRow = cellDetails[row][col].parent_i;
		int tempCol = cellDetails[row][col].parent_j;

		row = tempRow;
		col = tempCol;
	}

	m_path.push_back(std::make_pair(row, col));
	

}

void Pathfinding::aStarSearch(Map &map, Pair src, Pair dest)
{
	
	const std::vector<Tile> grid = *map.getMap();
	m_size = grid.size()/2;
	if (!isValid(dest.first, dest.second))
	{
		std::cout << "Destination is invalid" << std::endl;
		return;
	}

	if (!isUnBlocked(map, src) || !isUnBlocked(map, dest))
	{
		std::cout << "Source or Destination blocked" << std::endl;
		return;
	}

	if (isDestination(src.first, src.second, dest))
	{
		std::cout << "Destination Already reached" << std::endl;
		return;
	}


	//bool closedList[m_size][m_size];
	std::vector < std::vector<bool>> closedList;
	closedList.resize(m_size);
	for (int i = 0; i < closedList.size(); i++)
		closedList[i].resize(m_size);
	



	std::vector<std::vector<cell> > cellDetails;
	cellDetails.resize(m_size);
	for (int i = 0; i < cellDetails.size(); i++)
		cellDetails[i].resize(m_size);

	


	int i, j;

	for (i = 0; i < m_size; i++)
	{
		for (j = 0; j < m_size; j++)
		{
			cellDetails[i][j].f = FLT_MAX;
			cellDetails[i][j].g = FLT_MAX;
			cellDetails[i][j].h = FLT_MAX;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
		}
	}

	i = src.first, j = src.second;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;
	cellDetails[i][j].h = 0.0;
	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;


	//open list contains pair <f,<i,j>>
	//f = h+g
	//i and j are the rows and cols
	std::set<pPair> openList;
	openList.insert(std::make_pair(0.0, std::make_pair(i, j)));

	bool destFound = false;

	while (!openList.empty())
	{

		pPair p = *openList.begin();
		openList.erase(openList.begin());

		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;
		if (isValid(i, j))
		{
			std::vector<Tile*> adjacentCells = map.getAdjacentTiles(Pair(i, j));


			double sucG, sucH, sucF;

			for (int cellIndex = 0; cellIndex < adjacentCells.size(); cellIndex++)
			{
				int x = adjacentCells[cellIndex]->m_tileCoordinate.first;
				int y = adjacentCells[cellIndex]->m_tileCoordinate.second;
				if (isValid(x, y))
				{

					if (isDestination(x, y, dest))
					{
						cellDetails[x][y].parent_i = i;
						cellDetails[x][y].parent_j = j;
						tracePath(cellDetails, dest);
						destFound = true;
						return;

					}
					else if (!closedList[x][y] && isUnBlocked(map, Pair(x, y)))
					{
						sucG = cellDetails[i][j].g + 1.0;
						sucH = calculateHeuristicValue(x, y, dest);
						sucF = sucG + sucH;


						if (cellDetails[x][y].f == FLT_MAX || cellDetails[x][y].f > sucF)
						{
							openList.insert(std::make_pair(sucF, std::make_pair(x, y)));
							cellDetails[x][y].f = sucF;
							cellDetails[x][y].g = sucG;
							cellDetails[x][y].h = sucH;
							cellDetails[x][y].parent_i = i;
							cellDetails[x][y].parent_j = j;
						}

					}
				}
			}

		


		}
		
	}
	if (!destFound)
		std::cout << "Failed to find destination" << std::endl;
}



void Pathfinding::findAvailableTiles(Pair src, Map &map, int depth)
{

	const std::vector<Tile> grid = *map.getMap();
	m_size = grid.size()/2;
	int currentDepth = 0;
	int i;
	int j;
	std::set<Pair> openList;
	openList.insert(src);

	std::vector < std::vector<bool>> closedList;
	closedList.resize(m_size);
	for (int i = 0; i < closedList.size(); i++)
		closedList[i].resize(m_size);

	while (currentDepth < depth)
	{

		Pair p = *openList.begin();
		openList.erase(openList.begin());

		i = p.first;
		j = p.second;
		closedList[i][j] = true;

		if (isValid(i, j))
		{
			std::vector<Tile*> adjacentCells = map.getAdjacentTiles(Pair(i, j));



			for (int cellIndex = 0; cellIndex < adjacentCells.size(); cellIndex++)
			{
				if (adjacentCells[cellIndex] != nullptr)
				{
					int x = adjacentCells[cellIndex]->m_tileCoordinate.first;
					int y = adjacentCells[cellIndex]->m_tileCoordinate.second;

					if (isValid(x, y))
					{

						if (!closedList[x][y] && isUnBlocked(map, Pair(x, y)))
						{

							openList.insert(std::make_pair(x, y));

							m_range.push_back(std::make_pair(x, y));
						}
					}
				}


			}

			++currentDepth;
		}
	}
}



