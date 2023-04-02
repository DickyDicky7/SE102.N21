#include "GraphicsHelper.h"

LPDIRECT3DDEVICE9 GraphicsHelper::device;
LPD3DXSPRITE GraphicsHelper::spriteHandler;

LPDIRECT3DTEXTURE9 GraphicsHelper::CreateTexture(LPCWSTR textureFilePath)
{
	HRESULT result;
	D3DXIMAGE_INFO info;
	LPDIRECT3DTEXTURE9 texture;

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

std::pair<RECT*, TEXTURE_ID> GraphicsHelper::CreateSprite(INT top, INT left, INT right, INT bottom, TEXTURE_ID textureId)
{
	RECT* rect = new RECT();
	rect->top = top;
	rect->left = left;
	rect->right = right;
	rect->bottom = bottom;
	return std::pair<RECT*, TEXTURE_ID>({ rect, textureId });
}

std::pair<DWORD, std::vector<std::pair<SPRITE_ID, DWORD>>> GraphicsHelper::CreateAnimation(DWORD defaultTime, std::vector<std::pair<SPRITE_ID, DWORD>> frames)
{
	for (auto& frame : frames)
	{
		if (frame.second == 0) frame.second = defaultTime;
	}
	return std::pair<DWORD, std::vector<std::pair<SPRITE_ID, DWORD>>>({ defaultTime, frames });
}

void GraphicsHelper::DrawSprite(std::pair<RECT*, TEXTURE_ID> sprite, D3DXVECTOR3 position, DIRECTION direction)
{
	D3DXMATRIX flippingMatrix;
	D3DXVECTOR2 flippingCenter(0, 0);
	D3DXVECTOR3 center((FLOAT)(sprite.first->right - sprite.first->left) / 2.0f, (FLOAT)(sprite.first->bottom - sprite.first->top), 0.0f);

	if (direction == LEFT)
	{
		position.x = -position.x;
		D3DXVECTOR2 flippingRatio(-2.0f, 2.0f);
		D3DXMatrixTransformation2D(&flippingMatrix, &flippingCenter, 0.0f, &flippingRatio, NULL, 0.0f, NULL);
	}
	if (direction == RIGHT)
	{
		D3DXVECTOR2 flippingRatio(+2.0f, 2.0f);
		D3DXMatrixTransformation2D(&flippingMatrix, &flippingCenter, 0.0f, &flippingRatio, NULL, 0.0f, NULL);
	}

	//device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	//device->BeginScene();

	spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	spriteHandler->SetTransform(&flippingMatrix);
	spriteHandler->Draw
	(
		GraphicsDatabase::textures[sprite.second],
		sprite.first, &center, &position, D3DCOLOR_XRGB(255, 255, 255)
	);
	spriteHandler->SetTransform(NULL);
	spriteHandler->End();

	//device->EndScene();
	//device->Present(NULL, NULL, NULL, NULL);
}

void GraphicsHelper::InsertTexures(TEXTURE_ID id, LPCWSTR link) {
	GraphicsDatabase::textures.insert({ id, GraphicsHelper::CreateTexture(link) });
}

void GraphicsHelper::InsertSprites(SPRITE_ID ID_SPRITE, INT top, INT left, INT right, INT bottom, TEXTURE_ID textureId) {
	GraphicsDatabase::sprites.insert({ ID_SPRITE, GraphicsHelper::CreateSprite(top ,left  ,right ,bottom ,textureId) });
}

void GraphicsHelper::InsertAnimation(ANIMATION_ID ID_ANI, DWORD defaultTime, std::vector<std::pair<SPRITE_ID, DWORD>> frames) {
	GraphicsDatabase::animations.insert
	({ ID_ANI,
		// list textures to create animation
		GraphicsHelper::CreateAnimation(defaultTime, frames)
		});
}
