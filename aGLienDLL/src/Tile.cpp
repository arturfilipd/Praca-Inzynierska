#include "Tile.h"
#include <iostream>



Tile::Tile() {}

Tile::~Tile() {}


TileModel* Tile::GetVertices() {
    return nullptr;
}

FloorTile::FloorTile(int x, int y)
{
    this->x = x;
    this->y = y;
    passable = true;
    blocking = false;
    type = FLOOR;
}

TileModel* FloorTile::GetVertices()
{

    float a[] = {
        -0.5f + x, 0.f,  0.5f + y,  0.0f, 0.0f, //0
         0.5f + x, 0.f,   0.5f + y,  1.0f, 0.0f, //1
         0.5f + x, 0.f,  -0.5f + y,  1.0f, 1.0f, //2
        -0.5f + x, 0.f, -0.5f + y,  0.0f, 1.0f, //3
    };
    return new TileModel(4, a);
}

WallTile::WallTile(int x, int y)
{
    this->x = x;
    this->y = y;
    passable = false;
    blocking = true;
    type = WALL;
}

TileModel* WallTile::GetVertices()
{
    float wallH = 0.5f;
    float wall[] = {

        // Lower square
        -0.50f + x, 0.f,   0.50f + y, 0.00f, 0.0f, //0
         0.50f + x, 0.f,   0.50f + y, 0.25f, 0.0f, //1
         0.50f + x, 0.f,  -0.50f + y, 0.50f, 0.0f,//2
        -0.50f + x, 0.f,  -0.50f + y, 0.75f, 0.0f,//3                            
        // Upper square
        -0.50f + x, wallH,   0.50f + y, 0.00f, 1.0f,//4
         0.50f + x, wallH,   0.50f + y, 0.25f, 1.0f,//5
         0.50f + x, wallH,  -0.50f + y, 0.50f, 1.0f,//6
        -0.50f + x, wallH,  -0.50f + y, 0.75f, 1.0f,//7

        -0.50f + x, 0.f,   0.50f + y, 1.00f, 0.0f, //0A
        -0.50f + x, wallH,   0.50f + y, 1.00f, 1.0f,//4A
    };
    return new TileModel(10, wall);
}

UnpassableTile::UnpassableTile(int x, int y)
{
    this->x = x;
    this->y = y;
    passable = false;
    blocking = false;
    type = HOLE;
}

TileModel* UnpassableTile::GetVertices()
{
    float a[] = {
        -0.5f + x, 0.f,  0.5f + y,  0.0f, 0.0f, //0
         0.5f + x, 0.f,   0.5f + y,  1.0f, 0.0f, //1
         0.5f + x, 0.f,  -0.5f + y,  1.0f, 1.0f, //2
        -0.5f + x, 0.f, -0.5f + y,  0.0f, 1.0f, //3
    };
    return new TileModel(4, a);
}