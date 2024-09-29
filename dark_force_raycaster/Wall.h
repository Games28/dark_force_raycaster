#ifndef WALL_H
#define WALL_H
#include "olcPixelGameEngine.h"
//#include "defs.h"
#include "Player.h"
#include "Raycast.h"

class Wall
{
public:
	Wall() = default;
	void initwalltextures(olc::PixelGameEngine * pge);
	void WallProjection(olc::PixelGameEngine* pge, Player& player, Map& map, Raycast& ray);
	void CalculateWallBottomAndTop(olc::PixelGameEngine* pge, Map& map,Player& player,float horzheight, float fCorrectedDistToWall,float nWallHeight, int& nWallTop, int& nWallBottom);
public:
	std::vector<olc::Sprite*> walltextures;
	bool has_ceiling = false;
	bool isreflective = true;
};

#endif // !WALL_H


