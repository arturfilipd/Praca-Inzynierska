#include "Map.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <exception>
#include <functional>
#include "Texture.h"
#include "Game.h"

Map::Map(const char* mapFile){
    
    std::ifstream file;
    file.open(mapFile);
    std::string line;
    path = mapFile;
    //Map name
    getline(file, line);
    name = line;
    //Map size
    getline(file, line);
    mapSize = std::stoi(line);

    //X
    getline(file, line);
    mapStartX = std::stoi(line);

    //Y
    getline(file, line);
    mapStartY = std::stoi(line);

    for (int y = 0; y < mapSize; y++) {
        getline(file, line);
        for (int x = 0; x < mapSize; x++) {
            if (line[x] == '1')
                tiles[{x, y}] = new FloorTile(x,y);
            else if (line[x] == '0')
                tiles[{x, y}] = new WallTile(x, y);
            else
                tiles[{x, y}] = new UnpassableTile(x, y);
        }
    }
    file.close();

    floorTexture = new Texture("res/textures/tex1.png");
    wallTexture = new Texture("res/textures/tex2.png");
    holeTexture = new Texture("res/textures/tex3.png");
    
}

void Map::Init(Game* game){    
    game->GetCamera();
}


Map::~Map() {
	for (int y = 0; y < mapSize; y++)
		for (int x = 0; x < mapSize; x++)
			delete tiles[{x, y}];
}

bool Map::isNodeValid(int x, int y){
    if (x >= 0 && y >= 0 && x < mapSize / nodeSize && y < mapSize / nodeSize) {      
        float fx = x * nodeSize;
        float fy = y * nodeSize;
        return (GetTile(floor(fx), floor(fy))->GetType() == FLOOR);
    }        
    return false;
}

bool Map::isValid(int x, int y){
    if (x >= 0 && y >= 0 && x < mapSize && y < mapSize)
        return (GetTile(x, y)->GetType() == FLOOR);
    return false;	
}

double Map::calculateH(int x, int y, Node dest){
	
	double H = (sqrt((x - dest.x) * (x - dest.x)
			+ (y - dest.y) * (y - dest.y)));
	return H;
	
}

bool Map::isDestination(int x, int y, Node dest){
    if (x == dest.x && y == dest.y) {
        return true;
    }
    return false;
}

void Map::setTexture(TILETYPE type, Texture* tex){
    switch (type){
    case FLOOR: floorTexture = tex;
        break;
    case WALL: wallTexture = tex;
        break;
    case HOLE: holeTexture = tex;
        break;
    default:
        break;
    }
}

