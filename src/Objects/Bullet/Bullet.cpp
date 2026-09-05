#include "Bill.h"
#include "Fire.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Bridge.h"
#include "ParticleSystem.h"
#include "Soldier.h"
#include "RockFly.h"
#include "TerrainBlock.h"
#include "GunBossStage1.h"
#include "BossStage3Hand.h"
#include "FinalBossStage1.h"
#include "BossStage3Joint.h"

Bullet::Bullet(                  ) : Entity(), HasTextures(), HasSprites(), HasAnimations(), CollidableEntity()
{
	this->_isFake = false;
	this->_isEnemy = false;
	this->_state = nullptr;
	this->_updateState = nullptr;
	this->_handleInputState = nullptr;
	CollidableEntity::_self = this;
}

Bullet::Bullet(BulletState* state) : Bullet()
{
	this->_state = state;
}

Bullet::~Bullet()
{
	Destroy(this->_state);
	Destroy(this->_updateState);
	Destroy(this->_handleInputState);
}

Bullet* Bullet::Create(float x, float y, float vx, float vy, float ax, float ay, float angle, DIRECTION movingDirection, bool isEnemy, BulletState* state, bool isFake)
{
	Bullet* bullet = new Bullet();
	bullet->_isFake = isFake;
	bullet->_isEnemy = isEnemy;
	bullet->SetX(x);
	bullet->SetY(y);
	bullet->SetVX(vx);
	bullet->SetVY(vy);
	bullet->SetAX(ax);
	bullet->SetAY(ay);
	bullet->SetAngle(angle);
	bullet->SetMovingDirection(movingDirection);
	bullet->SetState(state);
	return bullet;
}

Explosion* Bullet::CreateDeathExplosion() const
{
	if (this->_state) return this->_state->CreateDeathExplosion(*this);
	return nullptr;
}

Bullet* Bullet::CreateBulletExplosion() const
{
	BulletState* bulletState = this->GetState();
	DirectX::XMFLOAT4 explodeColor = bulletState ? bulletState->GetExplodeColor() : GraphicsHelper::ToXMFloat4(Constants::Particles::EXPLODE_COLOUR_DEFAULT);

	Bullet* explosion = new Bullet();
	if (this->GetVX() < 0.0f)
		explosion->SetX(this->GetL() - Constants::Weapons::BULLET_EXPLOSION_OFFSET);
	else if (this->GetVX() > 0.0f)
		explosion->SetX(this->GetR() + Constants::Weapons::BULLET_EXPLOSION_OFFSET);
	else
		explosion->SetX(this->GetX() + 0.0f);

	if (this->GetVY() < 0.0f)
		explosion->SetY(this->GetB() - Constants::Weapons::BULLET_EXPLOSION_OFFSET);
	else if (this->GetVY() > 0.0f)
		explosion->SetY(this->GetT() + Constants::Weapons::BULLET_EXPLOSION_OFFSET);
	else
		explosion->SetY(this->GetY() + 0.0f);

	explosion->SetState(new BulletExplodeState(explodeColor));
	return explosion;
}

void Bullet::Update()
{
	if (this->_state)
	{
		DeferState(this->_updateState, this->_state->Update(*this));
		if (!this->IsDead())
		{
			this->_state->SpawnTrail(*this);
		}
	}

	ApplyDeferredState(this->_state, this->_updateState, this);
	ApplyDeferredState(this->_state, this->_handleInputState, this);
}

void Bullet::Render()
{
	// A Bullet built through the default constructor - or through Create with a
	// null state - has none, and the rest of this function already guards for
	// that; this line did not.
	if (this->_state)
	{
		this->_state->Render(*this);
	}
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();

	// An exploding bullet draws its own sprite above and gets no glow on top.
	if (this->_state && this->_state->IsExploding())
	{
		return;
	}

	auto config = this->_state ? this->_state->GetParticleConfig() : BulletParticleConfig{};
	GraphicsHelper::DrawParticle(D3DXVECTOR3(this->GetX(), this->GetY(), 0.0f), config.coreSize, config.coreColor);
}

void Bullet::HandleInput(Input& input)
{
	if (this->_state)
	{
		DeferState(this->_handleInputState, this->_state->HandleInput(*this, input));
	}
}

void Bullet::SetState(BulletState* state)
{
	ChangeState(this->_state, state, this);
}

BulletState* Bullet::GetState() const
{
	return this->_state;
}

