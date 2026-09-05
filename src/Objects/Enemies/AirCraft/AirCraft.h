#pragma once
#include "Bill.h"
#include "Enemy.h"
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"
#include "ItemCommon.h"

class AirCraft : public Entity, public Enemy<Bill>
	, public HasTextures<AirCraft>, public HasSprites<AirCraft>, public HasAnimations<AirCraft>
{
public:

	AirCraft(ITEM_TYPE ammoType, AIRCRAFT_DIRECTION direction);
	virtual ~AirCraft();
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	ITEM_TYPE GetAmmoType();
	void SetAmmoType(ITEM_TYPE ammoType);

	bool IsEnemy() const override { return true; }
	ENEMY_TYPE GetEnemyType() const override { return this->_enemyType; }
	void SetTarget(const Bill* target) override { this->Enemy<Bill>::SetTarget(target); }
	Item* CreateDroppedItem() const override;
	bool TakeBulletHit() override { return this->Enemy<Bill>::TakeEnemyBulletHit(this); }

	AIRCRAFT_DIRECTION GetAircraftDirection()
	{
		return this->_aircraftDirection;
	}
	void SetAircraftDirection(AIRCRAFT_DIRECTION direction) {
		this->_aircraftDirection = direction;
	}
protected:

	ITEM_TYPE _ammoType;
	AIRCRAFT_DIRECTION _aircraftDirection; // horizontal or vertical

	float _time;
	float _y0;
	float _x0;
	float _dt;
	float _period;
	float _amplitude;
	float _phi;

	// Distinguishes "x0/y0 not captured yet" from "the spawn position really is
	// 0".  Testing _x0 == 0.0f for that conflated the two, and would have
	// re-captured the origin on the second frame - after the first one had
	// already displaced the craft by the amplitude.
	bool _hasCapturedOrigin;

	// x0, y0: initial position
	// time: time
	// dt: delta time ( t = t + dt )
	// period: period (in seconds)
	// amplitude: amplitude / radius
	// phi: initial phase (-pi < phi < pi)
};
