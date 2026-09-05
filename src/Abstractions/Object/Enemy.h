#pragma once

#include <cmath>

#include "Common.h"
#include "Entity.h"

template <class T>
class Enemy
{

public:

	int  GetHitCounts() const { return this->_hitCounts; }
	void SetHitCounts(int hitCounts) { this->_hitCounts = hitCounts; }

	// Consumes one hit; true when this was the hit that brought the count to
	// zero, so the caller knows it owns the death. Used when something other
	// than the enemy itself absorbs the shot (a boss joint hit on its hand).
	bool RegisterHit() { return --this->_hitCounts == 0; }

	Enemy();
	virtual ~Enemy();

	virtual void SetHP(int hp = 0);
	virtual void SetTarget(const T* target = nullptr);

	virtual int GetHP() const;
	virtual const T* GetTarget() const;

	float CalculateTargetAngle(const Entity* self) const;
	float CalculateTargetDistance(const Entity* self) const;
	float CalculateShootingAngle(const Entity* self) const;

	virtual bool TakeEnemyBulletHit(Entity* self)
	{
		if (!self) return false;
		if (--this->_hitCounts <= 0)
		{
			self->SetDead(true);
		}
		return true;
	}
protected:

	int _hp;
	const T* _target;
	int _hitCounts;
	ENEMY_TYPE _enemyType;

};

template <class T>
inline Enemy<T>::Enemy() : _hp(0), _target(nullptr), _hitCounts(0), _enemyType(ENEMY_TYPE::NONE)
{
}

template <class T>
inline Enemy<T>::~Enemy()
{
}

template <class T>
inline void Enemy<T>::SetHP(int hp) { this->_hp = hp; }

template <class T>
inline void Enemy<T>::SetTarget(const T* target) { this->_target = target; }

template <class T>
inline int Enemy<T>::GetHP() const { return this->_hp; }

template <class T>
inline const T* Enemy<T>::GetTarget() const { return this->_target; }

template <class T>
inline float Enemy<T>::CalculateTargetAngle(const Entity* self) const
{
	if (!this->_target || !self) return -Constants::Physics::QUARTER_CIRCLE_DEGREES;
	float dx = +(self->GetPosition().x - this->_target->GetPosition().x);
	float dy = -(self->GetPosition().y - this->_target->GetPosition().y);

	if (dx > 0 && dy < 0)
		return D3DXToDegree(-std::atan(dx / std::abs(dy)));
	if (dx < 0 && dy < 0)
		return D3DXToDegree(std::atan(std::abs(dx) / std::abs(dy)));
	if (dx > 0 && dy > 0)
		return D3DXToDegree(std::atan(dx / dy)) - Constants::Physics::HALF_CIRCLE_DEGREES;
	if (dx < 0 && dy > 0)
		return D3DXToDegree(-std::atan(std::abs(dx) / dy)) + Constants::Physics::HALF_CIRCLE_DEGREES;

	return -Constants::Physics::QUARTER_CIRCLE_DEGREES;
}

template <class T>
inline float Enemy<T>::CalculateTargetDistance(const Entity* self) const
{
	if (!this->_target || !self) return 0.0f;
	float dx = +(self->GetPosition().x - this->_target->GetPosition().x);
	float dy = -(self->GetPosition().y - this->_target->GetPosition().y);
	return std::sqrt(dx * dx + dy * dy);
}

template <class T>
inline float Enemy<T>::CalculateShootingAngle(const Entity* self) const
{
	if (!this->_target || !self) return +Constants::Physics::QUARTER_CIRCLE_DEGREES;
	float dx = self->GetPosition().x - this->_target->GetPosition().x;
	float dy = -(self->GetPosition().y - this->_target->GetPosition().y);
	if (dy == 0.0f)
	{
		return +Constants::Physics::QUARTER_CIRCLE_DEGREES;
	}
	return D3DXToDegree(std::atan(dx / dy));
}
