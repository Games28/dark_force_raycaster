#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "defs.h"

#include "Player.h"
#include "Sprite.h"
#include "Wall.h"
#include "Raycast.h"
#include "Map.h"
#include "MapEditor.h"



class dark_force_raycaster : public olc::PixelGameEngine
{
public:
	dark_force_raycaster()
	{
		sAppName = "raycaster";
	}


public:
	Map map;
	Player player;
	Sprite sprite;
	Wall wall;
	Raycast ray;
 
public:

	bool OnUserCreate() override
	{
		//map.initMap();
		wall.wallTextures();
		sprite.initSpriteinfo();
		sprite.initsprites();
		

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		player.processInput(this, fElapsedTime, map);
		player.movePlayer(fElapsedTime, map);
		Clear(RENDER_CEILING ? olc::BLACK : olc::BLUE);
		ray.castAllRays(this,player, map);

		wall.renderWallProjection(this, player, ray);
		sprite.SpriteProjection(this, ray, player);
		map.renderMapGrid(this);
		player.renderMapPlayer(this);
		ray.renderMapRays(this, player, 0);
		


			DrawString(200, 10, "texture " + std::to_string(ray.rays[NUM_RAYS / 2].listinfo[0].texture));
		
		
		DrawLine(WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT, olc::CYAN);
		
		return true;
	}
};


int main()
{
	dark_force_raycaster game;
	if (game.Construct(WINDOW_WIDTH, WINDOW_HEIGHT, 2, 2))
	{
		game.Start();
	}

}