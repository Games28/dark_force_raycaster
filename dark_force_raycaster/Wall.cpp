#include "Wall.h"

void Wall::init_trig()
{
	lu_cos_array = new float[360 * SIG_POW10];
	lu_sin_array = new float[360 * SIG_POW10];
	init_lu_cos_array();
	init_lu_sin_array();
}

void Wall::wallTextures()
{
    //std::string sPath = "image/";
	sprites[0] = new olc::Sprite( "sand1.png");
	sprites[1] = new olc::Sprite("stonewall.png");
	sprites[2] = new olc::Sprite("bluestone.png");
	sprites[3] = new olc::Sprite( "mossystone.png");
	sprites[4] = new olc::Sprite("Tatooinewindow3.png");
	sprites[5] = new olc::Sprite("redbrick.png");
	sprites[6] = new olc::Sprite("wood.png");

	std::cout << "All textures loaded." << std::endl;
}

void Wall::changeColorIntensity(olc::Pixel& p, float factor)
{
    // Joseph21 - assumed implementation
    p *= factor;
}

void Wall::calculateBottomAndTop(float wallDistance, int halfheight, int wallheight, int& wallceil, int& wallfloor, Player &player)
{
	int nsliceHeight = ((TILE_SIZE / wallDistance) * DIST_TO_PROJ_PLANE);
	wallceil  = halfheight - (nsliceHeight * (1.0f - player.fPlayerH)) - (wallheight - 1) * nsliceHeight;
	wallfloor = halfheight + (nsliceHeight *  player.fPlayerH );
}



int Wall::gettexture(int i, int j, int height)
{
	int textureid = 0;

	switch (height)
	{
	case 1:
		textureid = texturemapOne[i][j];
		break;
	case 2:
		textureid = texturemapTwo[i][j];
		break;
	case 3:
		textureid = texturemapThree[i][j];
		break;
	case 4:
		textureid = texturemapFour[i][j];
		break;
	}
	return textureid;
}



