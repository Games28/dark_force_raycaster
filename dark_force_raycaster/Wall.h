#ifndef WALL_H
#define WALL_H

#include "olcPixelGameEngine.h"

#include "Player.h"
#include "Raycast.h"
#include "defs.h"
#include <cfloat>







class Wall
{
public:
	Wall() = default;
	void init_trig();
	void wallTextures();
	void changeColorIntensity(olc::Pixel& p, float factor);
	void calculateBottomAndTop(float wallDistance,int halfheight, int wallheight, int& wallceil, int& wallfloor, Player& player);
	void newCalculateBottomAndTop(float wallDistance, int horheight, int wallheight, int& wallTop, int& wallBottom, Player& player);
	int gettexture(int i, int j, int height);
	void renderWallProjection(olc::PixelGameEngine* PGEptr, Player& player, Raycast& rays);

public:
	olc::Sprite* sprites[7];
	int text = 0;
	int time = 0;

	int nTestRay = NUM_RAYS / 2;
	float fTestRay = 0.5f;
	



public:
	//test
///////////////////////////////////////////////////////



#define SIGNIFICANCE 3
#define SIG_POW10 1000


	float deg2rad(float fAngleDeg);
	float rad2deg(float fAngleRad);
	
	float deg_mod2pi(float fAngleDeg);
	
	float rad_mod2pi(float fAngleRad);
	
	float* lu_sin_array = nullptr;
	float* lu_cos_array = nullptr;
	
	void init_lu_sin_array();
	
	
	void init_lu_cos_array();
	
	float lu_sin(float fDegreeAngle);
	
	float lu_cos(float fDegreeAngle);





	/////////////////////////////////////////////////////
};

#endif // !WALL_H


