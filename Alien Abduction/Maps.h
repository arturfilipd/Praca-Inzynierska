#pragma once
#include "Map.h"

class MapTest : public Map {
public:
	MapTest();
	void Init(Game * game);
};

class MapL1 : public Map {	
public : 
	MapL1();
	void Init(Game* game);
};

class MapL2 : public Map {
public: 
	MapL2();
	void Init(Game* game);
};

class MapL3 : public Map {
public:
	MapL3();
	void Init(Game * game);
};