void Wall::renderWallProjection(olc::PixelGameEngine* PGEptr, Player& player, Raycast& rays)
{
	int halfscreenwidth = WINDOW_WIDTH / 2;
	int halfscreenheight = WINDOW_HEIGHT * player.fPlayerH + (int)player.lookupordown;
	float anglestep = 60 / float(WINDOW_WIDTH);

	
	for (int x = 0; x < NUM_RAYS; x++)
	{
	
			float fViewangle = float(x - halfscreenwidth) * anglestep, fdistance;

			int wallTopY, wallBottomY, nWallCeil, nWallCeil2, nWallFloor;
			int colheight, text, Blocktype, MapindexX, MapindexY;


			float fPlayerH = TILE_SIZE / 2.0f;
			float fFoV = 60.0f;
			float fAngleStep = fFoV / (float)NUM_RAYS;
			float fViewAngle = (float)(x - (NUM_RAYS / 2)) * fAngleStep;
			float fCurAngle = (player.rotationAngle * 180.0f / PI) + fViewAngle;
			float fPlayerX = player.x;
			float fPlayerY = player.y;
			float fCosCurAngle = cos(fCurAngle * PI / 180.0f);
			float fSinCurAngle = sin(fCurAngle * PI / 180.0f);
			float fCosViewAngle = cos(fViewAngle * PI / 180.0f);


			auto get_floor_sample = [=](int px, int py, int textureid)-> olc::Pixel
				{
					float fFloorProjDistance = (((TILE_SIZE * player.fPlayerH) / (float)(py - halfscreenheight)) * DIST_TO_PROJ_PLANE) / fCosViewAngle;
					float fFloorProjX = fPlayerX + fFloorProjDistance * fCosCurAngle;
					float fFloorProjY = fPlayerY + fFloorProjDistance * fSinCurAngle;

					while (fFloorProjX < 0.0f) { fFloorProjX += floor(TILE_SIZE); }
					while (fFloorProjY < 0.0f) { fFloorProjY += floor(TILE_SIZE); }
					int nSampleX = (int)(fFloorProjX) % TILE_SIZE;
					int nSampleY = (int)(fFloorProjY) % TILE_SIZE;

					return sprites[textureid]->GetPixel(nSampleX, nSampleY);
					//rays.DrawDepth(PGEptr, rays.Depthbuffer[x], x, y,p);
					//PGEptr->Draw(px, py, p);

				};

			auto get_Ceiling_sample = [=](int px, int py, int textureid)-> olc::Pixel
				{
					float fFloorProjDistance = (((TILE_SIZE * player.fPlayerH) / (float)(halfscreenheight - py)) * DIST_TO_PROJ_PLANE) / fCosViewAngle;
					float fFloorProjX = fPlayerX + fFloorProjDistance * fCosCurAngle;
					float fFloorProjY = fPlayerY + fFloorProjDistance * fSinCurAngle;
					int nSampleX = (int)(fFloorProjX) % TILE_SIZE;
					int nSampleY = (int)(fFloorProjY) % TILE_SIZE;

					return sprites[textureid]->GetPixel(nSampleX, nSampleY);
					//rays.DrawDepth(PGEptr, rays.Depthbuffer[x], x, y, p);
					//PGEptr->Draw(px, py, p);
				};


			//auto block_side_hit = [=](int rayid, int hitindex,int &textureid)
			//	{
			//		float fSampleX;
			//		if (rays.rays[x].listinfo[hitindex].wasHitVertical) {
			//			fSampleX = (int)rays.rays[x].listinfo[hitindex].wallHitY % TILE_SIZE;
			//			    if (rays.rays[x].listinfo[hitindex].rayRt)
			//				{
			//					textureid = 4;
			//				}
			//				
			//				if (rays.rays[x].listinfo[hitindex].rayLt)
			//				{
			//					textureid = 2;
			//				}
			//
			//
			//
			//		}
			//		else
			//		{
			//			fSampleX = (int)rays.rays[x].listinfo[hitindex].wallHitX % TILE_SIZE;
			//
			//			    if (rays.rays[x].listinfo[hitindex].rayRt)
			//				{
			//					textureid = 4;
			//				}
			//				
			//				if (rays.rays[x].listinfo[hitindex].rayLt)
			//				{
			//					textureid = 2;
			//				}
			//
			//		}
			//
			//		return fSampleX;
			//	};
			//
			//auto block_hit = [=](int rayid, int hitindex)
			//	{
			//		float fSampleX;
			//		if (rays.rays[x].listinfo[hitindex].wasHitVertical) {
			//			fSampleX = (int)rays.rays[x].listinfo[hitindex].wallHitY % TILE_SIZE;
			//			
			//
			//		}
			//		else
			//		{
			//			fSampleX = (int)rays.rays[x].listinfo[hitindex].wallHitX % TILE_SIZE;
			//
			//			
			//		}
			//
			//		return fSampleX;
			//	};
			//
			////rendering floor and ceiling/sky first
			for (int y = WINDOW_HEIGHT - 1; y >= 0; y--)
			{
				if (y < halfscreenheight)
				{
					olc::Pixel skysample = olc::CYAN;
					PGEptr->Draw(x, y, skysample);
				}
				if (y > halfscreenheight)
				{
					olc::Pixel floorsample = get_floor_sample(x, y, 0);
					PGEptr->Draw(x, y, floorsample);
				}
			}

			//// calculated corrected distance as well as bottom and top of the wall projection - per hitpoint
			//for (int i = 0; i < (int)rays.rays[x].listinfo.size(); i++)
			//{
			//	rays.rays[x].listinfo[i].distance *= cos(fViewangle * PI / 180.0f);
			//	calculateBottomAndTop(rays.rays[x].listinfo[i].distance, halfscreenheight, rays.rays[x].listinfo[i].height, rays.rays[x].listinfo[i].ceil_front, rays.rays[x].listinfo[i].bottom_front, player);
			//}
			//
			//for (int i = 0; i < (int)rays.rays[x].listinfo.size(); i++)
			//{
			//	if (i == (int)rays.rays[x].listinfo.size() - 1)
			//	{
			//		rays.rays[x].listinfo[i].distanceback = rays.rays[x].listinfo[i].distance;
			//		rays.rays[x].listinfo[i].ceil_back = rays.rays[x].listinfo[i].ceil_front;
			//		rays.rays[x].listinfo[i].bottom_back = rays.rays[x].listinfo[i].bottom_front;
			//	}
			//	else
			//	{
			//		rays.rays[x].listinfo[i].distanceback = rays.rays[x].listinfo[i + 1].distance;
			//		calculateBottomAndTop(
			//			rays.rays[x].listinfo[i].distanceback,
			//			halfscreenheight,
			//			rays.rays[x].listinfo[i].height,
			//			rays.rays[x].listinfo[i].ceil_back,
			//			rays.rays[x].listinfo[i].bottom_back,
			//			player);
			//	}
			//}
			//
			//for (auto& hitRec : rays.rays[x].listinfo)
			//{
			//	float fSampleY = 0;
			//	int nDisplayBlockHeight = 0;
			//
			//	if (hitRec.height > 0.0f)
			//	{
			//		colheight = hitRec.height;
			//		nWallCeil = hitRec.ceil_front;
			//		nWallCeil2 = hitRec.ceil_back;
			//
			//	}
			//	colheight = rays.rays[x].listinfo[0].height;
			//	nWallCeil = rays.rays[x].listinfo[0].ceil_front;
			//	nWallCeil2 = rays.rays[x].listinfo[0].ceil_back;
			//	nWallFloor = rays.rays[x].listinfo[0].bottom_front;
			//	fdistance = rays.rays[x].listinfo[0].distance;
			//	text = rays.rays[x].listinfo[0].texture;
			//	Blocktype = rays.rays[x].listinfo[0].blocktype;
			//
			//
			//}

		}
	

	
}

