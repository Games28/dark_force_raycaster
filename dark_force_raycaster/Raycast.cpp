#include "Raycast.h"

bool Raycast::CastRay(Player& player, Map& map, float rayAngle, std::vector<IntersectInfo>& vHitList)
{
    float fFromX = player.fPlayerX;
    float fFromY = player.fPlayerY;
    // Calculate the "to point" using the player's angle and fMaxDistance
    float fToX = player.fPlayerX + map.fMaxDistance * cos(rayAngle * PI / 180.0f);
    float fToY = player.fPlayerY + map.fMaxDistance * sin(rayAngle * PI / 180.0f);
    // work out normalized direction vector (fDX, fDY)
    float fDX = fToX - fFromX;
    float fDY = fToY - fFromY;
    float fRayLen = sqrt(fDX * fDX + fDY * fDY);
    fDX /= fRayLen;
    fDY /= fRayLen;

    // calculate the scaling factors for the ray increments per unit in x resp y direction
    // this calculation takes division by 0.0f into account
    float fSX = (fDX == 0.0f) ? FLT_MAX : sqrt(1.0f + (fDY / fDX) * (fDY / fDX));
    float fSY = (fDY == 0.0f) ? FLT_MAX : sqrt(1.0f + (fDX / fDY) * (fDX / fDY));

    // work out if line is going right or left resp. down or up
    int nGridStepX = (fDX > 0.0f) ? +1 : -1;
    int nGridStepY = (fDY > 0.0f) ? +1 : -1;

    float fLengthPartialRayX = 0.0f;
    float fLengthPartialRayY = 0.0f;

    int nCurX = int(fFromX);
    int nCurY = int(fFromY);

    // work out the first intersections with the grid
    if (nGridStepX < 0) { // ray is going left - get scaled difference between start point and left cell border
        fLengthPartialRayX = (fFromX - float(nCurX)) * fSX;
    }
    else {              // ray is going right - get scaled difference between right cell border and start point
        fLengthPartialRayX = (float(nCurX + 1.0f) - fFromX) * fSX;
    }
    if (nGridStepY < 0) { // ray is going up - get scaled difference between start point and top cell border
        fLengthPartialRayY = (fFromY - float(nCurY)) * fSY;
    }
    else {              // ray is going down - get scaled difference between bottom cell border and start point
        fLengthPartialRayY = (float(nCurY + 1.0f) - fFromY) * fSY;
    }

    // check whether analysis got out of map boundaries
    bool bOutOfBounds = (nCurX < 0 || nCurX >= map.nMapX ||
        nCurY < 0 || nCurY >= map.nMapY);
    // did analysis reach the destination cell?
    bool bDestCellFound = (nCurX == int(fToX) && nCurY == int(fToY));

    float fDistIfFound = 0.0f;  // accumulates distance of analysed piece of ray
    float   nCurHeight = 0.0f;       // to check on differences in height

    while (!bOutOfBounds && !bDestCellFound && fDistIfFound < map.fMaxDistance) {

        // advance to next map cell, depending on length of partial ray's
        if (fLengthPartialRayX < fLengthPartialRayY) {
            // continue analysis in x direction
            nCurX += nGridStepX;
            fDistIfFound = fLengthPartialRayX;
            fLengthPartialRayX += fSX;
        }
        else {
            // continue analysis in y direction
            nCurY += nGridStepY;
            fDistIfFound = fLengthPartialRayY;
            fLengthPartialRayY += fSY;
        }

        bOutOfBounds = (nCurX < 0 || nCurX >= map.nMapX ||
            nCurY < 0 || nCurY >= map.nMapY);
        if (bOutOfBounds) {
            bDestCellFound = false;

            // If out of bounds, finalize the list with one additional intersection with the map boundary and height 0.
            // (only if the list is not empty!) This additional intersection record is necessary for proper rendering at map boundaries.
            if (nCurHeight != 0 && !vHitList.empty()) {

                nCurHeight = 0;  // since we're out of bounds

                // put the collision info in a new IntersectInfo node and push it up the hit list
                IntersectInfo sInfo;
                sInfo.front_Distance = fDistIfFound;
                sInfo.fHitX = fFromX + fDistIfFound * fDX;
                sInfo.fHitY = fFromY + fDistIfFound * fDY;
                sInfo.nMapCoordX = nCurX;
                sInfo.nMapCoordY = nCurY;
                sInfo.nHeight = nCurHeight;
                vHitList.push_back(sInfo);
            }
        }
        else {
            // check if there's a difference in height found
            bool bHitFound = (map.fMap[nCurY * map.nMapX + nCurX] != nCurHeight);
            bDestCellFound = (nCurX == int(fToX) && nCurY == int(fToY));

            if (bHitFound) {

                // reset current height to new value
                nCurHeight =  map.fMap[nCurY * map.nMapX + nCurX];

                // put the collision info in a new IntersectInfo node and push it up the hit list
                IntersectInfo sInfo;
                sInfo.front_Distance = fDistIfFound;
                sInfo.fHitX = fFromX + fDistIfFound * fDX;
                sInfo.fHitY = fFromY + fDistIfFound * fDY;
                sInfo.nMapCoordX = nCurX;
                sInfo.nMapCoordY = nCurY;
                sInfo.nHeight = nCurHeight;
                vHitList.push_back(sInfo);
            }
        }
    }

    return (vHitList.size() > 0);
}
