#ifndef MAP_H
#define MAP_H

#include "defs.h"
#include "MapEditor.h"
#include "olcPixelGameEngine.h"

 struct Block
{
	 int Height;
	 //std::vector<float> Heightmaps;
	// std::vector<int*> texturemaps;
	 std::vector<int> textureid;
	 void getTextureid(int id);
};

class Map
{
public:
	Map() = default;
	void initMap();
	bool mapHasWallAt(    float x, float y );
	bool isInsideMap(     float x, float y );
	bool isOutSideMap(    float x, float y );
    bool isOnMapBoundary( float x, float y );   // Added Joseph21

	void renderMapGrid(olc::PixelGameEngine* PGEptr);

	int getBlockType(int x, int y,int level);

	int getFromHeightMap( int x, int y );
	int getTextureMap(    int x, int y, int Height);
	std::vector<Block> blocks;
	
};

#endif // !MAP_H


