#include "Bill.h"
#include "Item.h"
#include "Bridge.h"
#include "Falcon.h"
#include "RockFly.h"
#include "Soldier.h"
#include "AirCraft.h"
#include "BossStage3.h"
#include "TerrainBlock.h"
#include "GunBossStage1.h"
#include "BossStage3Gate.h"
#include "BossStage3Hand.h"
#include "FinalBossStage1.h"
#include "BossStage3Joint.h"

Bill::Bill() : Entity(), HasTextures(), HasSprites(), HasAnimations(), CollidableEntity(), HasWeapons(new BulletRState()), _livesLeft(nullptr)
{
	CollidableEntity::_self = this;

	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_angle = 0;
	this->_position.x = Constants::Bill::DEFAULT_SPAWN_X;
	this->_position.y = Constants::Bill::DEFAULT_SPAWN_Y;

	this->_state = nullptr;
	this->_updateState = nullptr;
	this->_handleInputState = nullptr;

	//
	this->SetDebugName(L"Bill\n");
	//

	this->_immortalTime = Constants::Bill::IMMORTAL_DURATION_FRAMES;
	this->_immortalTick = 000;

	if (!this->_state)
	{
		 this->_state = new BillBeginState();
		 this->_state->Enter(*this);
	}
}

Bill::~Bill()
{
	Destroy(this->_state);
	Destroy(this->_updateState);
	Destroy(this->_handleInputState);
}

void Bill::GoDead()
{
	if (this->_immortalTick > this->_immortalTime)
	{
		if (this->_state && !this->_state->IsDead() && !this->_state->IsInvulnerable())
		{
			ChangeState(this->_state, new BillDeadState(), this);
		}
	}
}

void Bill::Update()
{
	DeferState(this->_updateState, this->_state->Update(*this));

	if  (this->_immortalTick <= this->_immortalTime)
	   ++this->_immortalTick;

	// Last, so that BillBeginState::Enter's reset of _immortalTick is not spent by
	// the increment above in the same step.
	ApplyDeferredState(this->_state, this->_updateState, this);
	ApplyDeferredState(this->_state, this->_handleInputState, this);
}

void Bill::Render()
{
	if  (this->_immortalTick <= this->_immortalTime)
	{
	if  (this->_immortalTick %2)
	{
		 this->_state->Render(*this);
	}
	}
	else
	{
		 this->_state->Render(*this);
	}
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

//int i = 1;
void Bill::HandleInput(Input& input)
{
	DeferState(this->_handleInputState, this->_state->HandleInput(*this, input));
}

void Bill::LoadSprites()
{
	if (HasSprites<Bill>::_hasBeenLoaded) return;
	HasSprites<Bill>::_hasBeenLoaded = true;

#pragma region Load Sprites

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_01,  66,  2, 23,102, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_02,  66, 24, 41,102, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_03,  66, 42, 61,102, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_04,  66, 62, 83,102, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_05,  66, 84,101,102, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_06,  66,102,122,102, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::JUMP_01,  44,117,134, 65, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::JUMP_02,  44,135,155, 65, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::JUMP_03,  44,156,173, 65, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::JUMP_04,  44,174,194, 65, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::DEAD_01, 107,  2, 18,131, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::DEAD_02, 112, 19, 42,131, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::DEAD_03, 107, 43, 59,131, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::DEAD_04, 120, 60, 93,131, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::DIVE_01, 123,112,129,131, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::DIVE_02, 123,112,129,129, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::FALL_01,  66, 42, 61,102, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::BEGIN_01,  44,117,134, 65, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::BEGIN_02,  44,135,155, 65, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::BEGIN_03,  44,156,173, 65, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::BEGIN_04,  44,174,194, 65, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::NORMAL_01,  30,  1, 25, 65, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::LAYDOWN_01,  48, 83,116, 65, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SWIM_RUN_01, 115,130,147,131, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SWIM_RUN_02, 115,130,147,129, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_01, 189,155,181,224, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_02, 189,184,208,224, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_03, 189,210,236,224, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SWIM_SHOT_01, 113,191,217,131, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SWIM_SHOT_02, 113,191,217,129, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::BEGIN_SWIM_01, 115, 94,111,131, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SWIM_NORMAL_01, 115,130,147,131, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SWIM_NORMAL_02, 115,130,147,129, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::NORMAL_SHOT_01,  30,  1, 25, 65, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::NORMAL_SHOT_02,  30, 26, 50, 65, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::STRAIGHT_UP_01,  19, 51, 66, 65, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SHOT_STRAIGHT_UP_01,  19, 51, 66, 65, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SHOT_STRAIGHT_UP_02,  19, 67, 82, 65, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_ANGLE_UP_01,  66,123,144,102, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_ANGLE_UP_02,  66,145,162,102, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_ANGLE_UP_03,  66,163,183,102, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SWIM_SHOT_ANGLE_UP_01, 113,148,168,131, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SWIM_SHOT_ANGLE_UP_02, 113,148,168,129, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_ANGLE_DOWN_01,  66,184,206,102, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_ANGLE_DOWN_02,  66,207,228,102, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_ANGLE_DOWN_03,  66,229,251,102, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SWIM_SHOT_STRAIGHT_UP_01, 103,169,187,131, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SWIM_SHOT_STRAIGHT_UP_02, 103,169,187,129, DIRECTION::RIGHT, BILL_TEXTURE_ID::BILL_01);

#pragma endregion Load Sprites

	OutputDebugString(L"Bill Sprites Loaded Successfully\n");
}

