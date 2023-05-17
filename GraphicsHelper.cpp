#include "GraphicsHelper.h"





#define DRAW_BOX FALSE
struct VERTEX
{
	FLOAT x;
	FLOAT y;
	FLOAT z;
	D3DCOLOR color;
};
VOID* pVoid = NULL;
VERTEX verticesH[4];
VERTEX verticesV[4];
LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
void RotateVertex(VERTEX& vertex, FLOAT angle)
{
	FLOAT x0 = vertex.x;
	FLOAT y0 = vertex.y;
	vertex.x = x0 * std::cos(D3DXToRadian(angle)) - y0 * std::sin(D3DXToRadian(angle));
	vertex.y = x0 * std::sin(D3DXToRadian(angle)) + y0 * std::cos(D3DXToRadian(angle));
}





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
		device, textureFilePath, info.Width, info.Height, D3DX_DEFAULT,
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

void GraphicsHelper::DrawSprite(SPRITE sprite, D3DXVECTOR3 position, DIRECTION movingDirection, FLOAT angle)
{
	RECT*      rect            = std::get<RECT*>(sprite);
	DIRECTION  spriteDirection = std::get<DIRECTION>(sprite);
	TEXTURE_ID textureId       = std::get<TEXTURE_ID>(sprite);

	D3DXMATRIX  transformMatrix;
	D3DXVECTOR2 flippingCenter(0.0f, 0.0f);
	D3DXVECTOR2 rotatingCenter(0.0f, 0.0f);

	FLOAT drawingCenterX = (FLOAT)(rect->right  - rect->left) / 2.0f;
	FLOAT drawingCenterY = (FLOAT)(rect->bottom - rect->top ) / 1.0f;
	//FLOAT drawingCenterX0 = drawingCenterX;
	//FLOAT drawingCenterY0 = drawingCenterY;
	//drawingCenterX = drawingCenterX0 * std::cos(-D3DXToRadian(angle)) - drawingCenterY0 * std::sin(-D3DXToRadian(angle));
	//drawingCenterY = drawingCenterX0 * std::sin(-D3DXToRadian(angle)) + drawingCenterY0 * std::cos(-D3DXToRadian(angle));
	D3DXVECTOR3 drawingCenter(drawingCenterX, drawingCenterY, 0.0f);

	FLOAT x0 = position.x;
	FLOAT y0 = position.y;
	position.x = x0 * std::cos(-D3DXToRadian(angle)) - y0 * std::sin(-D3DXToRadian(angle));
	position.y = x0 * std::sin(-D3DXToRadian(angle)) + y0 * std::cos(-D3DXToRadian(angle));





	if (DRAW_BOX)
	{
		verticesH[0] = { position.x - (rect->right - rect->left) / 2.0f, position.y + (rect->bottom - rect->top), 0.0f, 0xffff007f }; // TL vertex
		verticesH[1] = { position.x + (rect->right - rect->left) / 2.0f, position.y + (rect->bottom - rect->top), 0.0f, 0xffff007f }; // TR vertex
		verticesH[2] = { position.x + (rect->right - rect->left) / 2.0f, position.y, 0.0f, 0xffff007f }; // BR vertex
		verticesH[3] = { position.x - (rect->right - rect->left) / 2.0f, position.y, 0.0f, 0xffff007f }; // BL vertex

		for (auto& vertex : verticesH)
		{
			RotateVertex(vertex, angle);
		}

		verticesV[0] = { position.x - (rect->right - rect->left) / 2.0f, position.y, 0.0f, 0xffff007f }; // BL vertex
		verticesV[1] = { position.x - (rect->right - rect->left) / 2.0f, position.y + (rect->bottom - rect->top), 0.0f, 0xffff007f }; // TL vertex
		verticesV[2] = { position.x + (rect->right - rect->left) / 2.0f, position.y + (rect->bottom - rect->top), 0.0f, 0xffff007f }; // TR vertex
		verticesV[3] = { position.x + (rect->right - rect->left) / 2.0f, position.y, 0.0f, 0xffff007f }; // BR vertex

		for (auto& vertex : verticesV)
		{
			RotateVertex(vertex, angle);
		}
	}





	if (movingDirection != spriteDirection)
	{
		position.x = -position.x;
		position.y = -position.y;
		D3DXVECTOR2 flippingRatio(-1.0f, -1.0f);
		D3DXMatrixTransformation2D(&transformMatrix, &flippingCenter, 0.0f, &flippingRatio, &rotatingCenter, D3DXToRadian(angle), NULL);
	}
	if (movingDirection == spriteDirection)
	{
		position.x = +position.x;
		position.y = -position.y;
		D3DXVECTOR2 flippingRatio(+1.0f, -1.0f);
		D3DXMatrixTransformation2D(&transformMatrix, &flippingCenter, 0.0f, &flippingRatio, &rotatingCenter, D3DXToRadian(angle), NULL);
	}

	//device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	//device->BeginScene();

	



	if (DRAW_BOX)
	{
		if (!vertexBuffer)
		{
			device->CreateVertexBuffer(10 * sizeof(VERTEX), 0, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &vertexBuffer, NULL);
			device->SetRenderState(D3DRS_LIGHTING, FALSE);
			device->SetRenderState(D3DRS_COLORVERTEX, TRUE);

			device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
			device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
		}

		vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, verticesH, sizeof(verticesH));
		vertexBuffer->Unlock();
		device->SetStreamSource(0, vertexBuffer, 0, sizeof(VERTEX));
		device->DrawPrimitive(D3DPT_LINELIST, 0, 4);
		
		vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, verticesV, sizeof(verticesV));
		vertexBuffer->Unlock();
		device->SetStreamSource(0, vertexBuffer, 0, sizeof(VERTEX));
		device->DrawPrimitive(D3DPT_LINELIST, 0, 4);

		//vertexBuffer->Release();
		//vertexBuffer = NULL;
		//pVoid = NULL;
	}





	spriteHandler->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE);
	spriteHandler->SetTransform(&transformMatrix);
	spriteHandler->Draw
	(
		GraphicsDatabase::textures[textureId],
		rect, &drawingCenter, &position, D3DCOLOR_XRGB(255, 255, 255)
	);
	spriteHandler->SetTransform(NULL);
	spriteHandler->End();

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
