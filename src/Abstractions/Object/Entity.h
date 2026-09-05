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

	bool IsDead() const { return this->_isDead; }
	void SetDead(bool dead) { this->_isDead = dead; }
	bool IsDrown() const { return this->_isDrown; }
	void SetDrown(bool drown) { this->_isDrown = drown; }

	// Debug label only - written by each subclass constructor and read back by
	// LogName. Deliberately not TerrainBlock's _name, which is map-object data.
	LPCWSTR GetDebugName() const { return this->_debugName; }
	void SetDebugName(LPCWSTR debugName) { this->_debugName = debugName; }

	Entity();
	virtual ~Entity();
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void HandleInput(Input& input) = 0;
	virtual void LogName() { OutputDebugString(this->_debugName); };

	virtual bool IsBullet() const { return false; }
	virtual bool IsBill() const { return false; }
	virtual bool IsEnemy() const { return false; }
	virtual bool ShouldRetainWhenDead() const { return false; }
	virtual ENEMY_TYPE GetEnemyType() const { return ENEMY_TYPE::NONE; }
	virtual bool CollidesWithBoundaryWalls() const { return false; }
	virtual void SetTarget(const Bill* target) {}
	virtual Explosion* CreateDeathExplosion() const { return nullptr; }
	virtual Item* CreateDroppedItem() const { return nullptr; }
	virtual void ProcessSpecialDeathEffects(std::vector<Entity*>& effectEntities) {}
	virtual void ForEachCollisionEntity(std::function<void(Entity*)> callback) { callback(this); }

	virtual TERRAIN_BLOCK_TYPE GetTerrainType() const { return TERRAIN_BLOCK_TYPE::NONE; }
	virtual std::string GetEntityName() const { return ""; }
	virtual bool IsBridge() const { return false; }
	virtual bool IsRockFly() const { return false; }
	virtual bool IsWalkableSurface() const { return false; }
	virtual bool RidesWithSurface() const { return false; }
	virtual bool IsLethalToTouch() const { return false; }
	virtual bool IsPushableObstacle() const { return false; }
	virtual bool IsVulnerableToBullet() const { return true; }
	virtual bool TakeBulletHit();
	virtual bool OnBillCollision(Bill& bill, const AABBSweepResult& result) { return false; }

	virtual CollidableEntity* AsCollidable() { return nullptr; }
	virtual void StaticResolveNoCollision() {}
	virtual void StaticResolveOnCollision(AABBSweepResult aabbSweepResult) {}
	virtual void DynamicResolveNoCollision() {}
	virtual void DynamicResolveOnCollision(AABBSweepResult aabbSweepResult) {}

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

inline Entity::Entity() : _w(0.0f), _h(0.0f), _ax(0.0f), _ay(0.0f), _vx(0.0f), _vy(0.0f), _position(0.0f, 0.0f, 0.0f), _angle(0.0f), _movingDirection(DIRECTION::LEFT), _isDead(false), _isDrown(false)
{
}

inline Entity::~Entity()
{
}

inline void Entity::SetW(float w) { this->_w = w; }

inline void Entity::SetH(float h) { this->_h = h; }

inline void Entity::SetX(float x) { this->_position.x = x; }

inline void Entity::SetY(float y) { this->_position.y = y; }

inline void Entity::SetAX(float ax) { this->_ax = ax; }

inline void Entity::SetAY(float ay) { this->_ay = ay; }

inline void Entity::SetVX(float vx) { this->_vx = vx; }

inline void Entity::SetVY(float vy) { this->_vy = vy; }

inline void Entity::SetAngle(float angle) { this->_angle = angle; }

inline void Entity::SetMovingDirection(DIRECTION movingDirection) { this->_movingDirection = movingDirection; }

inline float Entity::GetB() const { return this->_position.y; }

inline float Entity::GetT() const { return this->_position.y + this->_h; }

inline float Entity::GetL() const { return this->_position.x - this->_w / 2.0f; }

inline float Entity::GetR() const { return this->_position.x + this->_w / 2.0f; }

inline float Entity::GetW() const { return this->_w; }

inline float Entity::GetH() const { return this->_h; }

inline float Entity::GetX() const { return this->_position.x; }

inline float Entity::GetY() const { return this->_position.y; }

inline float Entity::GetAX() const { return this->_ax; }

inline float Entity::GetAY() const { return this->_ay; }

inline float Entity::GetVX() const { return this->_vx; }

inline float Entity::GetVY() const { return this->_vy; }

inline float Entity::GetAngle() const { return this->_angle; }

inline D3DXVECTOR3 Entity::GetPosition() const { return this->_position; }

inline DIRECTION Entity::GetMovingDirection() const { return this->_movingDirection; }

inline bool Entity::TakeBulletHit() { return false; }