void Bill::LoadTextures()
{
	if (HasTextures<Bill>::_hasBeenLoaded) return;
	HasTextures<Bill>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(BILL_TEXTURE_ID::BILL_01, L"Resources\\Textures\\BillAndLance.bmp");

	OutputDebugString(L"Bill Textures Loaded Successfully\n");
}

void Bill::LoadAnimations()
{
	if (HasAnimations<Bill>::_hasBeenLoaded) return;
	HasAnimations<Bill>::_hasBeenLoaded = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::RUN, Constants::Bill::ANIMATION_RUN_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::RUN_01,0},
		{BILL_SPRITE_ID::RUN_02,0},
		{BILL_SPRITE_ID::RUN_03,0},
		{BILL_SPRITE_ID::RUN_04,0},
		{BILL_SPRITE_ID::RUN_05,0},
		{BILL_SPRITE_ID::RUN_06,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::JUMP, Constants::Bill::ANIMATION_JUMP_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::JUMP_01,0},
		{BILL_SPRITE_ID::JUMP_02,0},
		{BILL_SPRITE_ID::JUMP_03,0},
		{BILL_SPRITE_ID::JUMP_04,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::DEAD, Constants::Bill::ANIMATION_DEAD_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::DEAD_01,0},
		{BILL_SPRITE_ID::DEAD_02,0},
		{BILL_SPRITE_ID::DEAD_03,0},
		{BILL_SPRITE_ID::DEAD_04,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::DIVE, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::DIVE_01,0},
		{BILL_SPRITE_ID::DIVE_02,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::FALL, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::FALL_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::BEGIN, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::BEGIN_01,0},
		{BILL_SPRITE_ID::BEGIN_02,0},
		{BILL_SPRITE_ID::BEGIN_03,0},
		{BILL_SPRITE_ID::BEGIN_04,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::NORMAL, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::NORMAL_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::LAYDOWN, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::LAYDOWN_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SWIM_RUN, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::SWIM_RUN_01,0},
		{BILL_SPRITE_ID::SWIM_RUN_02,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::RUN_SHOT, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::RUN_SHOT_01,0},
		{BILL_SPRITE_ID::RUN_SHOT_02,0},
		{BILL_SPRITE_ID::RUN_SHOT_03,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SWIM_SHOT, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::SWIM_SHOT_01,0},
		{BILL_SPRITE_ID::SWIM_SHOT_02,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::BEGIN_SWIM, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::BEGIN_SWIM_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SWIM_NORMAL, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::SWIM_NORMAL_01,0},
		{BILL_SPRITE_ID::SWIM_NORMAL_02,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::NORMAL_SHOT, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::NORMAL_SHOT_01,0},
		{BILL_SPRITE_ID::NORMAL_SHOT_02,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::STRAIGHT_UP, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::STRAIGHT_UP_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SHOT_STRAIGHT_UP, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::SHOT_STRAIGHT_UP_01,0},
		{BILL_SPRITE_ID::SHOT_STRAIGHT_UP_02,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::RUN_SHOT_ANGLE_UP, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::RUN_SHOT_ANGLE_UP_01,0},
		{BILL_SPRITE_ID::RUN_SHOT_ANGLE_UP_02,0},
		{BILL_SPRITE_ID::RUN_SHOT_ANGLE_UP_03,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SWIM_SHOT_ANGLE_UP, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::SWIM_SHOT_ANGLE_UP_01,0},
		{BILL_SPRITE_ID::SWIM_SHOT_ANGLE_UP_02,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::RUN_SHOT_ANGLE_DOWN, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::RUN_SHOT_ANGLE_DOWN_01,0},
		{BILL_SPRITE_ID::RUN_SHOT_ANGLE_DOWN_02,0},
		{BILL_SPRITE_ID::RUN_SHOT_ANGLE_DOWN_03,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SWIM_SHOT_STRAIGHT_UP, Constants::Bill::ANIMATION_DEFAULT_DELAY_MILLISECONDS,
	{
		{BILL_SPRITE_ID::SWIM_SHOT_STRAIGHT_UP_01,0},
		{BILL_SPRITE_ID::SWIM_SHOT_STRAIGHT_UP_02,0},
	});

