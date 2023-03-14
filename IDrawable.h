#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

class IDrawable
{

public:

	virtual IDrawable* SetWidth(LONG);
	virtual IDrawable* SetHeight(LONG);
	virtual IDrawable* SetTopPosition(LONG);
	virtual IDrawable* SetLeftPosition(LONG);
	virtual IDrawable* SetCurrentSprite(LONG);
	virtual IDrawable* SetNumberOfSprites(LONG);
	virtual IDrawable* SetImageFilePath(LPCWSTR);

	virtual LONG GetWidth() const;
	virtual LONG GetHeight() const;
	virtual LONG GetTopPosition() const;
	virtual LONG GetLeftPosition() const;
	virtual LONG GetCurrentSprite() const;
	virtual LONG GetNumberOfSprites() const;
	virtual LPCWSTR GetImageFilePath() const;

	virtual void Release();
	virtual void Build(LPDIRECT3DDEVICE9);
	virtual void Draw(LPD3DXSPRITE, DWORD, D3DXVECTOR3*, D3DXVECTOR3*, D3DCOLOR);

protected:

	LONG width;
	LONG height;
	LONG topPosition;
	LONG leftPosition;
	LONG currentSprite;
	LONG numberOfSprites;
	LPCWSTR imageFilePath;

	RECT cuttingHelper;
	D3DXIMAGE_INFO imageInfo;
	LPDIRECT3DTEXTURE9 texture;

};