float Wall::deg2rad(float fAngleDeg)
{
	return fAngleDeg * (PI / 180.0f);
}

float Wall::rad2deg(float fAngleRad)
{
	return fAngleRad / (PI * 180.0f);
}

float Wall::deg_mod2pi(float fAngleDeg)
{
	while (fAngleDeg < 0.0f) fAngleDeg += 360.0f;
	while (fAngleDeg >= 360.0f) fAngleDeg -= 360.0f;
	return fAngleDeg;
}

float Wall::rad_mod2pi(float fAngleRad)
{
	while (fAngleRad < 0.0f) { fAngleRad += (2.0f * PI); }
	while (fAngleRad >= 2.0f * PI) { fAngleRad -= (2.0f * PI); }

	return fAngleRad;
}

void Wall::init_lu_sin_array()
{
	for (int i = 0; i < 360; i++)
	{
		for (int j = 0; j < SIG_POW10; j++)
		{
			int nIndex = i * SIG_POW10 + j;
			float fArg_deg = float(nIndex) / float(SIG_POW10);
			lu_sin_array[nIndex] = sinf(deg2rad(fArg_deg));
		}
	}
}

void Wall::init_lu_cos_array()
{
	for (int i = 0; i < 360; i++)
	{
		for (int j = 0; j < SIG_POW10; j++)
		{
			int nIndex = i * SIG_POW10 + j;
			float fArg_deg = float(nIndex) / float(SIG_POW10);
			lu_sin_array[nIndex] = cosf(deg2rad(fArg_deg));
		}
	}
}

float Wall::lu_sin(float fDegreeAngle)
{
	fDegreeAngle = deg_mod2pi(fDegreeAngle);
	int nWholeNr = int(fDegreeAngle);
	int nRemainder = int((fDegreeAngle - nWholeNr) * float(SIG_POW10));
	int nIndex = nWholeNr * SIG_POW10 + nRemainder;
	return lu_sin_array[nIndex];
}

float Wall::lu_cos(float fDegreeAngle)
{
	fDegreeAngle = deg_mod2pi(fDegreeAngle);
	int nWholeNr = int(fDegreeAngle);
	int nRemainder = int((fDegreeAngle - nWholeNr) * float(SIG_POW10));
	int nIndex = nWholeNr * SIG_POW10 + nRemainder;
	return lu_cos_array[nIndex];
}


