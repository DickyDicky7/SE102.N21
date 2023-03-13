#include "BaseSprites.h"

BaseSprites::BaseSprites()
{
}
BaseSprites::BaseSprites(int x, int y, int w, int h, int dx, int dy, int curFrame, int animDelay, int animCount)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->dx = dx;
	this->dy = dy;
	this->curFrame = curFrame;
	this->animDelay = animDelay;
	this->animCount = animCount;
}

BaseSprites* BaseSprites::SetX(int x) { this->x = x; return this; }
BaseSprites* BaseSprites::SetY(int y) { this->y = y; return this; }
BaseSprites* BaseSprites::SetW(int w) { this->w = w; return this; }
BaseSprites* BaseSprites::SetH(int h) { this->h = h; return this; }
BaseSprites* BaseSprites::SetDx(int dx) { this->dx = dx; return this; }
BaseSprites* BaseSprites::SetDy(int dy) { this->dy = dy; return this; }
BaseSprites* BaseSprites::SetCurFrame(int curFrame) { this->curFrame = curFrame; return this; }
BaseSprites* BaseSprites::SetAnimDelay(int animDelay) { this->animDelay = animDelay; return this; }
BaseSprites* BaseSprites::SetAnimCount(int animCount) { this->animCount = animCount; return this; }

int BaseSprites::GetX() const { return x; }
int BaseSprites::GetY() const { return y; }
int BaseSprites::GetW() const { return w; }
int BaseSprites::GetH() const { return h; }
int BaseSprites::GetDx() const { return dx; }
int BaseSprites::GetDy() const { return dy; }
int BaseSprites::GetCurFrame() const { return curFrame; }
int BaseSprites::GetAnimDelay() const { return animDelay; }
int BaseSprites::GetAnimCount() const { return animCount; }

void BaseSprites::Release()
{
	for (LPDIRECT3DSURFACE9 surface : surfaces) surface->Release();
	surfaces.clear();
}

LPDIRECT3DSURFACE9 BaseSprites::GetSurface()
{
	LPDIRECT3DSURFACE9 surface = surfaces[curFrame];
	++curFrame %= surfaces.size();
	return surface;
}
