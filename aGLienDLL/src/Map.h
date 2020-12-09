#pragma once
#include "Tile.h"
#include <map>
#include <vector>
#include <string>

class Game;
class Texture;
struct Node
{
	
	float y;
	float x;
	int parentX;
	int parentY;
	float gCost;
	float hCost;
	float fCost;
};

inline bool operator < (const Node& lhs, const Node& rhs) {
	return lhs.fCost < rhs.fCost;
}

class __declspec(dllexport) Map {
private:
	int mapSize;
	std::string name;
	const char* path;
	struct coord { 
		int x, y;
		bool operator=(const coord& o) const {
			return x == o.x && y == o.y;
		}
		bool operator<(const coord& o) const{
			return x < o.x || (x == o.x && y < o.y);
		}
	};
	std::map <coord, Tile*> tiles;
	int mapStartX;
	int mapStartY;
	Texture * floorTexture;
	Texture * wallTexture;
	Texture * holeTexture;
	const float nodeSize = 0.2f;

	double calculateH(int x, int y, Node dest);
	bool isDestination(int x, int y, Node dest);
	
	void setTexture(TILETYPE type, Texture* tex);
public:
	Map(const char* mapFile);
	virtual void Init(Game * game);
	Texture* GetTexture(int x, int y);
	Tile * GetTile(int x, int y);
	~Map();
	bool isNodeValid(int x, int y);
	bool isValid(int, int);
	inline int GetStartX() { return mapStartX;}
	inline int GetStartY() { return mapStartY;}
	inline int GetSize() { return mapSize; }
	const char* GetFilePath() { return path; }
	std::string GetName() { return name; }
	Texture * GetTexture(TILETYPE type);
	std::vector<Node> FindPath(Node player, Node dest);
	float GetDistance(Node source, Node dest);
	const inline float GetNodeSize() { return nodeSize; }

};

