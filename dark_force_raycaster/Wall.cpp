#include "Wall.h"


void Wall::initwalltextures(olc::PixelGameEngine* pge)
{
	olc::Sprite* texture = new olc::Sprite("bluestone.png");
	walltextures.push_back(texture);
	olc::Sprite* texture1 = new olc::Sprite("sand1.png");
	walltextures.push_back(texture1);
	olc::Sprite* texture2 = new olc::Sprite("colorstone.png");
	walltextures.push_back(texture2);
	
	olc::Sprite* texture3 = new olc::Sprite("wood.png");
	walltextures.push_back(texture3);

	//will look in to again when doing individual block level;
	//draw one sprite over another in same location 
	//olc::Sprite* tempsprite = new olc::Sprite("stonewall.png");
	//olc::Sprite* texture3 = texture->Duplicate();
	//pge->SetDrawTarget(texture3);
	//pge->SetPixelMode(olc::Pixel::MASK);
	//pge->DrawSprite(0, 0, tempsprite);
	//pge->SetPixelMode(olc::Pixel::NORMAL);
	//pge->SetDrawTarget(nullptr);
	//
	//walltextures.push_back(texture3);
}

void Wall::WallProjection(olc::PixelGameEngine* pge, Player& player, Map& map, Raycast& ray)
{
    int nHalfScreenWidth = pge->ScreenWidth() / 2;
    int nHorizonHeight = pge->ScreenHeight() * player.fPlayerH + (int)player.fLookUp;
    float fAngleStep = player.fPlayer_fov / float(pge->ScreenWidth());

    // iterate over all screen slices, processing the screen in columns
    for (int x = 0; x < pge->ScreenWidth(); x++) {
        float fViewAngle = float(x - nHalfScreenWidth) * fAngleStep;
        float fCurAngle = player.rotationAngle + fViewAngle;

        float fX_hit, fY_hit;   // to hold exact (float) hit location
        int   nX_hit, nY_hit;   // to hold coords of tile that was hit

        int nWallCeil, nWallCeil2, nWallFloor;   // to store the top and bottom y coord of the wall per column

        // this lambda returns a sample of the ceiling through the pixel at screen coord (px, py)
        auto get_ceil_sample = [=](int px, int py) -> olc::Pixel {
            // work out the distance to the location on the ceiling you are looking at through this pixel
            // (the pixel is given since you know the x and y screen coordinate to draw to)
            float fCeilProjDistance = (((1.0f - player.fPlayerH) / float(nHorizonHeight - py)) * map.fDistToProjPlane) /  cos(fViewAngle * PI / 180.0f);
            // calculate the world ceiling coordinate from the player's position, the distance and the view angle + player angle
            float fCeilProjX = player.fPlayerX + fCeilProjDistance * cos(fCurAngle * PI / 180.0f);
            float fCeilProjY = player.fPlayerY + fCeilProjDistance * sin(fCurAngle * PI / 180.0f);
            // calculate the sample coordinates for that world ceiling coordinate, by subtracting the
            // integer part and only keeping the fractional part. Wrap around if the result < 0
            float fSampleX = fCeilProjX - int(fCeilProjX); if (fSampleX < 0.0f) fSampleX += 1.0f;
            float fSampleY = fCeilProjY - int(fCeilProjY); if (fSampleY < 0.0f) fSampleY += 1.0f;
            // having both sample coordinates, get the sample and return it
            return walltextures[1]->Sample(fSampleX, fSampleY);
            };

        // this lambda returns a sample of the floor through the pixel at screen coord (px, py)
        auto get_floor_sample = [=](int px, int py) -> olc::Pixel {
            // work out the distance to the location on the floor you are looking at through this pixel
            // (the pixel is given since you know the x and y to draw to)
            float fFloorProjDistance = ((player.fPlayerH / float(py - nHorizonHeight)) * map.fDistToProjPlane) / cos(fViewAngle * PI / 180.0f);
            // calculate the world floor coordinate from the distance and the view angle + player angle
            float fFloorProjX = player.fPlayerX + fFloorProjDistance * cos(fCurAngle * PI / 180.0f);
            float fFloorProjY = player.fPlayerY + fFloorProjDistance * sin(fCurAngle * PI / 180.0f);
            // calculate the sample coordinates for that world floor coordinate, by subtracting the
            // integer part and only keeping the fractional part. Wrap around if the result < 0
            float fSampleX = fFloorProjX - int(fFloorProjX); if (fSampleX < 0.0f) fSampleX += 1.0f;
            float fSampleY = fFloorProjY - int(fFloorProjY); if (fSampleY < 0.0f) fSampleY += 1.0f;
            // having both sample coordinates, get the sample and return it
            return walltextures[2]->Sample(fSampleX, fSampleY);
            };

        // this lambda returns a sample of the roof through the pixel at screen coord (px, py)
        auto get_roof_sample = [=](int px, int py, int nHeight) -> olc::Pixel {
            // work out the distance to the location on the roof you are looking at through this pixel
            // (the pixel is given since you know the x and y to draw to)
            float fRoofProjDistance = (((player.fPlayerH - float(nHeight)) / float(py - nHorizonHeight)) * map.fDistToProjPlane) / cos(fViewAngle * PI / 180.0f);
            // calculate the world floor coordinate from the distance and the view angle + player angle
            float fRoofProjX = player.fPlayerX + fRoofProjDistance * cos(fCurAngle * PI / 180.0f);
            float fRoofProjY = player.fPlayerY + fRoofProjDistance * sin(fCurAngle * PI / 180.0f);
            // calculate the sample coordinates for that world floor coordinate, by subtracting the
            // integer part and only keeping the fractional part. Wrap around if the result < 0
            float fSampleX = fRoofProjX - int(fRoofProjX); if (fSampleX < 0.0f) fSampleX += 1.0f;
            float fSampleY = fRoofProjY - int(fRoofProjY); if (fSampleY < 0.0f) fSampleY += 1.0f;
            // having both sample coordinates, get the sample and return it
            return walltextures[3]->Sample(fSampleX, fSampleY);
            };

        // prepare the rendering for this screen slice by calculating the list of intersections in this direction
        std::vector<IntersectInfo> vColHitlist;
        float nColHeight = 1.0f;
        float fCurDistance = 0.0f;
        if (ray.CastRay(player, map, fCurAngle, vColHitlist)) {

            // at least one wall / block was hit. Extend the hit list with projected bottom / ceiling info
            for (int i = 0; i < (int)vColHitlist.size(); i++) {
                // make correction for the fish eye effect
                vColHitlist[i].front_Distance *= cos(fViewAngle * PI / 180.0f);
                CalculateWallBottomAndTop(pge, map,player, nHorizonHeight, vColHitlist[i].front_Distance, vColHitlist[i].nHeight, vColHitlist[i].ceil_front, vColHitlist[i].bottom_front);
            }
            // Extend the hit list with projected ceiling info for the back of the wall
            for (int i = 0; i < (int)vColHitlist.size(); i++) {
                if (i == (int)vColHitlist.size() - 1) {
                    vColHitlist[i].ceil_back = vColHitlist[i].ceil_front;
                }
                else {
                    int nDummy;
                    CalculateWallBottomAndTop(pge, map,player, nHorizonHeight, vColHitlist[i + 1].front_Distance, vColHitlist[i].nHeight, vColHitlist[i].ceil_back, nDummy);
                }
            }

            // get the info from first hit point
            fX_hit = vColHitlist[0].fHitX;
            fY_hit = vColHitlist[0].fHitY;
            nX_hit = vColHitlist[0].nMapCoordX;
            nY_hit = vColHitlist[0].nMapCoordY;
            nColHeight = vColHitlist[0].nHeight;
            fCurDistance = vColHitlist[0].front_Distance;

            nWallCeil = vColHitlist[0].ceil_front;
            nWallCeil2 = vColHitlist[0].ceil_back;
            nWallFloor = vColHitlist[0].bottom_front;

        }
        else {
            // no wall was hit - set bottom and top value for wall at the horizon
            nWallCeil = nHorizonHeight;
            nWallCeil2 = nWallCeil;
            nWallFloor = nHorizonHeight;
            fCurDistance = map.fMaxDistance;
        }

        // now render this slice using the info of the hit list
        int nHitListIndex = 0;

        map.fDepthBuffer[x] = fCurDistance;
        // note that we are working upwards
        for (int y = pge->ScreenHeight() - 1; y >= 0; y--) {

            // constants for different types of rendering
#define UNKNOWN_DRAWING 0
#define   FLOOR_DRAWING 1
#define    WALL_DRAWING 2
#define    CEIL_DRAWING 3
#define    ROOF_DRAWING 4
#define RENDER_CEILING !MULTIPLE_LEVELS

                // determine what type of segment is rendered: floor, wall, roof or ceiling
            int nDrawMode = UNKNOWN_DRAWING;
            if (y >= nWallFloor) {
                nDrawMode = (y <= nHorizonHeight) ? CEIL_DRAWING : FLOOR_DRAWING;
            }
            else if (nWallFloor > y && y > nWallCeil) {
                nDrawMode = WALL_DRAWING;
            }
            else if (nWallCeil >= y && y > nWallCeil2) {
                nDrawMode = (nColHeight == 0) ? FLOOR_DRAWING : ROOF_DRAWING;
            }
            else {
                while (nDrawMode == UNKNOWN_DRAWING) {
                    if (nHitListIndex < (int)vColHitlist.size() - 1) {
                        // the y coord is above the current wall and roof slide, but there are still hit points to process
                        // so there could be other walls behind current wall sticking out above it
                        nHitListIndex += 1;

                        // get the info from next hit point
                        fX_hit = vColHitlist[nHitListIndex].fHitX;
                        fY_hit = vColHitlist[nHitListIndex].fHitY;
                        nX_hit = vColHitlist[nHitListIndex].nMapCoordX;
                        nY_hit = vColHitlist[nHitListIndex].nMapCoordY;
                        nColHeight = vColHitlist[nHitListIndex].nHeight;
                        fCurDistance = vColHitlist[nHitListIndex].front_Distance;

                        nWallCeil = vColHitlist[nHitListIndex].ceil_front;
                        nWallCeil2 = vColHitlist[nHitListIndex].ceil_back;
                        nWallFloor = vColHitlist[nHitListIndex].bottom_front;

                        if (y >= nWallFloor) {
                            nDrawMode = (y <= nHorizonHeight) ? CEIL_DRAWING : FLOOR_DRAWING;
                        }
                        else if (nWallFloor > y && y > nWallCeil) {
                            nDrawMode = WALL_DRAWING;
                        }
                        else if (nWallCeil >= y && y > nWallCeil2) {
                            nDrawMode = ROOF_DRAWING;
                        }
                    }
                    else {
                        nDrawMode = (y <= nHorizonHeight) ? CEIL_DRAWING : FLOOR_DRAWING;
                    }
                }
            }

            // now we know what type of segment we're working on, render it
            switch (nDrawMode) {
            case CEIL_DRAWING: {                         // ========== render ceiling ====================
                if (RENDER_CEILING) {
                    olc::Pixel auxSample = get_ceil_sample(x, y);
                    pge->Draw(x, y, auxSample);
                }
                else
                {
                    pge->Draw(x, y, olc::CYAN);
                }
               
            }
                             break;
            case FLOOR_DRAWING: {                        // ========== render floor   ====================
                olc::Pixel auxSample = get_floor_sample(x, y);
                pge->Draw(x, y, auxSample);
            }
                              break;
            case ROOF_DRAWING: {                        // ========== render roof   ====================
                olc::Pixel auxSample = get_roof_sample(x, y, nColHeight);
                pge->Draw(x, y, auxSample);
            }
                             break;
            case WALL_DRAWING: {                         // ========== render wall    ====================

                float fSampleY;
                if (STRETCHED_TEXTURING) {
                    // original sampling = stretched over full height of wall
                    // the y sample coordinate depends only on the pixel y coord on the screen
                    // in relation to the vertical space the wall is taking up
                    fSampleY = float(y - nWallCeil) / float(nWallFloor - nWallCeil);
                }
                else {
                    // sophisticated sampling = sampling per unit block size
                    float fBlockProjHeight = float(nWallFloor - nWallCeil) / float(nColHeight);
                    float fRelativeY = float(y - nWallCeil);
                    while (fRelativeY > fBlockProjHeight)
                        fRelativeY -= fBlockProjHeight;
                    fSampleY = fRelativeY / fBlockProjHeight;
                }

                // the x sample coordinate takes more work to figure out. You need to check what side of the
                // block was hit
                float fSampleX;
                float fBlockMidX = (float)nX_hit + 0.5f;   // location of middle of the cell that was hit
                float fBlockMidY = (float)nY_hit + 0.5f;
                // determine in what quadrant the hit location is wrt the block mid point
                float fTestAngle = atan2f((fY_hit - fBlockMidY), (fX_hit - fBlockMidX));
                if (-0.75f * PI <= fTestAngle && fTestAngle < -0.25f * PI) fSampleX = fX_hit - (float)nX_hit;  // south side
                if (-0.25f * PI <= fTestAngle && fTestAngle < 0.25f * PI) fSampleX = fY_hit - (float)nY_hit;  // east  side
                if (0.25f * PI <= fTestAngle && fTestAngle < 0.75f * PI) fSampleX = fX_hit - (float)nX_hit;  // north side
                if (-0.75f * PI > fTestAngle || fTestAngle >= 0.75f * PI) fSampleX = fY_hit - (float)nY_hit;  // west  side

                // having both sample coordinates, get the sample and draw the pixel
                olc::Pixel auxSample = walltextures[0]->Sample(fSampleX, fSampleY);
                pge->Draw(x, y, auxSample);
            }
                             break;
            }
        }
    }
}

void Wall::CalculateWallBottomAndTop(olc::PixelGameEngine* pge, Map& map,Player& player,float horzheight, float fCorrectedDistToWall, float nWallHeight, int& nWallTop, int& nWallBottom)
{
	
    int nSliceHeight = int((1.0f / fCorrectedDistToWall) * map.fDistToProjPlane);
    nWallTop = horzheight - (nSliceHeight * (1.0f - player.fPlayerH)) - (nWallHeight - 1) * nSliceHeight;
    nWallBottom = horzheight + (nSliceHeight * player.fPlayerH);
}