std::vector<Node> makePath(Node **map, Node dest) {
    try {
        int x = dest.x;
        int y = dest.y;
        std::stack<Node> path;
        std::vector<Node> usablePath;
        while (!(map[x][y].parentX == x && map[x][y].parentY == y)
            && map[x][y].x != -1 && map[x][y].y != -1){
            path.push(map[x][y]);
            int tempX = map[x][y].parentX;
            int tempY = map[x][y].parentY;
            x = tempX;
            y = tempY;

        }
        path.push(map[x][y]);
        while (!path.empty()) {
            Node top = path.top();
            path.pop();
            usablePath.emplace_back(top);
        }
        return usablePath;
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

Texture* Map::GetTexture(TILETYPE type){
    switch (type)
    {
    case FLOOR: return floorTexture;
        break;
    case WALL: return wallTexture;
        break;
    case HOLE: return  holeTexture;
        break;
    default: return NULL;
        break;
    }
}

std::vector<Node> Map::FindPath(Node player, Node dest){       
    std::vector<Node> empty;
    if (!isNodeValid(dest.x, dest.y)) return empty;
    if (isDestination(player.x, player.y, dest)) return empty;

    int nodesCount = mapSize / nodeSize;   
    bool** closedList = new bool *[nodesCount];
    for (int i = 0; i < nodesCount; i++)
        closedList[i] = new bool[nodesCount];

    //Initialize whole map
    Node** allMap = new Node*[nodesCount];
    for (int i = 0; i < nodesCount; i++)
        allMap[i] = new Node[nodesCount];
   
    for (int x = 0; x < nodesCount; x++) {
        for (int y = 0; y < nodesCount; y++) {
            allMap[x][y].fCost = FLT_MAX;
            allMap[x][y].gCost = FLT_MAX;
            allMap[x][y].hCost = FLT_MAX;
            allMap[x][y].parentX = -1;
            allMap[x][y].parentY = -1;
            allMap[x][y].x = x;
            allMap[x][y].y = y;
            closedList[x][y] = false;
        }
    }
    //Initialize our starting list
    int x = player.x;
    int y = player.y;
    allMap[x][y].fCost = 0.0;
    allMap[x][y].gCost = 0.0;
    allMap[x][y].hCost = 0.0;
    allMap[x][y].parentX = x;
    allMap[x][y].parentY = y;
    std::vector<Node> openList;
    openList.emplace_back(allMap[x][y]);
    bool destinationFound = false;
    while (!openList.empty() && openList.size() < nodesCount * nodesCount) {
        Node node;
        do {      
            float temp = FLT_MAX;
            std::vector<Node>::iterator itNode;
            int i = 0;
            for (std::vector<Node>::iterator it = openList.begin(); it != openList.end(); it = next(it)) {
                if ((*it).fCost < temp || temp == FLT_MAX) {
                    temp = (*it).fCost;
                    itNode = it;
                }
            } 
            if (openList.size() == 0) break;
            Node d = *itNode;           
            node = *itNode;
            openList.erase(itNode);           
            
        } while (isNodeValid(node.x, node.y) == false);
        x = node.x;
        y = node.y;
        closedList[x][y] = true;
        //For each neighbour starting from North-West to South-East
        for (int newX = -1; newX <= 1; newX++) {
            for (int newY = -1; newY <= 1; newY++) {
                //if (newX != 0 && newY != 0) continue; //Skipping diagonal
                double gNew, hNew, fNew;
                if (isNodeValid(x + newX, y + newY)) {
                    if (isDestination(x + newX, y + newY, dest)){
                        //Destination found - make path
                        allMap[x + newX][y + newY].parentX = x;
                        allMap[x + newX][y + newY].parentY = y;
                        destinationFound = true;
                        std::vector<Node> path =  makePath(allMap, dest);
                        for (int i = 0; i < nodesCount; i++) {
                            delete allMap[i];
                            delete closedList[i];
                        }
                        delete allMap;
                        delete closedList;                      
                        return path;
                    }
                    else if (closedList[x + newX][y + newY] == false){
                        gNew = node.gCost + 1.0;
                        hNew = calculateH(x + newX, y + newY, dest);
                        fNew = gNew + hNew;
                        // Check if this path is better than the one already present
                        if (allMap[x + newX][y + newY].fCost == FLT_MAX ||
                            allMap[x + newX][y + newY].fCost > fNew)
                        {                           
                            allMap[x + newX][y + newY].fCost = fNew;
                            allMap[x + newX][y + newY].gCost = gNew;
                            allMap[x + newX][y + newY].hCost = hNew;
                            allMap[x + newX][y + newY].parentX = x;
                            allMap[x + newX][y + newY].parentY = y;
                            openList.emplace_back(allMap[x + newX][y + newY]);
                        }
                    }
                }
            }
        } 
        if (openList.size() == 0) {
            break;
        }        
    }
    if (destinationFound == false) { 
        for (int i = 0; i < nodesCount; i++) {
            delete allMap[i];
            delete closedList[i];
        }
        delete allMap;
        delete closedList;        
        return empty;
    }
}

float Map::GetDistance(Node source, Node dest){
    return sqrtf((source.x - dest.x) * (source.x - dest.x) + (source.y - dest.y) * (source.y - dest.y)) * nodeSize;
}

Tile* Map::GetTile(int x, int y) {
	if (x >= mapSize || y >= mapSize) return nullptr;
	return tiles[{x, y}];
}
