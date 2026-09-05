#include <cmath>
#include "Bridge.h"
#include "Soldier.h"
#include "TerrainBlock.h"

Soldier::Soldier() : Entity(), HasTextures(), HasSprites(), HasAnimations(), CollidableEntity(), HasWeapons(new BulletEnemyState())
{
	this->_vx = +1.0f;
	this->_vy = -1.0f;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position.x = Constants::Enemies::Soldier::DEFAULT_SPAWN_X;
	this->_position.y = Constants::Enemies::Soldier::DEFAULT_SPAWN_Y;
	this->SetDebugName(L"Soldier\n");

	this->_updateState = nullptr;
	this->_handleInputState = nullptr;
	// set direction default is right
	this->_movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	this->_state = new SoldierRunState();

	this->_shootable = false;
	this->_firingRate = Constants::Enemies::Soldier::FIRING_RATE_MILLISECONDS;
	CollidableEntity::_self = this;

	this->_hitCounts = Constants::Enemies::Soldier::HEALTH_POINTS;
	this->_enemyType = ENEMY_TYPE::HUMAN;
}

void Soldier::Fire()
{
	float coefficient = this->_state ? this->_state->GetGunMountOffsetRatio() : 0.0f;
	if (this->_movingDirection == DIRECTION::LEFT)
	{
		this->HasWeapons::Fire(this->GetL(), this->_position.y + this->_h * coefficient, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, this->_movingDirection);
	}
	else
	if (this->_movingDirection == DIRECTION::RIGHT)
	{
		this->HasWeapons::Fire(this->GetR(), this->_position.y + this->_h * coefficient, 0.0f, +1.0f, 0.0f, 0.0f, 0.0f, this->_movingDirection);
	}
}

void Soldier::StaticResolveNoCollision()
{
	if (this->_state && this->_state->IsDead())
	{
		this->_surfaceEntity = nullptr;
		this->_isAbSurface = false;
		return;
	}
}

void Soldier::StaticResolveOnCollision(AABBSweepResult aabbSweepResult)
{
	if (this->_state && this->_state->IsDead())
	{
		this->_surfaceEntity = nullptr;
		this->_isAbSurface = false;
		return;
	}
}

void Soldier::DynamicResolveNoCollision()
{
	if (this->_state && this->_state->IsDead())
	{
		this->_surfaceEntity = nullptr;
		this->_isAbSurface = false;
		return;
	}

	if (this->_isAbSurface)
	{
		if (this->_surfaceEntity)
		{
			if (this->GetL() > this->_surfaceEntity->GetR()
			||  this->GetR() < this->_surfaceEntity->GetL())
			{
				if (this->_state && !this->_state->IsJumping())
				{
					this->_isAbSurface = false; this->_surfaceEntity = nullptr;
					auto currentY = this->_position.y; ChangeState(this->_state, new SoldierJumpState(), this); this->_position.y = currentY;
				}
			}
		}
	}
}

