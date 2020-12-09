#include "TileModel.h"
#include <iostream>

TileModel::TileModel(int count, float* vert)
{
	//std::cout << "new TileModel" << std::endl;
	vertices = new float[count*5];
	verticesCount = count;
	for (int i = 0; i < count * 5; i++) {
		vertices[i] = vert[i];		
	}

}

TileModel::~TileModel()
{
	delete vertices;
}

