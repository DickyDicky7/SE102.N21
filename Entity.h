#pragma once

#include "Input.h"
#include "Common.h"

class Entity
{

public:

	Entity();
	virtual ~Entity();
	LPCWSTR name = L"";
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void HandleInput(Input&) = 0;
	virtual void LogName() { OutputDebugString(name); };

	virtual void SetW(FLOAT = 0.0f);
	virtual void SetH(FLOAT = 0.0f);
	virtual void SetX(FLOAT = 0.0f);
	virtual void SetY(FLOAT = 0.0f);
	virtual void SetAX(FLOAT = 0.0f);
	virtual void SetAY(FLOAT = 0.0f);
	virtual void SetVX(FLOAT = 0.0f);
	virtual void SetVY(FLOAT = 0.0f);
	virtual void SetAngle(FLOAT = 0.0f);
	virtual void SetMovingDirection(DIRECTION);

	virtual FLOAT GetB() const;
	virtual FLOAT GetT() const;
	virtual FLOAT GetL() const;
	virtual FLOAT GetR() const;
	virtual FLOAT GetW() const;
	virtual FLOAT GetH() const;
	virtual FLOAT GetX() const;
	virtual FLOAT GetY() const;
	virtual FLOAT GetAX() const;
	virtual FLOAT GetAY() const;
	virtual FLOAT GetVX() const;
	virtual FLOAT GetVY() const;
	virtual FLOAT GetAngle() const;
	virtual D3DXVECTOR3 GetPosition() const;
	virtual DIRECTION GetMovingDirection() const;

protected:

	FLOAT w;
	FLOAT h;
	FLOAT ax;
	FLOAT ay;
	FLOAT vx;
	FLOAT vy;
	FLOAT angle;
	D3DXVECTOR3 position;
	DIRECTION movingDirection;

};

inline Entity::Entity() : w(0.0f), h(0.0f), ax(0.0f), ay(0.0f), vx(0.0f), vy(0.0f), position(0.0f, 0.0f, 0.0f), angle(0.0f), movingDirection(DIRECTION::LEFT)
{
}

inline Entity::~Entity()
{
}

inline void Entity::SetW(FLOAT w) { this->w = w; }

inline void Entity::SetH(FLOAT h) { this->h = h; }

inline void Entity::SetX(FLOAT x) { position.x = x; }

inline void Entity::SetY(FLOAT y) { position.y = y; }

inline void Entity::SetAX(FLOAT ax) { this->ax = ax; }

inline void Entity::SetAY(FLOAT ay) { this->ay = ay; }

inline void Entity::SetVX(FLOAT vx) { this->vx = vx; }

inline void Entity::SetVY(FLOAT vy) { this->vy = vy; }

inline void Entity::SetAngle(FLOAT angle) { this->angle = angle; }

inline void Entity::SetMovingDirection(DIRECTION movingDirection) { this->movingDirection = movingDirection; }

inline FLOAT Entity::GetB() const { return position.y; }

inline FLOAT Entity::GetT() const { return position.y + h; }

inline FLOAT Entity::GetL() const { return position.x - w / 2.0f; }

inline FLOAT Entity::GetR() const { return position.x + w / 2.0f; }

inline FLOAT Entity::GetW() const { return w; }

inline FLOAT Entity::GetH() const { return h; }

inline FLOAT Entity::GetX() const { return position.x; }

inline FLOAT Entity::GetY() const { return position.y; }

inline FLOAT Entity::GetAX() const { return ax; }

inline FLOAT Entity::GetAY() const { return ay; }

inline FLOAT Entity::GetVX() const { return vx; }

inline FLOAT Entity::GetVY() const { return vy; }

inline FLOAT Entity::GetAngle() const { return angle; }

inline D3DXVECTOR3 Entity::GetPosition() const { return position; }

inline DIRECTION Entity::GetMovingDirection() const { return movingDirection; }