void Bullet::LoadSprites()
{
	if (HasSprites<Bullet>::_hasBeenLoaded) return;
	HasSprites<Bullet>::_hasBeenLoaded = true;

	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::R_01, 42, 52, 55, 45, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::M_01, 28, 69, 74, 33, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::S_01, 28, 69, 74, 33, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::S_02, 27, 61, 67, 33, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::S_03, 25, 50, 58, 33, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::L_01, 39, 23, 38, 45, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::F_01, 37, 41, 49, 45, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);

	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::BURST_01, 38, 03, 18, 44, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);

	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::ENEMY_01, 42, 58, 60, 44, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);

	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::BOSS_1_01, 25, 50, 58, 33, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::BOSS_2_01, 00, 00, 15, 15, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::BOSS_2_02, 00, 19, 34, 15, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::BOSS_2_03, 00, 37, 52, 15, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);
	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::BOSS_2_04, 00, 55, 70, 15, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);

	GraphicsHelper::InsertSprite(BULLET_SPRITE_ID::EXPLODE_01, 25, 41, 46, 32, DIRECTION::LEFT, BULLET_TEXTURE_ID::BULLET_01);

	OutputDebugString(L"Bullet Sprites Loaded Successfully\n");
}

void Bullet::LoadTextures()
{
	if (HasTextures<Bullet>::_hasBeenLoaded) return;
	HasTextures<Bullet>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(BULLET_TEXTURE_ID::BULLET_01, L"Resources\\Textures\\Bullet.bmp");

	OutputDebugString(L"Bullet Textures Loaded Successfully\n");
}

void Bullet::LoadAnimations()
{
	if (HasAnimations<Bullet>::_hasBeenLoaded) return;
	HasAnimations<Bullet>::_hasBeenLoaded = true;

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::R, Constants::Weapons::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{BULLET_SPRITE_ID::R_01, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::M, Constants::Weapons::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{BULLET_SPRITE_ID::M_01, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::S, Constants::Weapons::ANIMATION_SPREAD_AMMO_DELAY_MILLISECONDS,
		{
			{BULLET_SPRITE_ID::S_01, 0},
			{BULLET_SPRITE_ID::S_02, 0},
			{BULLET_SPRITE_ID::S_03, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::L, Constants::Weapons::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{BULLET_SPRITE_ID::L_01, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::F, Constants::Weapons::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{BULLET_SPRITE_ID::F_01, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::BURST, Constants::Weapons::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{BULLET_SPRITE_ID::BURST_01, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::ENEMY, Constants::Weapons::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{BULLET_SPRITE_ID::ENEMY_01, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::BOSS_1, Constants::Weapons::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{BULLET_SPRITE_ID::BOSS_1_01, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::BOSS_2, Constants::Weapons::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{BULLET_SPRITE_ID::BOSS_2_01, 0},
			{BULLET_SPRITE_ID::BOSS_2_02, 0},
			{BULLET_SPRITE_ID::BOSS_2_03, 0},
			{BULLET_SPRITE_ID::BOSS_2_04, 0},
		});

	GraphicsHelper::InsertAnimation(BULLET_ANIMATION_ID::EXPLODE, Constants::Weapons::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{BULLET_SPRITE_ID::EXPLODE_01, 0},
		});

	OutputDebugString(L"Bullet Animations Loaded Successfully\n");
}

void Bullet::StaticResolveNoCollision(                               )
{
}

void Bullet::StaticResolveOnCollision(AABBSweepResult aabbSweepResult)
{
}

void Bullet::DynamicResolveNoCollision(                               )
{
}

void Bullet::DynamicResolveOnCollision(AABBSweepResult aabbSweepResult)
{
	Entity* target = aabbSweepResult.surfaceEntity;
	if (!target)
		return;

	if (target->IsRockFly() || target->IsBridge())
	{
		return;
	}

	if (target->IsEnemy())
	{
		if (this->_isEnemy)
		{
			return;
		}

		if (!target->IsVulnerableToBullet())
		{
			return;
		}

		this->SetDead(true);
		Sound::GetInstance()->Play("beShooted", false, 1);
		target->TakeBulletHit();
		return;
	}

	if (target->IsBill() && this->_isEnemy)
	{
		static_cast<Bill*>(target)->GoDead();
		this->SetDead(true);
		return;
	}

	TERRAIN_BLOCK_TYPE terrainType = target->GetTerrainType();
	if (terrainType != TERRAIN_BLOCK_TYPE::NONE && this->_state)
	{
		this->_state->OnTerrainCollision(*this, terrainType, aabbSweepResult.normalY);
		return;
	}
}
