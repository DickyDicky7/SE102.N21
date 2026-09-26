#pragma once

#include "Input.h"
#include "Common.h"

class Bill;
class Explosion;
class Item;
class CollidableEntity;
struct AABBSweepResult;

class Entity
{

public:

	bool IsDead() const;
	void SetDead(bool dead);
	bool IsDrown() const;
	void SetDrown(bool drown);

	// Debug label only - written by each subclass constructor and read back by
	// LogName. Deliberately not TerrainBlock's _name, which is map-object data.
	LPCWSTR GetDebugName() const;
	void SetDebugName(LPCWSTR debugName);

	Entity();
	virtual ~Entity();
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void HandleInput(Input& input) = 0;
	virtual void LogName();

	virtual bool IsBullet() const;
	virtual bool IsBill() const;
	virtual bool IsEnemy() const;
	virtual bool ShouldRetainWhenDead() const;
	virtual ENEMY_TYPE GetEnemyType() const;
	virtual bool CollidesWithBoundaryWalls() const;
	virtual void SetTarget(const Bill* target);
	virtual Explosion* CreateDeathExplosion() const;
	virtual Item* CreateDroppedItem() const;
	virtual void ProcessSpecialDeathEffects(std::vector<Entity*>& effectEntities);
	virtual void ForEachCollisionEntity(std::function<void(Entity*)> callback);

	virtual TERRAIN_BLOCK_TYPE GetTerrainType() const;
	virtual std::string GetEntityName() const;
	virtual bool IsBridge() const;
	virtual bool IsRockFly() const;
	virtual bool IsWalkableSurface() const;
	virtual bool RidesWithSurface() const;
	virtual bool IsLethalToTouch() const;
	virtual bool IsPushableObstacle() const;
	virtual bool IsVulnerableToBullet() const;
	virtual bool TakeBulletHit();
	virtual bool OnBillCollision(Bill& bill, const AABBSweepResult& result);

	virtual CollidableEntity* AsCollidable();
	virtual void StaticResolveNoCollision();
	virtual void StaticResolveOnCollision(AABBSweepResult aabbSweepResult);
	virtual void DynamicResolveNoCollision();
	virtual void DynamicResolveOnCollision(AABBSweepResult aabbSweepResult);

	virtual void SetW(float w = 0.0f);
	virtual void SetH(float h = 0.0f);
	virtual void SetX(float x = 0.0f);
	virtual void SetY(float y = 0.0f);
	virtual void SetAX(float ax = 0.0f);
	virtual void SetAY(float ay = 0.0f);
	virtual void SetVX(float vx = 0.0f);
	virtual void SetVY(float vy = 0.0f);
	virtual void SetAngle(float angle = 0.0f);
	virtual void SetMovingDirection(DIRECTION movingDirection);

	virtual float GetB() const;
	virtual float GetT() const;
	virtual float GetL() const;
	virtual float GetR() const;
	virtual float GetW() const;
	virtual float GetH() const;
	virtual float GetX() const;
	virtual float GetY() const;
	virtual float GetAX() const;
	virtual float GetAY() const;
	virtual float GetVX() const;
	virtual float GetVY() const;
	virtual float GetAngle() const;
	virtual D3DXVECTOR3 GetPosition() const;
	virtual DIRECTION GetMovingDirection() const;

protected:

	float _w;
	float _h;
	float _ax;
	float _ay;
	float _vx;
	float _vy;
	float _angle;
	D3DXVECTOR3 _position;
	DIRECTION _movingDirection;
	bool _isDead;
	bool _isDrown;
	LPCWSTR _debugName = L"";

};
