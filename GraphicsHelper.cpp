#include "GraphicsHelper.h"

LPDIRECT3DDEVICE9 GraphicsHelper::device;
LPD3DXSPRITE GraphicsHelper::spriteHandler;

TEXTURE GraphicsHelper::CreateTexture(LPCWSTR textureFilePath)
{
	HRESULT result;
	TEXTURE texture;
	D3DXIMAGE_INFO info;

	result = D3DXGetImageInfoFromFile(textureFilePath, &info);
	if (result != D3D_OK)
	{
		MessageBox(NULL, L"Failed to get information from image file", L"Error", MB_OK);
		return NULL;
	}

	result = D3DXCreateTextureFromFileEx
	(
		device, textureFilePath, info.Width, info.Height, 1,
		D3DPOOL_DEFAULT, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
		D3DX_DEFAULT, D3DX_DEFAULT,
		NULL, &info, NULL, &texture
	);
	if (result != D3D_OK)
	{
		MessageBox(NULL, L"Failed to create texture from file", L"Error", MB_OK | MB_ERR_INVALID_CHARS);
		return NULL;
	}

	return texture;
}

SPRITE GraphicsHelper::CreateSprite(INT top, INT left, INT right, INT bottom, DIRECTION spriteDirection, TEXTURE_ID textureId)
{
	RECT* rect = new RECT();
	rect->top = top;
	rect->left = left;
	rect->right = right;
	rect->bottom = bottom;
	return SPRITE({ rect, spriteDirection, textureId });
}

ANIMATION GraphicsHelper::CreateAnimation(DEFAULT_TIME defaultTime, std::vector<std::tuple<SPRITE_ID, TIME>> frames)
{
	for (auto& frame : frames)
	{
		if (std::get<TIME>(frame) == 0)
			std::get<TIME>(frame) = defaultTime;
	}
	return ANIMATION({ defaultTime, frames });
}

void GraphicsHelper::DrawSprite(SPRITE sprite, D3DXVECTOR3 position, DIRECTION movingDirection)
{
	RECT*      rect            = std::get<RECT*>(sprite);
	DIRECTION  spriteDirection = std::get<DIRECTION>(sprite);
	TEXTURE_ID textureId       = std::get<TEXTURE_ID>(sprite);

	D3DXMATRIX flippingMatrix;
	D3DXVECTOR2 flippingCenter(0, 0);
	D3DXVECTOR3 center((FLOAT)(rect->right - rect->left) / 2.0f, (FLOAT)(rect->bottom - rect->top), 0.0f);

	if (movingDirection != spriteDirection)
	{
		position.x = -position.x;
		D3DXVECTOR2 flippingRatio(-2.0f, 2.0f);
		D3DXMatrixTransformation2D(&flippingMatrix, &flippingCenter, 0.0f, &flippingRatio, NULL, 0.0f, NULL);
	}
	if (movingDirection == spriteDirection)
	{
		D3DXVECTOR2 flippingRatio(+2.0f, 2.0f);
		D3DXMatrixTransformation2D(&flippingMatrix, &flippingCenter, 0.0f, &flippingRatio, NULL, 0.0f, NULL);
	}

	//device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	//device->BeginScene();

	//spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	spriteHandler->SetTransform(&flippingMatrix);
	spriteHandler->Draw
	(
		GraphicsDatabase::textures[textureId],
		rect, &center, &position, D3DCOLOR_XRGB(255, 255, 255)
	);
	spriteHandler->SetTransform(NULL);
	//spriteHandler->End();

	//device->EndScene();
	//device->Present(NULL, NULL, NULL, NULL);
}

void GraphicsHelper::InsertTexure(TEXTURE_ID textureId, LPCWSTR textureFilePath)
{
	GraphicsDatabase::textures.insert({ textureId, GraphicsHelper::CreateTexture(textureFilePath) });
}

void GraphicsHelper::InsertSprite(SPRITE_ID spriteId, INT top, INT left, INT right, INT bottom, DIRECTION spriteDirection, TEXTURE_ID textureId)
{
	GraphicsDatabase::sprites.insert({ spriteId, GraphicsHelper::CreateSprite(top, left, right, bottom, spriteDirection, textureId) });
}

void GraphicsHelper::InsertAnimation(ANIMATION_ID animationId, DEFAULT_TIME defaultTime, std::vector<std::tuple<SPRITE_ID, TIME>> frames)
{
	GraphicsDatabase::animations.insert
	({ animationId, GraphicsHelper::CreateAnimation(defaultTime, frames) /* list textures to create animation */ });
}