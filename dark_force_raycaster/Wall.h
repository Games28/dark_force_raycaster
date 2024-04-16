#ifndef WALL_H
#define WALL_H

#include "olcPixelGameEngine.h"

#include "Player.h"
#include "Raycast.h"
#include "defs.h"

typedef struct sDelayedPixel
{
	int x, y;
	float  depth;
	olc::Pixel p;
}DelayedPixel;



class Wall
{
public:
	Wall() = default;
	void wallTextures();
	void changeColorIntensity(olc::Pixel& p, float factor);
	void calculateBottomAndTop(float wallDistance,int halfheight, int wallheight, int& wallceil, int& wallfloor, Player& player);
	int gettexture(int i, int j, int height);
	void renderWallProjection(olc::PixelGameEngine* PGEptr, Player& player, Raycast& rays);

public:
	olc::Sprite sprites[7];
	int text = 0;
	int time = 0;

	int nTestRay = NUM_RAYS / 2;
	float fTestRay = 0.5f;
	std::vector<DelayedPixel> vRenderLater;
};

#endif // !WALL_H


