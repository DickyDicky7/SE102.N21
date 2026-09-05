#pragma once


#include "Sound.h"
#include "State.h"
#include "Motion.h"
#include "Common.h"
#include "Entity.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"
#include "CollidableEntity.h"


class Bullet;
class BulletState;
class BulletRState;
class BulletMState;
class BulletSState;
class BulletLState;
class BulletFState;
class BulletEnemyState;
class BulletExplodeState;
class BulletBossStage1State;
class BulletScubaSoldierState;
class BulletBossStage2StateHand;
class BulletBossStage2StateHead;


class Bullet : public Entity, public HasTextures<Bullet>, public HasSprites<Bullet>, public HasAnimations<Bullet>, public CollidableEntity
{

public:

	bool IsFake () const { return this->_isFake ; }
	bool IsEnemyBullet() const { return this->_isEnemy; }

	Bullet(            );
	Bullet(BulletState* state);
	virtual ~Bullet();
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void SetState(BulletState* state);
	BulletState* GetState() const;
	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	static Bullet* Create(float x, float y, float vx, float vy, float ax, float ay, float angle, DIRECTION movingDirection, bool isEnemy, BulletState* state, bool isFake = false);

	bool IsBullet() const override { return true; }
	Explosion* CreateDeathExplosion() const override;
	Bullet* CreateBulletExplosion() const;
	CollidableEntity* AsCollidable() override { return this; }

	void  StaticResolveNoCollision(                               ) override;
	void  StaticResolveOnCollision(AABBSweepResult aabbSweepResult) override;
	void DynamicResolveNoCollision(                               ) override;
	void DynamicResolveOnCollision(AABBSweepResult aabbSweepResult) override;

protected:

	bool _isFake ;
	bool _isEnemy;

	BulletState* _state;
	BulletState* _updateState;
	BulletState* _handleInputState;

};


struct BulletParticleConfig
{
	DirectX::XMFLOAT4 coreColor = GraphicsHelper::ToXMFloat4(Constants::Particles::GLOW_COLOUR_DEFAULT);
	float coreSize = Constants::Particles::GLOW_SIZE_DEFAULT;
};

class BulletState : public State<BulletState, Bullet>
{

public:

	BulletState();
	virtual ~BulletState();

	virtual void Exit(Bullet& bullet) override = 0;
	virtual void Enter(Bullet& bullet) override = 0;
	virtual void Render(Bullet& bullet) override = 0;

	virtual BulletState* Update(Bullet& bullet) override = 0;
	virtual BulletState* HandleInput(Bullet& bullet, Input& input) override = 0;

	virtual void SpawnTrail(const Bullet& bullet) const;
	virtual DirectX::XMFLOAT4 GetExplodeColor() const;
	virtual void SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const;

	virtual BulletParticleConfig GetParticleConfig() const;
	virtual bool IsExploding() const { return false; }
	virtual Explosion* CreateDeathExplosion(const Bullet& bullet) const { return nullptr; }
	virtual void OnTerrainCollision(Bullet& bullet, TERRAIN_BLOCK_TYPE terrainType, float normalY) const {}

protected:

	float _time;

};


class BulletRState : public BulletState
{

public:

	BulletRState();
	virtual ~BulletRState();

	virtual void Exit(Bullet& bullet) override;
	virtual void Enter(Bullet& bullet) override;
	virtual void Render(Bullet& bullet) override;

	virtual BulletState* Update(Bullet& bullet) override;
	virtual BulletState* HandleInput(Bullet& bullet, Input& input) override;

	void SpawnTrail(const Bullet& bullet) const override;
	DirectX::XMFLOAT4 GetExplodeColor() const override;
	void SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const override;
	BulletParticleConfig GetParticleConfig() const override;

};


class BulletMState : public BulletState
{

public:

	BulletMState();
	virtual ~BulletMState();

	virtual void Exit(Bullet& bullet) override;
	virtual void Enter(Bullet& bullet) override;
	virtual void Render(Bullet& bullet) override;

	virtual BulletState* Update(Bullet& bullet) override;
	virtual BulletState* HandleInput(Bullet& bullet, Input& input) override;

	void SpawnTrail(const Bullet& bullet) const override;
	DirectX::XMFLOAT4 GetExplodeColor() const override;
	void SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const override;
	BulletParticleConfig GetParticleConfig() const override;

};


class BulletSState : public BulletState
{

public:

	BulletSState();
	virtual ~BulletSState();

	virtual void Exit(Bullet& bullet) override;
	virtual void Enter(Bullet& bullet) override;
	virtual void Render(Bullet& bullet) override;

	static const std::vector<float> spreadDegrees;
	virtual BulletState* Update(Bullet& bullet) override;
	virtual BulletState* HandleInput(Bullet& bullet, Input& input) override;

	void SpawnTrail(const Bullet& bullet) const override;
	DirectX::XMFLOAT4 GetExplodeColor() const override;
	void SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const override;
	BulletParticleConfig GetParticleConfig() const override;

};


class BulletLState : public BulletState
{

public:

	BulletLState();
	virtual ~BulletLState();

