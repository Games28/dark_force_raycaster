#ifndef SPRITE_H
#define SPRITE_H
#include "olcPixelGameEngine.h"
#include "Player.h"
#include "Map.h"

struct sprite_object
{
	float x, y;
	float scale;
	olc::Sprite* sprite;
	float distance, angle;
};


class SpriteManager
{
public:
	SpriteManager() = default;
	void init();
	void Render(olc::PixelGameEngine* pge,Player& player, Map& map);
	float deg2rad(float fAngleDeg) { return fAngleDeg * PI / 180.0f; }
	float rad2deg(float fAngleRad) { return fAngleRad / PI * 180.0f; }
public:
	std::vector<sprite_object> Sobjects;
	
};

#endif // !SPRITE_H

