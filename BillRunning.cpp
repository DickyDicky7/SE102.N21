#include "BillRunning.h"

BillRunning::BillRunning()
{
}
BillRunning::BillRunning(int x, int y, int w, int h, int dx, int dy, int curFrame, int animDelay, int animCount)
	: BaseSprites(x, y, w, h, dx, dy, curFrame, animDelay, animCount)
{
}


void BillRunning::BuildSurfaces(LPDIRECT3DDEVICE9 d3ddev)
{

	RECT rect{};
	rect.left = 7; rect.right = 25;
	rect.top = 67; rect.bottom = 101;

	for (int i = 0; i < 6; i++)
	{
		LPDIRECT3DSURFACE9 surface;

		d3ddev->CreateOffscreenPlainSurface
		(
			100,
			100,
			D3DFMT_X8R8G8B8,
			D3DPOOL_DEFAULT,
			&surface,
			NULL
		);

		D3DXLoadSurfaceFromFile
		(
			surface,
			NULL,
			NULL,
			L"Resources\\Sprites\\MainCharacter2.png",
			&rect,
			D3DX_DEFAULT,
			100,
			NULL
		);

		rect.left += 19;
		rect.right += 19;

		surfaces.push_back(surface);
	}

}
