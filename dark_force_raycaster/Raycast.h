#ifndef RAYCAST_H
#define RAYCAST_H
#include "olcPixelGameEngine.h"
#include "Map.h"
#include "Player.h"
#include "defs.h"

struct IntersectInfo
{
	float fHitX, fHitY;
	float front_Distance;
	int nMapCoordX,
		nMapCoordY;
	float nHeight;
	int nwalltype;

	int bottom_front;
	int ceil_front;
	int ceil_back;
};

class Raycast
{
public:
	Raycast() = default;

public:
	bool CastRay(Player& player, Map& map, float rayAngle,std::vector<IntersectInfo> &vHitList);

};


#endif // !RAYCAST_H


