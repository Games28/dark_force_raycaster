#include "Wall.h"

void Wall::wallTextures()
{
    //std::string sPath = "image/";
	sprites[0] = olc::Sprite( "sand1.png");
	sprites[1] = olc::Sprite("stonewall.png");
	sprites[2] = olc::Sprite("bluestone.png");
	sprites[3] = olc::Sprite( "mossystone.png");
	sprites[4] = olc::Sprite("Tatooinewindow3.png");
	sprites[5] = olc::Sprite("redbrick.png");
	sprites[6] = olc::Sprite("wood.png");

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
	vRenderLater.clear();
	int halfscreenwidth = WINDOW_WIDTH / 2;
	int halfscreenheight = WINDOW_HEIGHT * player.fPlayerH + (int)player.lookupordown;
	float anglestep = 60 / float(WINDOW_WIDTH);

	for (int x = 0; x < NUM_RAYS; x++) {     // iterate over all slices of the screen from left to right

		// work out angle from player perspective belonging to this slice
		float fViewangle = float(x - halfscreenwidth) * anglestep, fdistance;

		int wallTopY, wallBottomY, nWallCeil, nWallCeil2, nWallFloor;
		int colheight, FACE_HIT,text;

		// calculated corrected distance as well as bottom and top of the wall projection - per hitpoint
		for (int i = 0; i < (int)rays.rays[x].listinfo.size(); i++)
		{
			rays.rays[x].listinfo[i].distance *= cos(fViewangle * PI / 180.0f);
			calculateBottomAndTop(rays.rays[x].listinfo[i].distance, halfscreenheight, rays.rays[x].listinfo[i].height, rays.rays[x].listinfo[i].ceil_front, rays.rays[x].listinfo[i].bottom_front, player);
		}

		for (int i = 0; i < (int)rays.rays[x].listinfo.size(); i++)
		{
			if (i == (int)rays.rays[x].listinfo.size() - 1)
			{
				rays.rays[x].listinfo[i].ceil_back = rays.rays[x].listinfo[i].ceil_front;
			}
			else
			{
				int nDummy;
				calculateBottomAndTop(rays.rays[x].listinfo[i + 1].distance, halfscreenheight, rays.rays[x].listinfo[i].height, rays.rays[x].listinfo[i].ceil_back, nDummy, player);
			}
		}

		// if there's at least one hitpoint ...
		if (rays.rays[x].listinfo.size() > 0) {

			// ... set the working variables using the first hitpoint ...
			colheight = rays.rays[x].listinfo[0].height;
			nWallCeil = rays.rays[x].listinfo[0].ceil_front;
			nWallCeil2 = rays.rays[x].listinfo[0].ceil_back;
			nWallFloor = rays.rays[x].listinfo[0].bottom_front;
			fdistance = rays.rays[x].listinfo[0].distance;
			text = rays.rays[x].listinfo[0].texture;
			//FACE_HIT = rays.rays[x].listinfo[0].facehit;
			rays.Depthbuffer[x] = fdistance;
		}
		else {
			// ... if there's no hitpoint, set the working variables to correspond with empty horizon displaying
//            colheight = 0;
			colheight = 1;
			nWallCeil = halfscreenheight;
			nWallCeil2 = halfscreenheight;
			nWallFloor = halfscreenheight;
			rays.Depthbuffer[x] = DIST_TO_PROJ_PLANE;
		}

		
		// code to debug the result of the hit list info
		// prints the hit list info for the slice that is denoted by nTestRay (upon releasing T)

//		float fPlayerH = TILE_SIZE / 2.0f;
		float fFoV = 60.0f;
		float fAngleStep = fFoV / (float)NUM_RAYS;
		float fViewAngle = (float)(x - (NUM_RAYS / 2)) * fAngleStep;
		float fCurAngle = (player.rotationAngle * 180.0f / PI) + fViewAngle;
		float fPlayerX = player.x;
		float fPlayerY = player.y;
		float fCosCurAngle = cos(fCurAngle * PI / 180.0f);
		float fSinCurAngle = sin(fCurAngle * PI / 180.0f);
		float fCosViewAngle = cos(fViewAngle * PI / 180.0f);

#define ROOF_COLOR olc::CYAN

		int hitindex = 0;

		for (int y = WINDOW_HEIGHT - 1; y >= 0; y--)
		{
#define UNKNOWN_DRAW  0
#define FLOOR_DRAW    1
#define WALL_DRAW     2
#define CEIL_DRAW     3
#define ROOF_DRAWING  4

			int drawmode = UNKNOWN_DRAW;
			if (y >= nWallFloor)
			{
				drawmode = (y <= halfscreenheight) ? CEIL_DRAW : FLOOR_DRAW;
			}
			else if (nWallFloor > y && y > nWallCeil)
			{
				drawmode = WALL_DRAW;
			}
			else if (nWallCeil >= y && y > nWallCeil2)
			{
				drawmode = (colheight == 0) ? FLOOR_DRAW : ROOF_DRAWING;
			}
			else
			{
				while (drawmode == UNKNOWN_DRAW)
				{
					if (hitindex < (int)rays.rays[x].listinfo.size() - 1)
					{
						// the y coord is above the current wall and roof slide, but there are still hit points to process
						// so there could be other walls behind current wall sticking out above it
						hitindex += 1;
						colheight = rays.rays[x].listinfo[hitindex].height;
						nWallCeil = rays.rays[x].listinfo[hitindex].ceil_front;
						nWallCeil2 = rays.rays[x].listinfo[hitindex].ceil_back;
						nWallFloor = rays.rays[x].listinfo[hitindex].bottom_front;
						text = rays.rays[x].listinfo[hitindex].texture;
						
						//FACE_HIT = FACE_HIT = rays.rays[x].listinfo[hitindex].facehit;

						if (y >= nWallFloor)
						{
							drawmode = (y <= halfscreenheight) ? CEIL_DRAW : FLOOR_DRAW;
						}
						else if (nWallFloor > y && y > nWallCeil)
						{
							drawmode = WALL_DRAW;
						}
						else if (nWallCeil >= y && y > nWallCeil2)
						{
							drawmode = ROOF_DRAWING;
						}
					}
					else
					{
						//drawmode = CEIL_DRAW;
						drawmode = (y <= halfscreenheight) ? CEIL_DRAW : FLOOR_DRAW;
					}
				}
			}

			switch (drawmode)
			{
			case CEIL_DRAW:
			{
				if (RENDER_CEILING)
				{
					float fFloorProjDistance = (((TILE_SIZE * player.fPlayerH) / (float)(halfscreenheight - y)) * DIST_TO_PROJ_PLANE) / fCosViewAngle;
					float fFloorProjX = fPlayerX + fFloorProjDistance * fCosCurAngle;
					float fFloorProjY = fPlayerY + fFloorProjDistance * fSinCurAngle;
					int nSampleX = (int)(fFloorProjX) % TILE_SIZE;
					int nSampleY = (int)(fFloorProjY) % TILE_SIZE;

					olc::Pixel p = sprites[0].GetPixel(nSampleX, nSampleY);
					//rays.DrawDepth(PGEptr, rays.Depthbuffer[x], x, y, p);
					PGEptr->Draw(x, y, p);
				}
				break;
			}
			case FLOOR_DRAW:
			{
				float fFloorProjDistance = (((TILE_SIZE * player.fPlayerH) / (float)(y - halfscreenheight)) * DIST_TO_PROJ_PLANE) / fCosViewAngle;
				float fFloorProjX = fPlayerX + fFloorProjDistance * fCosCurAngle;
				float fFloorProjY = fPlayerY + fFloorProjDistance * fSinCurAngle;

				while (fFloorProjX < 0.0f) { fFloorProjX += floor(TILE_SIZE); }
				while (fFloorProjY < 0.0f) { fFloorProjY += floor(TILE_SIZE); }
				int nSampleX = (int)(fFloorProjX) % TILE_SIZE;
				int nSampleY = (int)(fFloorProjY) % TILE_SIZE;

				olc::Pixel p = sprites[0].GetPixel(nSampleX, nSampleY);
				//rays.DrawDepth(PGEptr, rays.Depthbuffer[x], x, y,p);
				PGEptr->Draw(x, y, p);
				break;
			}
			case ROOF_DRAWING:
			{
				//olc::Pixel p = ROOF_COLOR;

				float fRoofProjDistance = (((player.fPlayerH - float(colheight)) * TILE_SIZE / float(y - halfscreenheight)) * DIST_TO_PROJ_PLANE) / fCosViewAngle;
				float fRoofProjX = player.x + fRoofProjDistance * fCosCurAngle;
				float fRoofProjY = player.y + fRoofProjDistance * fSinCurAngle;

				int nSampleX = (int)(fRoofProjX) % TILE_SIZE;
				int nSampleY = (int)(fRoofProjY) % TILE_SIZE;
				olc::Pixel p = sprites[3].GetPixel(nSampleX, nSampleY);
				//rays.DrawDepth(PGEptr, rays.Depthbuffer[x], x, y, p);
				PGEptr->Draw(x, y, p);
				break;
			}
			case WALL_DRAW:
			{
				float fSampleY = 0;
				int nDisplayBlockHeight = 0;

				if (STRETCHED_TEXTURING)
				{
					fSampleY = float(y - wallTopY) / float(wallBottomY - wallTopY);
				}
				else {
					float blocksize = float(nWallFloor - nWallCeil) / colheight;

					float relative = float(y - nWallCeil);
					nDisplayBlockHeight = colheight;
					while (relative > blocksize)
					{
						relative -= blocksize;
						nDisplayBlockHeight -= 1;
					}

					fSampleY = relative / blocksize;
				}



				float fSampleX;
				olc::Pixel auxSample;
				int tid = 0;
				int tid2 = 0;
				fSampleY = fSampleY * TILE_SIZE;
				//working
				int newid = 0;
				int blocktype = 0;
				int index2;
				tid = gettexture(rays.rays[x].listinfo[hitindex].mapY, rays.rays[x].listinfo[hitindex].mapX, nDisplayBlockHeight);
				if (tid == 4)
				{
					index2 = gettexture(rays.rays[x].listinfo[2].mapY, rays.rays[x].listinfo[2].mapX, nDisplayBlockHeight);
				}
				

				//if (tid == 4)
				//{
				//	tid2 = gettexture(rays.rays[x].listinfo[hitindex + 1].mapY, rays.rays[x].listinfo[hitindex].mapX, nDisplayBlockHeight);
				//}

				


				//fSampleX = int(fSampleX) % TILE_SIZE;
				

				// having both sample coordinates, get the sample and draw the pixel
				if (tid == 4)
				{
					//delayed texture info
					if (rays.rays[x].listinfo[hitindex].wasHitVertical) {
						fSampleX = (int)rays.rays[x].listinfo[hitindex].wallHitY % TILE_SIZE;

						if (tid == 4 && rays.rays[x].listinfo[hitindex].rayRt)
						{
							newid = 4;
						}

						if (tid == 4 && rays.rays[x].listinfo[hitindex].rayLt)
						{
							newid = 2;
						}


					}
					else
					{
						fSampleX = (int)rays.rays[x].listinfo[hitindex].wallHitX % TILE_SIZE;

						if (tid == 4 && rays.rays[x].listinfo[hitindex].rayUp)
						{
							newid = 3;
						}

						if (tid == 4 && rays.rays[x].listinfo[hitindex].rayDn)
						{
							newid = 1;
						}

					}


					auxSample = sprites[newid].GetPixel(fSampleX, fSampleY);


					DelayedPixel aux = { x,y,rays.Depthbuffer[x],auxSample };
					vRenderLater.push_back(aux);

					//next texture to display
					if (rays.rays[x].listinfo[index2].wasHitVertical) {
						fSampleX = (int)rays.rays[x].listinfo[index2].wallHitY % TILE_SIZE;

						if (rays.rays[x].listinfo[hitindex + 1].rayRt)
						{
							newid = 2;
						}
						
						if (rays.rays[x].listinfo[hitindex + 1].rayLt) 
						{
							newid = 5;
						}


					}
					else
					{
						fSampleX = (int)rays.rays[x].listinfo[index2].wallHitX % TILE_SIZE;

					if (rays.rays[x].listinfo[hitindex + 1].rayUp)
					{
						newid = 3;
					}
					
					if (rays.rays[x].listinfo[hitindex + 1].rayDn)
					{
						newid = 1;
					}
					
					}

					auxSample = sprites[newid].GetPixel(fSampleX, fSampleY);
					PGEptr->Draw(x, y, auxSample);
					
				}
				else
				{
					if (rays.rays[x].listinfo[hitindex].wasHitVertical) {
						fSampleX = (int)rays.rays[x].listinfo[hitindex].wallHitY % TILE_SIZE;

						//if (tid == 4 && rays.rays[x].listinfo[hitindex].rayRt)
						//{
						//	newid = 4;
						//}
						//
						//if (tid == 4 && rays.rays[x].listinfo[hitindex].rayLt)
						//{
						//	newid = 2;
						//}


					}
					else
					{
						fSampleX = (int)rays.rays[x].listinfo[hitindex].wallHitX % TILE_SIZE;

						//if (tid == 4 && rays.rays[x].listinfo[hitindex].rayUp)
						//{
						//	tid = 3;
						//}
						//
						//if (tid == 4 && rays.rays[x].listinfo[hitindex].rayDn)
						//{
						//	tid = 1;
						//}

					}

					auxSample = sprites[tid].GetPixel(fSampleX, fSampleY);
					PGEptr->Draw(x, y, auxSample);
				}
				
				
				
				
			     

				
				//rays.DrawDepth(PGEptr,rays.Depthbuffer[x], x, y, auxSample);
				
				break;
			}
			}

		}
		for (auto& elt : vRenderLater)
		{
			if (elt.p != olc::BLANK)
			{
				//rays.DrawDepth(PGEptr, elt.depth, elt.x, elt.y, elt.p);
				PGEptr->Draw(elt.x, elt.y, elt.p);
			}
		}

	}

	
}