	virtual void Exit(Bullet& bullet) override;
	virtual void Enter(Bullet& bullet) override;
	virtual void Render(Bullet& bullet) override;

	virtual BulletState* Update(Bullet& bullet) override;
	virtual BulletState* HandleInput(Bullet& bullet, Input& input) override;

	void SpawnTrail(const Bullet& bullet) const override;
	DirectX::XMFLOAT4 GetExplodeColor() const override;
	void SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const override;
	BulletParticleConfig GetParticleConfig() const override;

};


class BulletFState : public BulletState
{

public:

	BulletFState
	(
		float xO = 0.0f,
		float yO = 0.0f,
		float omega = 0.0f
	);
	virtual ~BulletFState();

	virtual void Exit(Bullet& bullet) override;
	virtual void Enter(Bullet& bullet) override;
	virtual void Render(Bullet& bullet) override;

	virtual BulletState* Update(Bullet& bullet) override;
	virtual BulletState* HandleInput(Bullet& bullet, Input& input) override;

	void SpawnTrail(const Bullet& bullet) const override;
	DirectX::XMFLOAT4 GetExplodeColor() const override;
	void SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const override;
	BulletParticleConfig GetParticleConfig() const override;

protected:

	float _r;
	float _omega;
	float _deltaOmega;
	float _xO;
	float _yO;

};


class BulletEnemyState : public BulletState
{

public:

	BulletEnemyState();
	virtual ~BulletEnemyState();

	virtual void Exit(Bullet& bullet) override;
	virtual void Enter(Bullet& bullet) override;
	virtual void Render(Bullet& bullet) override;

	virtual BulletState* Update(Bullet& bullet) override;
	virtual BulletState* HandleInput(Bullet& bullet, Input& input) override;

	void SpawnTrail(const Bullet& bullet) const override;
	void SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const override;
	BulletParticleConfig GetParticleConfig() const override;

};


class BulletBossStage1State : public BulletState
{

public:

	BulletBossStage1State();
	virtual ~BulletBossStage1State();

	virtual void Exit(Bullet& bullet) override;
	virtual void Enter(Bullet& bullet) override;
	virtual void Render(Bullet& bullet) override;

	virtual BulletState* Update(Bullet& bullet) override;
	virtual BulletState* HandleInput(Bullet& bullet, Input& input) override;

	void SpawnTrail(const Bullet& bullet) const override;
	DirectX::XMFLOAT4 GetExplodeColor() const override;
	void SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const override;
	BulletParticleConfig GetParticleConfig() const override;
	Explosion* CreateDeathExplosion(const Bullet& bullet) const override;
	void OnTerrainCollision(Bullet& bullet, TERRAIN_BLOCK_TYPE terrainType, float normalY) const override;

};


class BulletBossStage2State : public BulletState
{
public:
	BulletBossStage2State();
	virtual ~BulletBossStage2State();

	virtual void Exit(Bullet& bullet) override;
	virtual void Enter(Bullet& bullet) override;
	virtual void Render(Bullet& bullet) override;

	virtual BulletState* Update(Bullet& bullet) override;
	virtual BulletState* HandleInput(Bullet& bullet, Input& input) override;

	void SpawnTrail(const Bullet& bullet) const override;
	DirectX::XMFLOAT4 GetExplodeColor() const override;
	BulletParticleConfig GetParticleConfig() const override;
};

class BulletBossStage2StateHand : public BulletBossStage2State
{
public:
	BulletBossStage2StateHand() = default;
	void SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const override;
};

class BulletBossStage2StateHead : public BulletBossStage2State
{
public:
	BulletBossStage2StateHead() = default;
	void SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const override;
};


class BulletExplodeState : public BulletState
{

public:

	BulletExplodeState(DirectX::XMFLOAT4 color = GraphicsHelper::ToXMFloat4(Constants::Particles::EXPLODE_COLOUR_DEFAULT));
	virtual ~BulletExplodeState();

	virtual void Exit(Bullet& bullet) override;
	virtual void Enter(Bullet& bullet) override;
	virtual void Render(Bullet& bullet) override;

	virtual BulletState* Update(Bullet& bullet) override;
	virtual BulletState* HandleInput(Bullet& bullet, Input& input) override;

	bool IsExploding() const override { return true; }

protected:
	DirectX::XMFLOAT4 _explodeColor;

};


class BulletScubaSoldierState : public BulletState
{

public:

	virtual ~BulletScubaSoldierState();
	BulletScubaSoldierState(float theta, float v0);

	virtual void Exit(Bullet& bullet) override;
	virtual void Enter(Bullet& bullet) override;
	virtual void Render(Bullet& bullet) override;

	virtual BulletState* Update(Bullet& bullet) override;
	virtual BulletState* HandleInput(Bullet& bullet, Input& input) override;

	void SpawnTrail(const Bullet& bullet) const override;
	void SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const override;
	BulletParticleConfig GetParticleConfig() const override;
	Explosion* CreateDeathExplosion(const Bullet& bullet) const override;
	void OnTerrainCollision(Bullet& bullet, TERRAIN_BLOCK_TYPE terrainType, float normalY) const override;

protected:

	float _theta;
	float _v0;

};

