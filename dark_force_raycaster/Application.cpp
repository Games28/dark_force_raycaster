#include "Application.h"



void Application::Setup(olc::PixelGameEngine* pge)
{
    wall.initwalltextures(pge);
    map.initMap();
    SM.init();
    map.fDistToProjPlane = ((pge->ScreenWidth() / 2.0f) / std::sin((player.fPlayer_fov / 2.0f) * PI / 180.0f)) * std::cos((player.fPlayer_fov / 2.0f) * PI / 180.0f);
    map.fDepthBuffer = new float[pge->ScreenWidth()];
    init_lu_cos_array();
    init_lu_sin_array();
}

void Application::ProcessInput(olc::PixelGameEngine* pge, float& fElapsedTime)
{
    player.processinput(pge,map,fElapsedTime);

}

void Application::Update(olc::PixelGameEngine* pge, float& fElapsedTime)
{
	
    


	
}

void Application::Render(olc::PixelGameEngine* pge)
{
    wall.WallProjection(pge, player, map, ray);
    SM.Render(pge, player, map);
}