#pragma endregion Load Animations

	OutputDebugString(L"Bill Animations Loaded Successfully\n");
}

void Bill::Fire()
{
	if (this->_state)
	{
		auto params = this->_state->GetBulletSpawnParams(*this);
		if (params.has_value())
		{
			this->HasWeapons::Fire(*params);
		}
	}
}

void Bill::CollectItem(ITEM_TYPE type)
{
	switch (type)
	{
	case ITEM_TYPE::B:
		Sound::GetInstance()->Play("weaponB", false, 1);
		break;
	case ITEM_TYPE::F:
		Sound::GetInstance()->Play("weaponF", false, 1);
		SetBulletState(new BulletFState);
		break;
	case ITEM_TYPE::L:
		Sound::GetInstance()->Play("weaponL", false, 1);
		SetBulletState(new BulletLState);
		break;
	case ITEM_TYPE::M:
		Sound::GetInstance()->Play("weaponM", false, 1);
		SetBulletState(new BulletMState);
		break;
	case ITEM_TYPE::R:
		Sound::GetInstance()->Play("weaponR", false, 1);
		SetBulletState(new BulletRState);
		break;
	case ITEM_TYPE::S:
		Sound::GetInstance()->Play("weaponS", false, 1);
		SetBulletState(new BulletSState);
		break;
	case ITEM_TYPE::I:
		Sound::GetInstance()->Play("weaponD", false, 1);
		break;
	}
}

void Bill::StaticResolveNoCollision(                               )
{
	if (this->_state && this->_state->IsDead())
	{
		this->_surfaceEntity = nullptr;
		this->_isAbSurface = false;
		return;
	}
}

void Bill::StaticResolveOnCollision(AABBSweepResult aabbSweepResult)
{
	if (this->_state && this->_state->IsDead())
	{
		this->_surfaceEntity = nullptr;
		this->_isAbSurface = false;
		return;
	}
}

void Bill::DynamicResolveNoCollision(                               )
{
	if (this->_state && this->_state->IsDead())
	{
		this->_surfaceEntity = nullptr;
		this->_isAbSurface = false;
		return;
	}

	// The guard above only proves _state is non-null on the branch it returns
	// from; falling through it also covers the _state == nullptr case, which the two
	// _state-> calls below then dereferenced.
	if (this->_isAbSurface && this->_state)
	{
		if (this->_surfaceEntity)
		{
			if (this->_surfaceEntity->RidesWithSurface()
			&&  this->_state->IsNormal())
			{
				this->_position.x = this->_surfaceEntity->GetX();
			}
			else
			if (this->GetL() > this->_surfaceEntity->GetR()
			||  this->GetR() < this->_surfaceEntity->GetL())
			{
				if (!this->_state->IsJumping())
				{
					this->_isAbSurface = false; this->_surfaceEntity = nullptr;
					auto currentY = this->_position.y; ChangeState(this->_state, new BillFallState(new BillNormalState()), this); this->_position.y = currentY;
				}
			}
		}
	}
}

