#pragma once

#include "Input.h"
#include "Common.h"

template <class T>
class Entity
{

public:

	Entity();
	virtual ~Entity();
	virtual void LoadAssets(LPDIRECT3DDEVICE9 *d3ddev, LPD3DXSPRITE *spriteHandler) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void HandleInput(Input&) = 0;

	virtual T* SetX(FLOAT = 0.0f);
	virtual T* SetY(FLOAT = 0.0f);
	virtual T* SetAX(FLOAT = 0.0f);
	virtual T* SetAY(FLOAT = 0.0f);
	virtual T* SetVX(FLOAT = 0.0f);
	virtual T* SetVY(FLOAT = 0.0f);
	virtual T* SetDirection(DIRECTION);

	virtual FLOAT GetX() const;
	virtual FLOAT GetY() const;
	virtual FLOAT GetAX() const;
	virtual FLOAT GetAY() const;
	virtual FLOAT GetVX() const;
	virtual FLOAT GetVY() const;
	virtual DIRECTION GetDirection() const;
	virtual D3DXVECTOR3 GetPosition() const;

protected:

	T* self = NULL;

	FLOAT ax;
	FLOAT ay;
	FLOAT vx;
	FLOAT vy;
	DIRECTION direction;
	D3DXVECTOR3 position;

};

template <class T>
inline Entity<T>::Entity() : position(0.0f, 0.0f, 0.0f), vx(0.0f), vy(0.0f), ax(0.0f), ay(0.0f), direction(LEFT)
{
}

template <class T>
inline Entity<T>::~Entity()
{
}

template <class T>
inline T* Entity<T>::SetX(FLOAT x) { position.x = x; return self; }

template <class T>
inline T* Entity<T>::SetY(FLOAT y) { position.y = y; return self; }

template <class T>
inline T* Entity<T>::SetAX(FLOAT ax) { this->ax = ax; return self; }

template <class T>
inline T* Entity<T>::SetAY(FLOAT ay) { this->ay = ay; return self; }

template <class T>
inline T* Entity<T>::SetVX(FLOAT vx) { this->vx = vx; return self; }

template <class T>
inline T* Entity<T>::SetVY(FLOAT vy) { this->vy = vy; return self; }

template <class T>
inline T* Entity<T>::SetDirection(DIRECTION direction) { this->direction = direction; return self; }

template <class T>
inline FLOAT Entity<T>::GetX() const { return position.x; }

template <class T>
inline FLOAT Entity<T>::GetY() const { return position.y; }

template <class T>
inline FLOAT Entity<T>::GetAX() const { return ax; }

template <class T>
inline FLOAT Entity<T>::GetAY() const { return ay; }

template <class T>
inline FLOAT Entity<T>::GetVX() const { return vx; }

template <class T>
inline FLOAT Entity<T>::GetVY() const { return vy; }

template <class T>
inline DIRECTION Entity<T>::GetDirection() const { return direction; }

template <class T>
inline D3DXVECTOR3 Entity<T>::GetPosition() const { return position; }