#ifndef PLAYER_H
#define PLAYER_H
#include "olcPixelGameEngine.h"
#include "Map.h"
#include "defs.h"

class Player
{
public:
	Player();
	~Player();
	void processinput(olc::PixelGameEngine* pge, Map& map, float deltatime);



public:
	float fPlayerX;
	float fPlayerY;
	float rotationAngle;
	float fPlayerH;
	float fPlayer_fov;
	float fLookUp = 0.0f;
	float fSpeedUp;

};
#endif // !PLAYER_H