void Soldier::DynamicResolveOnCollision(AABBSweepResult aabbSweepResult)
{
	if (!aabbSweepResult.surfaceEntity)
		return;

	TERRAIN_BLOCK_TYPE terrainType = aabbSweepResult.surfaceEntity->GetTerrainType();
	if (terrainType != TERRAIN_BLOCK_TYPE::NONE)
	switch (terrainType)
	{

	case TERRAIN_BLOCK_TYPE::WALL:
	{
		if (aabbSweepResult.normalY != +0.0f)
		{
			this->_position.y += aabbSweepResult.enTime * this->_vy;
			this->_isAbSurface = false;
			this->_surfaceEntity = nullptr;
			if (this->_state && !this->_state->IsDead())
			{
				ChangeState(this->_state, new SoldierDieState(), this);
			}
			else
			{
				this->_vy = +1.0f;
			}
		}
		else
		if (aabbSweepResult.normalX != +0.0f)
		{
			std::string wallName = aabbSweepResult.surfaceEntity->GetEntityName();
			if ((wallName == "L" && aabbSweepResult.normalX == +1.0f)
			||  (wallName == "R" && aabbSweepResult.normalX == -1.0f))
			{
				this->_position.x += aabbSweepResult.enTime * this->_vx;
				if (this->_state && !this->_state->IsDead())
				{
					if (aabbSweepResult.normalX == -1.0f)
					{
						this->_movingDirection = DIRECTION::LEFT;
					}
					else
					if (aabbSweepResult.normalX == +1.0f)
					{
						this->_movingDirection = DIRECTION::RIGHT;
					}
				}
			}
		}
		else
		{
		}
		return;
	}
	break;

	case TERRAIN_BLOCK_TYPE::WATER:
	{
		if (this->_state && this->_state->IsDead())
			return;

		if (aabbSweepResult.normalY == +1.0f)
		{
			this->_position.y += aabbSweepResult.enTime * this->_vy;
			this->_isAbSurface = false;
			this->_surfaceEntity = nullptr;
			ChangeState(this->_state, new SoldierDieState(), this);
			this->SetDrown(true);
		}
		else
		{
		}
		return;
	}
	break;

	case TERRAIN_BLOCK_TYPE::THROUGHABLE:
	{
		if (this->_state && this->_state->IsDead())
			return;

		if (aabbSweepResult.normalY == +1.0f)
		{
			if (this->_state && this->_state->IsJumping())
			{
				if (aabbSweepResult.surfaceEntity->GetY() > this->_position.y)
					return;
			}
			if (this->_surfaceEntity)
			{
				if (std::abs(aabbSweepResult.surfaceEntity->GetY() - this->_surfaceEntity->GetY()) > Constants::Enemies::Soldier::MAX_STEP_HEIGHT)
					return;
			}
			this->_position.y += aabbSweepResult.enTime * this->_vy;
			this->_isAbSurface = true;
			this->_surfaceEntity = aabbSweepResult.surfaceEntity;
			ChangeState(this->_state, new SoldierRunState(), this);
		}
		else
		{
		}
		return;
	}
	break;

	case TERRAIN_BLOCK_TYPE::NON_THROUGHABLE:
	{
		if (this->_state && this->_state->IsDead())
			return;

		if (aabbSweepResult.normalY == +1.0f)
		{
			this->_position.y += aabbSweepResult.enTime * this->_vy;
			this->_isAbSurface = true;
			this->_surfaceEntity = aabbSweepResult.surfaceEntity;
			ChangeState(this->_state, new SoldierRunState(), this);
		}
		else
		{
		}
		return;
	}
	break;

	}
	else
	{
		if (this->_state && this->_state->IsDead())
		{
			this->_surfaceEntity = nullptr;
			this->_isAbSurface = false;
			return;
		}

		if (aabbSweepResult.surfaceEntity->IsBridge())
		{
			if (aabbSweepResult.normalY == +1.0f)
			{
				if (this->_surfaceEntity)
				{
					if (std::abs(aabbSweepResult.surfaceEntity->GetY() - this->_surfaceEntity->GetY()) > Constants::Enemies::Soldier::MAX_STEP_HEIGHT)
						return;
				}
				this->_position.y += aabbSweepResult.enTime * this->_vy;
				this->_isAbSurface = true;
				this->_surfaceEntity = aabbSweepResult.surfaceEntity;
				ChangeState(this->_state, new SoldierRunState(), this);
			}
			return;
		}
	}
}

Soldier::~Soldier()
{
	Destroy(this->_state);
	Destroy(this->_updateState);
	Destroy(this->_handleInputState);
}

