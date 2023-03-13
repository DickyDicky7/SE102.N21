#pragma once

#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

class BaseSprites
{

public:

	BaseSprites();
	BaseSprites(int, int, int, int, int, int, int, int, int);

	virtual BaseSprites* SetX(int);
	virtual BaseSprites* SetY(int);
	virtual BaseSprites* SetW(int);
	virtual BaseSprites* SetH(int);
	virtual BaseSprites* SetDx(int);
	virtual BaseSprites* SetDy(int);
	virtual BaseSprites* SetCurFrame(int);
	virtual BaseSprites* SetAnimDelay(int);
	virtual BaseSprites* SetAnimCount(int);

	virtual int GetX() const;
	virtual int GetY() const;
	virtual int GetW() const;
	virtual int GetH() const;
	virtual int GetDx() const;
	virtual int GetDy() const;
	virtual int GetCurFrame() const;
	virtual int GetAnimDelay() const;
	virtual int GetAnimCount() const;

	virtual void Release();
	virtual LPDIRECT3DSURFACE9 GetSurface();
	virtual void BuildSurfaces(LPDIRECT3DDEVICE9) = 0;

protected:

	int x;
	int y;
	int w;
	int h;
	int dx;
	int dy;
	int curFrame;
	int animDelay;
	int animCount;
	std::vector<LPDIRECT3DSURFACE9> surfaces;

};

