#pragma once

class TileModel {
public:
	float * vertices;
	int verticesCount;
	TileModel(int count, float * vert);
	~TileModel();
};