void Soldier::Update()
{
	if (this->_state && !this->_state->IsDead() && !this->_state->IsJumping() && this->_target)
	{
		float dx = +(this->_position.x - this->_target->GetX());
		float dy = -(this->_position.y - this->_target->GetY());

		float targetAngle = 0.0f;

		if (dx > 0.0f && dy < 0.0f)
		{
			targetAngle = D3DXToDegree(-std::atan(std::abs(dx) / std::abs(dy)));
		}
		else
		if (dx < 0.0f && dy < 0.0f)
		{
			targetAngle = D3DXToDegree(+std::atan(std::abs(dx) / std::abs(dy)));
		}
		else
		if (dx > 0.0f && dy > 0.0f)
		{
			targetAngle = D3DXToDegree(+std::atan(std::abs(dx) / dy)) - Constants::Physics::HALF_CIRCLE_DEGREES;
		}
		else
		if (dx < 0.0f && dy > 0.0f)
		{
			targetAngle = D3DXToDegree(-std::atan(std::abs(dx) / dy)) + Constants::Physics::HALF_CIRCLE_DEGREES;
		}

		if (std::abs(this->_target->GetX() - this->_position.x) >= Constants::Enemies::Soldier::DETECT_DISTANCE)
		{
			if (targetAngle < 0.0f)
			{
				this->_movingDirection = DIRECTION::LEFT;
			}
			else
			if (targetAngle > 0.0f)
			{
				this->_movingDirection = DIRECTION::RIGHT;
			}
			if (this->_shootable)
			{
				if ((targetAngle <= +Constants::Enemies::Soldier::SHOOT_ANGLE_TOLERANCE_MAX_DEGREES && targetAngle >= +Constants::Enemies::Soldier::SHOOT_ANGLE_TOLERANCE_MIN_DEGREES)
				||  (targetAngle >= -Constants::Enemies::Soldier::SHOOT_ANGLE_TOLERANCE_MAX_DEGREES && targetAngle <= -Constants::Enemies::Soldier::SHOOT_ANGLE_TOLERANCE_MIN_DEGREES))
				{
					if (!this->_state->IsShooting())
					{
						ChangeState(this->_state, new SoldierShootState(), this);
					}
				}
				else
				if (this->_state->IsShooting())
				{
					ChangeState(this->_state, new SoldierRunState(), this);
				}
			}
		}
		else
		if (std::abs(this->_target->GetX() - this->_position.x) <= Constants::Enemies::Soldier::DETECT_DISTANCE)
		{
			if (this->_shootable)
			{
				if ((targetAngle <= +Constants::Enemies::Soldier::SHOOT_ANGLE_TOLERANCE_MAX_DEGREES && targetAngle >= +Constants::Enemies::Soldier::SHOOT_ANGLE_TOLERANCE_MIN_DEGREES)
				||  (targetAngle >= -Constants::Enemies::Soldier::SHOOT_ANGLE_TOLERANCE_MAX_DEGREES && targetAngle <= -Constants::Enemies::Soldier::SHOOT_ANGLE_TOLERANCE_MIN_DEGREES))
				{
					if (targetAngle < 0.0f)
					{
						this->_movingDirection = DIRECTION::LEFT;
					}
					else
					if (targetAngle > 0.0f)
					{
						this->_movingDirection = DIRECTION::RIGHT;
					}
					if (!this->_state->IsLayingDown())
					{
						ChangeState(this->_state, new SoldierLayDownState(), this);
					}
				}
				else
				if (this->_state->IsLayingDown())
				{
					ChangeState(this->_state, new SoldierRunState(), this);
				}
			}
		}
	}
	DeferState(this->_updateState, this->_state->Update(*this));

	ApplyDeferredState(this->_state, this->_updateState, this);
	ApplyDeferredState(this->_state, this->_handleInputState, this);
}

