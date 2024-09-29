#include "Player.h"

Player::Player()
{
	fPlayerX = 3.0f;
	fPlayerY = 3.0f;
	fPlayerH = 0.5f;
	fPlayer_fov = 60.0f;
	rotationAngle = 0.0f;

}

Player::~Player()
{
}

void Player::processinput(olc::PixelGameEngine* pge,Map& map, float deltatime)
{
	if (pge->GetKey(olc::D).bHeld)
	{
		rotationAngle += SPEED_ROTATE * deltatime;
		if (rotationAngle >= 360.0f) { rotationAngle -= 360.0f; }
	}
	if (pge->GetKey(olc::A).bHeld)
	{
		rotationAngle -= SPEED_ROTATE * deltatime;
		if (rotationAngle < 0.0f) { rotationAngle += 360.0f; }
	}

	float fnewX = fPlayerX;
	float fnewY = fPlayerY;

	if (pge->GetKey(olc::W).bHeld)
	{
		fnewX += cos(rotationAngle * PI / 180.0f) * SPEED_MOVE * deltatime;
		fnewY += sin(rotationAngle * PI / 180.0f) * SPEED_MOVE * deltatime;
	}

	if (pge->GetKey(olc::S).bHeld)
	{
		fnewX -= cos(rotationAngle * PI / 180.0f) * SPEED_MOVE * deltatime;
		fnewY -= sin(rotationAngle * PI / 180.0f) * SPEED_MOVE * deltatime;
	}
	if (pge->GetKey(olc::Q).bHeld)
	{
		fnewX += sin(rotationAngle * PI / 180.0f) * SPEED_STRAFE * deltatime;
		fnewY -= cos(rotationAngle * PI / 180.0f) * SPEED_STRAFE * deltatime;
	}

	if (pge->GetKey(olc::E).bHeld)
	{
		fnewX -= sin(rotationAngle * PI / 180.0f) * SPEED_STRAFE * deltatime;
		fnewY += cos(rotationAngle * PI / 180.0f) * SPEED_STRAFE * deltatime;
	}

	if (fnewX >= 0 && fnewX < map.nMapX &&
		fnewY >= 0 && fnewY < map.nMapY &&
		// collision detection criterion - is players height > height of map?
		float(map.fMap[int(fnewY) * map.nMapX + int(fnewX)]) < fPlayerH) {
		fPlayerX = fnewX;
		fPlayerY = fnewY;
	}


	//look up for down
	fSpeedUp = pge->GetKey(olc::SHIFT).bHeld ? 4.0f : 1.0f;
	// looking up or down - collision detection not necessary
	// NOTE - there's no clamping to extreme values (yet)
	if (pge->GetKey(olc::UP).bHeld) { fLookUp += SPEED_LOOKUP * fSpeedUp * deltatime; }
	if (pge->GetKey(olc::DOWN).bHeld) { fLookUp -= SPEED_LOOKUP * fSpeedUp * deltatime; }



	//flying

	float fCacheHorHeight = float(pge->ScreenHeight() * fPlayerH) + fLookUp;
	if (MULTIPLE_LEVELS) {
		// if the player height is adapted, keep horizon height stable by compensating with look up value
		if (pge->GetKey(olc::PGUP).bHeld) {
			fPlayerH += SPEED_STRAFE_UP * fSpeedUp * deltatime;
			fLookUp = fCacheHorHeight - float(pge->ScreenHeight() * fPlayerH);
		}
		if (pge->GetKey(olc::PGDN).bHeld) {
			float fNewHeight = fPlayerH - SPEED_STRAFE_UP * fSpeedUp * deltatime;
			// prevent negative height, and do CD on the height map
			if (fNewHeight > 0.0f && float(map.fMap[int(fPlayerY) * map.nMapX + int(fPlayerX)]) < fNewHeight) {
				fPlayerH = fNewHeight;
				fLookUp = fCacheHorHeight - float(pge->ScreenHeight() * fPlayerH);
			}
		}
	}
	else {
		if (pge->GetKey(olc::PGUP).bHeld) {
			float fNewHeight = fPlayerH + SPEED_STRAFE_UP * fSpeedUp * deltatime;
			if (fNewHeight < 1.0f) {
				fPlayerH = fNewHeight;
				// compensate look up value so that horizon remains stable
				fLookUp = fCacheHorHeight - float(pge->ScreenHeight() * fPlayerH);
			}
		}
		if (pge->GetKey(olc::PGDN).bHeld) {
			float fNewHeight = fPlayerH - SPEED_STRAFE_UP * fSpeedUp * deltatime;
			if (fNewHeight > 0.0f) {
				fPlayerH = fNewHeight;
				// compensate look up value so that horizon remains stable
				fLookUp = fCacheHorHeight - float(pge->ScreenHeight() * fPlayerH);
			}
		}
	}
}
