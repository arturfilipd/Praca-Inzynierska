#pragma once
#include "TileModel.h"

enum TILETYPE { FLOOR, WALL, HOLE };

class __declspec(dllexport) Tile {
protected:
	int x, y;
	bool passable;
	bool blocking;
	
public:
	
	Tile();
	~Tile();
	virtual TileModel* GetVertices();
	inline int GetType() { return type; }	
	inline bool isPassable() { return passable; }
	inline bool isBlocking() { return blocking; }
protected:
	TILETYPE type;
};

class FloorTile : public Tile{
public:
	FloorTile(int x, int y);
	TileModel * GetVertices();
};

class WallTile : public Tile {
public:
	WallTile(int x, int y);
	TileModel* GetVertices();
};

class UnpassableTile : public Tile {
public:
	UnpassableTile(int x, int y);
	TileModel* GetVertices();
};