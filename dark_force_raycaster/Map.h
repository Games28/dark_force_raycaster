#ifndef MAP_H
#define MAP_H
#include "olcPixelGameEngine.h"
#include "defs.h"

class Map
{
public:
	Map() = default;
	void initMap();

public:
	std::string sMap;
	float *fMap;
	
	int nMapX = 32;
	int nMapY = 32;
	float fMaxDistance;
	float fDistToProjPlane;
	float* fDepthBuffer = nullptr;
};
#endif // !MAP_H


