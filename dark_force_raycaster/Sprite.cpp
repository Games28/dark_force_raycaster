#include "Sprite.h"



void SpriteManager::init()
{
	Sobjects =
	{
		{10.0f, 4.5f,1.0f, new olc::Sprite("r2idletest.png"), -1.0f, 0.0f},
		{14.5f, 5.5f,1.0f, new olc::Sprite("r2idletest.png"), -1.0f, 0.0f},
		{14.5f, 6.5f,1.0f, new olc::Sprite("r2idletest.png"), -1.0f, 0.0f}
	};
}

void SpriteManager::Render(olc::PixelGameEngine* pge, Player& player, Map& map)
{
    float fPlayerFov_rad = deg2rad(player.fPlayer_fov);
	int nHorizonHeight = pge->ScreenHeight() * player.fPlayerH + (int)player.fLookUp;

	for (auto& object : Sobjects)
	{
		float fVecX = object.x - player.fPlayerX;
		float fVecY = object.y - player.fPlayerY;

		object.distance = sqrtf(fVecX * fVecX + fVecY * fVecY);

		float fEyeX = cos(player.rotationAngle * PI / 180.0f);
		float fEyeY = sin(player.rotationAngle * PI / 180.0f);
		float fObjA = atan2f(fVecY, fVecX) - atan2f(fEyeY, fEyeX);

		if (fObjA < -PI) fObjA += 2.0f * PI;
		if (fObjA > PI) fObjA -= 2.0f * PI;

		object.angle = fObjA;
	}

	std::sort(
		Sobjects.begin(),
		Sobjects.end(),
		[](sprite_object& a, sprite_object& b)
		{
			return a.distance > b.distance;
		}
	);

	for (auto& object : Sobjects)
	{
        float fObjDist = object.distance;
        float fObjA = object.angle;
        // determine whether object is in field of view (slightly larger to prevent objects being not rendered at
        // screen boundaries)
        bool bInFOV = fabs(fObjA) < fPlayerFov_rad / 1.6f;

        // render object only when within Field of View, and within visible distance.
        // the check on proximity is to prevent asymptotic errors when this distance becomes very small
        if (bInFOV && fObjDist >= 0.3f && fObjDist < map.fMaxDistance) {

            // determine the difference between standard player height (i.e. 0.5f = standing on the floor)
            // and current player height
            float fCompensatePlayerHeight = player.fPlayerH - 0.5f;
            // get the projected (halve) slice height of this object
            float fObjHlveSliceHeight = float(pge->ScreenHeight() / fObjDist);
            float fObjHlveSliceHeightScld = float((pge->ScreenHeight() * object.scale) / fObjDist);

            // work out where objects floor and ceiling are (in screen space)
            // due to scaling factor, differentiated a normalized (scale = 1.0f) ceiling and a scaled variant
            float fObjCeilingNormalized = float(nHorizonHeight) - fObjHlveSliceHeight;
            float fObjCeilingScaled = float(nHorizonHeight) - fObjHlveSliceHeightScld;
            // and adapt all the scaling into the ceiling value
            float fScalingDifference = fObjCeilingNormalized - fObjCeilingScaled;
            float fObjCeiling = fObjCeilingNormalized - 2 * fScalingDifference;
            float fObjFloor = float(nHorizonHeight) + fObjHlveSliceHeight;

            // compensate object projection heights for elevation of the player
            fObjCeiling += fCompensatePlayerHeight * fObjHlveSliceHeight * 2.0f;
            fObjFloor += fCompensatePlayerHeight * fObjHlveSliceHeight * 2.0f;

            // get height, aspect ratio and width
            float fObjHeight = fObjFloor - fObjCeiling;
            float fObjAR = float(object.sprite->height) / float(object.sprite->width);
            float fObjWidth = fObjHeight / fObjAR;
            // work out where the object is across the screen width
            float fMidOfObj = (0.5f * (fObjA / (fPlayerFov_rad / 2.0f)) + 0.5f) * float(pge->ScreenWidth());

            // render the sprite
            for (float fx = 0.0f; fx < fObjWidth; fx++) {
                // get distance across the screen to render
                int nObjColumn = int(fMidOfObj + fx - (fObjWidth / 2.0f));
                // only render this column if it's on the screen
                if (nObjColumn >= 0 && nObjColumn < pge->ScreenWidth()) {
                    for (float fy = 0.0f; fy < fObjHeight; fy++) {
                        // calculate sample coordinates as a percentage of object width and height
                        float fSampleX = fx / fObjWidth;
                        float fSampleY = fy / fObjHeight;
                        // sample the pixel and draw it
                        olc::Pixel pSample = object.sprite->Sample(fSampleX, fSampleY);
                        if (pSample != olc::MAGENTA && map.fDepthBuffer[nObjColumn] >= fObjDist) {
                            pge->Draw(nObjColumn, fObjCeiling + fy, pSample);
                            map.fDepthBuffer[nObjColumn] = fObjDist;
                        }
                    }
                }
            }
        }
	}
}