void Soldier::Render()
{
	this->_state->Render(*this);
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

void Soldier::HandleInput(Input& input)
{
	DeferState(this->_handleInputState, this->_state->HandleInput(*this, input));
}

void InsertSpriteSoldier(SPRITE_ID spriteId, int left, int top, int right, int bottom)
{
	// i write this function to shorten the function: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::LEFT, SOLDIER_TEXTURE_ID::SOLDIER_01);
}

void Soldier::LoadSprites()
{
	if (HasSprites<Soldier>::_hasBeenLoaded) return;
	HasSprites<Soldier>::_hasBeenLoaded = true;

#pragma region Load Sprites

	// SPRITES
	InsertSpriteSoldier(SOLDIER_SPRITE_ID::RUN_01, 90, 0, 106, 32);
	InsertSpriteSoldier(SOLDIER_SPRITE_ID::RUN_02, 72, 0, 88, 32);
	InsertSpriteSoldier(SOLDIER_SPRITE_ID::RUN_03, 54, 0, 70, 32);
	InsertSpriteSoldier(SOLDIER_SPRITE_ID::RUN_04, 36, 0, 52, 32);
	InsertSpriteSoldier(SOLDIER_SPRITE_ID::RUN_05, 18, 0, 34, 32);
	InsertSpriteSoldier(SOLDIER_SPRITE_ID::RUN_06, 0, 0, 16, 32);

	InsertSpriteSoldier(SOLDIER_SPRITE_ID::JUMP_01, 108, 0, 124, 32);

	InsertSpriteSoldier(SOLDIER_SPRITE_ID::SHOOT_01, 126, 0, 150, 32);
	InsertSpriteSoldier(SOLDIER_SPRITE_ID::SHOOT_02, 152, 0, 176, 32);

	InsertSpriteSoldier(SOLDIER_SPRITE_ID::LAY_DOWN_01, 178, 0, 210, 32);

	InsertSpriteSoldier(SOLDIER_SPRITE_ID::DIE_01, 212, 0, 227, 32);

#pragma endregion Load Sprites

	OutputDebugString(L"Soldier Sprites Loaded Successfully\n");
}

void Soldier::LoadTextures()
{
	if (HasTextures<Soldier>::_hasBeenLoaded) return;
	HasTextures<Soldier>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(SOLDIER_TEXTURE_ID::SOLDIER_01, L"Resources\\Textures\\Soldier.bmp");

	OutputDebugString(L"Soldier Textures Loaded Successfully\n");
}

void Soldier::LoadAnimations()
{
	if (HasAnimations<Soldier>::_hasBeenLoaded) return;
	HasAnimations<Soldier>::_hasBeenLoaded = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(SOLDIER_ANIMATION_ID::RUN, Constants::Enemies::Soldier::ANIMATION_RUN_DELAY_MILLISECONDS,
		{
			{SOLDIER_SPRITE_ID::RUN_01,0},
			{SOLDIER_SPRITE_ID::RUN_02,0},
			{SOLDIER_SPRITE_ID::RUN_03,0},
			{SOLDIER_SPRITE_ID::RUN_04,0},
			{SOLDIER_SPRITE_ID::RUN_05,0},
			{SOLDIER_SPRITE_ID::RUN_06,0},
		});
	GraphicsHelper::InsertAnimation(SOLDIER_ANIMATION_ID::JUMP, Constants::Enemies::Soldier::ANIMATION_JUMP_DELAY_MILLISECONDS,
		{
			{SOLDIER_SPRITE_ID::JUMP_01,0},
		});
	GraphicsHelper::InsertAnimation(SOLDIER_ANIMATION_ID::SHOOT, Constants::Enemies::Soldier::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{SOLDIER_SPRITE_ID::SHOOT_01,0},
			{SOLDIER_SPRITE_ID::SHOOT_02,0},
		});
	GraphicsHelper::InsertAnimation(SOLDIER_ANIMATION_ID::LAY_DOWN, Constants::Enemies::Soldier::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{SOLDIER_SPRITE_ID::LAY_DOWN_01,0},
		});
	GraphicsHelper::InsertAnimation(SOLDIER_ANIMATION_ID::DIE, Constants::Enemies::Soldier::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
		{
			{SOLDIER_SPRITE_ID::DIE_01,0},
		});

#pragma endregion Load Animations

	OutputDebugString(L"Soldier Animations Loaded Successfully\n");
}

SoldierState* Soldier::GetState()
{
	return this->_state;
}

void Soldier::SetState(SoldierState* state)
{
	ChangeState(this->_state, state, this);
}

void Soldier::GoDead()
{
	if (!this->_state || !this->_state->IsDead())
	{
		ChangeState(this->_state, new SoldierDieState(), this);
	}
}

bool Soldier::TakeBulletHit()
{
	if (--this->_hitCounts == 0)
	{
		this->GoDead();
	}
	return true;
}

bool Soldier::IsLethalToTouch() const
{
	return this->_state ? !this->_state->IsDead() : true;
}
