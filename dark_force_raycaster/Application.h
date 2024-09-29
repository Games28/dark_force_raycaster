#ifndef  APPLICATION_H
#define APPLICATION_H

#include "olcPixelGameEngine.h"


#include "Map.h"
#include "Player.h"
#include "Raycast.h"
#include "Wall.h"
#include "defs.h"
#include "Sprite.h"


class Application
{
public:
	Application() = default;

	void Setup(olc::PixelGameEngine* pge);

	void ProcessInput(olc::PixelGameEngine* pge, float& fElapsedTime);
	
	void Update(olc::PixelGameEngine* pge,float & fElapsedTime);

	void Render(olc::PixelGameEngine* pge);

public:
	Map map;
	Player player;
	Raycast ray;
	Wall wall;
	SpriteManager SM;
};


#endif // ! APPLICATION_H


