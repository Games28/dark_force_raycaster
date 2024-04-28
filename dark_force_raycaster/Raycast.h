#ifndef RAYCAST_H
#define RAYCAST_H

#include <vector>

#include "olcPixelGameEngine.h"
#include "defs.h"
#include "Player.h"
#include "Map.h"


struct intersectInfo
{
	float wallHitX;
	float wallHitY;
	int mapX;
	int mapY;
	float distance;
	float distanceback;
	int height;
	bool wasHitVertical;
	float rayAngle;
	int texture;
	
 	//multilevel flying and crouching
	int bottom_front;
	int bottom_back;
	int ceil_front;
	int ceil_back;

	int facehit = -1;
	int blocktype;
	// for debugging purposes
	bool rayUp, rayDn, rayLt, rayRt;
	
	//direction testing
	
};

struct MapLevel
{
	std::vector<intersectInfo> listinfo;
};
struct Ray {
	std::vector<MapLevel> maplevels;

};



class Raycast
{
public:
	
	Raycast() = default;
	void castAllRays(olc::PixelGameEngine* pge, Player& player, Map& map);
	void castRay(olc::PixelGameEngine* pge, float rayAngle, int stripID, Player& player, Map& map);
	void renderMapRays(olc::PixelGameEngine* PGEptr, Player& player, int testray); // Joseph21 - added testray for debugging

	void DrawDepth(olc::PixelGameEngine* pge, float fDepth, int x, int y, olc::Pixel col);

public:
	Ray rays[NUM_RAYS];
	float* Depthbuffer = new float[WINDOW_WIDTH * WINDOW_HEIGHT];
};

#endif // !RAYCAST_H


