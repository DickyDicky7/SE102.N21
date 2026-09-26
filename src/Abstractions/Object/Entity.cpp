#include "Entity.h"

Entity::Entity()
	: _w(0.0f), _h(0.0f), _ax(0.0f), _ay(0.0f), _vx(0.0f), _vy(0.0f),
	  _position(0.0f, 0.0f, 0.0f), _angle(0.0f),
	  _movingDirection(DIRECTION::LEFT), _isDead(false), _isDrown(false)
{
}

Entity::~Entity()
{
}

bool Entity::IsDead() const
{
	return this->_isDead;
}

void Entity::SetDead(bool dead)
{
	this->_isDead = dead;
}

bool Entity::IsDrown() const
{
	return this->_isDrown;
}

void Entity::SetDrown(bool drown)
{
	this->_isDrown = drown;
}

LPCWSTR Entity::GetDebugName() const
{
	return this->_debugName;
}

void Entity::SetDebugName(LPCWSTR debugName)
{
	this->_debugName = debugName;
}

void Entity::LogName()
{
	OutputDebugString(this->_debugName);
}

bool Entity::IsBullet() const
{
	return false;
}

bool Entity::IsBill() const
{
	return false;
}

bool Entity::IsEnemy() const
{
	return false;
}

bool Entity::ShouldRetainWhenDead() const
{
	return false;
}

ENEMY_TYPE Entity::GetEnemyType() const
{
	return ENEMY_TYPE::NONE;
}

bool Entity::CollidesWithBoundaryWalls() const
{
	return false;
}

void Entity::SetTarget(const Bill* target)
{
}

Explosion* Entity::CreateDeathExplosion() const
{
	return nullptr;
}

Item* Entity::CreateDroppedItem() const
{
	return nullptr;
}

void Entity::ProcessSpecialDeathEffects(std::vector<Entity*>& effectEntities)
{
}

void Entity::ForEachCollisionEntity(std::function<void(Entity*)> callback)
{
	callback(this);
}

TERRAIN_BLOCK_TYPE Entity::GetTerrainType() const
{
	return TERRAIN_BLOCK_TYPE::NONE;
}

std::string Entity::GetEntityName() const
{
	return "";
}

bool Entity::IsBridge() const
{
	return false;
}

bool Entity::IsRockFly() const
{
	return false;
}

bool Entity::IsWalkableSurface() const
{
	return false;
}

bool Entity::RidesWithSurface() const
{
	return false;
}

bool Entity::IsLethalToTouch() const
{
	return false;
}

bool Entity::IsPushableObstacle() const
{
	return false;
}

bool Entity::IsVulnerableToBullet() const
{
	return true;
}

bool Entity::TakeBulletHit()
{
	return false;
}

bool Entity::OnBillCollision(Bill& bill, const AABBSweepResult& result)
{
	return false;
}

CollidableEntity* Entity::AsCollidable()
{
	return nullptr;
}

void Entity::StaticResolveNoCollision()
{
}

void Entity::StaticResolveOnCollision(AABBSweepResult aabbSweepResult)
{
}

void Entity::DynamicResolveNoCollision()
{
}

void Entity::DynamicResolveOnCollision(AABBSweepResult aabbSweepResult)
{
}

void Entity::SetW(float w)
{
	this->_w = w;
}

void Entity::SetH(float h)
{
	this->_h = h;
}

void Entity::SetX(float x)
{
	this->_position.x = x;
}

void Entity::SetY(float y)
{
	this->_position.y = y;
}

void Entity::SetAX(float ax)
{
	this->_ax = ax;
}

void Entity::SetAY(float ay)
{
	this->_ay = ay;
}

void Entity::SetVX(float vx)
{
	this->_vx = vx;
}

void Entity::SetVY(float vy)
{
	this->_vy = vy;
}

void Entity::SetAngle(float angle)
{
	this->_angle = angle;
}

void Entity::SetMovingDirection(DIRECTION movingDirection)
{
	this->_movingDirection = movingDirection;
}

float Entity::GetB() const
{
	return this->_position.y;
}

float Entity::GetT() const
{
	return this->_position.y + this->_h;
}

float Entity::GetL() const
{
	return this->_position.x - this->_w / 2.0f;
}

float Entity::GetR() const
{
	return this->_position.x + this->_w / 2.0f;
}

float Entity::GetW() const
{
	return this->_w;
}

float Entity::GetH() const
{
	return this->_h;
}

float Entity::GetX() const
{
	return this->_position.x;
}

float Entity::GetY() const
{
	return this->_position.y;
}

float Entity::GetAX() const
{
	return this->_ax;
}

float Entity::GetAY() const
{
	return this->_ay;
}

float Entity::GetVX() const
{
	return this->_vx;
}

float Entity::GetVY() const
{
	return this->_vy;
}

float Entity::GetAngle() const
{
	return this->_angle;
}

D3DXVECTOR3 Entity::GetPosition() const
{
	return this->_position;
}

DIRECTION Entity::GetMovingDirection() const
{
	return this->_movingDirection;
}