void Bill::DynamicResolveOnCollision(AABBSweepResult aabbSweepResult)
{
	if (!aabbSweepResult.surfaceEntity)
		return;

	TERRAIN_BLOCK_TYPE terrainType = aabbSweepResult.surfaceEntity->GetTerrainType();
	if (terrainType != TERRAIN_BLOCK_TYPE::NONE)
	switch (terrainType)
	{

	case TERRAIN_BLOCK_TYPE::WALL:
	{
		if (aabbSweepResult.normalX != +0.0f)
		{
			this->_position.x += aabbSweepResult.enTime * this->_vx;
		}
		else
		if (aabbSweepResult.normalY != +0.0f)
		{
			this->_position.y += aabbSweepResult.enTime * this->_vy;
			this->_isAbSurface = false;
			this->_surfaceEntity = nullptr;
			if (!this->_state->IsDead())
			{
				ChangeState(this->_state, new BillDeadState(), this);
			}
			else
			{
				this->_vy = Constants::Bill::WALL_DEAD_DRIFT_VELOCITY_Y;
			}
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
			this->_isAbSurface = true;
			this->_surfaceEntity = aabbSweepResult.surfaceEntity;
			ChangeState(this->_state, new BillBeginSwimState(), this);
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
			if (this->_state && this->_state->IsFalling())
			{
				if (aabbSweepResult.surfaceEntity->GetY() > this->_position.y)
					return;
			}
			if (this->_surfaceEntity)
			{
				if (std::abs(aabbSweepResult.surfaceEntity->GetY() - this->_surfaceEntity->GetY()) > Constants::Bill::MAX_STEP_HEIGHT)
					return;
			}
			this->_position.y += aabbSweepResult.enTime * this->_vy;
			this->_isAbSurface = true;
			this->_surfaceEntity = aabbSweepResult.surfaceEntity;
			ChangeState(this->_state, new BillNormalState(), this);
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
			ChangeState(this->_state, new BillNormalState(), this);
		}
		else
		if (aabbSweepResult.normalX != +0.0f)
		{
			ChangeState(this->_state, new BillNormalState(), this);
			this->_position.y = aabbSweepResult.surfaceEntity->GetT();
			this->_vy = Constants::Bill::LEDGE_SLIDE_VELOCITY_Y;
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

		if (this->_state && (this->_state->IsInvulnerable() || this->_state->IsBeginning()))
		{
			return;
		}

		if (aabbSweepResult.surfaceEntity->OnBillCollision(*this, aabbSweepResult))
		{
			return;
		}

		if (aabbSweepResult.surfaceEntity->IsWalkableSurface())
		{
			if (aabbSweepResult.normalY == +1.0f)
			{
				if (this->_surfaceEntity)
				{
					if (std::abs(aabbSweepResult.surfaceEntity->GetY() - this->_surfaceEntity->GetY()) > Constants::Bill::MAX_STEP_HEIGHT)
						return;
				}
				this->_position.y += aabbSweepResult.enTime * this->_vy;
				this->_isAbSurface = true;
				this->_surfaceEntity = aabbSweepResult.surfaceEntity;
				ChangeState(this->_state, new BillNormalState(), this);
			}
			return;
		}

		if (aabbSweepResult.surfaceEntity->IsPushableObstacle())
		{
			this->_position.x += aabbSweepResult.enTime * this->_vx;
			return;
		}

		if (aabbSweepResult.surfaceEntity->IsEnemy())
		{
			if (this->_immortalTick <= this->_immortalTime)
			{
				return;
			}

			if (aabbSweepResult.surfaceEntity->IsLethalToTouch())
			{
				ChangeState(this->_state, new BillDeadState(), this);
			}
			return;
		}
	}
}
