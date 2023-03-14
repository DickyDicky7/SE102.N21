#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

class BaseEntity
{

public:

	virtual BaseEntity* SetPositionX(FLOAT = 0);
	virtual BaseEntity* SetPositionY(FLOAT = 0);
	virtual BaseEntity* SetPositionZ(FLOAT = 0);
	virtual BaseEntity* SetDistanceX(FLOAT = 0);
	virtual BaseEntity* SetDistanceY(FLOAT = 0);
	virtual BaseEntity* SetDistanceZ(FLOAT = 0);

	virtual FLOAT GetPositionX() const;
	virtual FLOAT GetPositionY() const;
	virtual FLOAT GetPositionZ() const;
	virtual FLOAT GetDistanceX() const;
	virtual FLOAT GetDistanceY() const;
	virtual FLOAT GetDistanceZ() const;

protected:

	FLOAT distanceX;
	FLOAT distanceY;
	FLOAT distanceZ;
	D3DXVECTOR3 position;

};