#include "IDrawable.h"

IDrawable* IDrawable::SetWidth(LONG width) { this->width = width; return this; }
IDrawable* IDrawable::SetHeight(LONG height) { this->height = height; return this; }
IDrawable* IDrawable::SetTopPosition(LONG topPosition) { this->topPosition = topPosition; return this; }
IDrawable* IDrawable::SetLeftPosition(LONG leftPosition) { this->leftPosition = leftPosition; return this; }
IDrawable* IDrawable::SetCurrentSprite(LONG currentSprite) { this->currentSprite = currentSprite; return this; }
IDrawable* IDrawable::SetNumberOfSprites(LONG numberOfSprites) { this->numberOfSprites = numberOfSprites; return this; }
IDrawable* IDrawable::SetImageFilePath(LPCWSTR imageFilePath) { this->imageFilePath = imageFilePath; return this; }

LONG IDrawable::GetWidth() const { return width; }
LONG IDrawable::GetHeight() const { return height; }
LONG IDrawable::GetTopPosition() const { return topPosition; }
LONG IDrawable::GetLeftPosition() const { return leftPosition; }
LONG IDrawable::GetCurrentSprite() const { return currentSprite; }
LONG IDrawable::GetNumberOfSprites() const { return numberOfSprites; }
LPCWSTR IDrawable::GetImageFilePath() const { return imageFilePath; }

void IDrawable::Release() { texture->Release(); }

void IDrawable::Build(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DXGetImageInfoFromFile(imageFilePath, &imageInfo);
	D3DXCreateTextureFromFileEx
	(
		d3ddev, imageFilePath, imageInfo.Width, imageInfo.Height,
		1, D3DPOOL_DEFAULT, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
		D3DX_DEFAULT, D3DX_DEFAULT,
		NULL, &imageInfo,
		NULL, &texture
	);
}

void IDrawable::Draw(LPD3DXSPRITE spriteHandler, DWORD flags, D3DXVECTOR3* center, D3DXVECTOR3* position, D3DCOLOR color)
{
	cuttingHelper.top = topPosition;
	cuttingHelper.bottom = cuttingHelper.top + height;
	cuttingHelper.left = leftPosition + width * currentSprite;
	cuttingHelper.right = cuttingHelper.left + width;
	
	spriteHandler->Begin(flags);
	spriteHandler->Draw
	(
		texture, &cuttingHelper,
		center, position,
		color
	);
	spriteHandler->End();

	++currentSprite %= numberOfSprites;
}