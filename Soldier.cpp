#include "Bridge.h"
#include "Soldier.h"
#include "TerrainBlock.h"

Soldier::Soldier() : Entity(), HasTextures(), HasSprites(), HasAnimations(), CollidableEntity(), HasWeapons(new BulletEnemyState())
{
	this->vx = +1.0f;
	this->vy = -1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = 100;
	this->position.y = 0;
	this->name = L"Soldier\n";

	this->updateState = NULL;
	this->handleInputState = NULL;
	// set direction default is right
	this->movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	this->state = new SoldierRunState();

	shootable  = 0;
	firingRate = 2000;
	CollidableEntity::self = this;

	this->hitCounts = 1;
	this->enemyType = ENEMY_TYPE::HUMAN;
}

void Soldier::Fire()
{
	FLOAT coefficient = dynamic_cast<SoldierShootState*>(state) ? 0.75f : dynamic_cast<SoldierLayDownState*>(state) ? 0.20f : 0.00f;
	if (movingDirection == DIRECTION::LEFT)
	{
		HasWeapons::Fire(GetL(), position.y + h * coefficient, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, movingDirection);
	}
	else 
	if (movingDirection == DIRECTION::RIGHT)
	{
		HasWeapons::Fire(GetR(), position.y + h * coefficient, 0.0f, +1.0f, 0.0f, 0.0f, 0.0f, movingDirection);
	}
}

void Soldier::StaticResolveNoCollision()
{
	if (dynamic_cast<SoldierDieState*>(state))
	{
		surfaceEntity = NULL;
		isAbSurface = 0;
		return;
	}
}

void Soldier::StaticResolveOnCollision(AABBSweepResult aabbSweepResult)
{
	if (dynamic_cast<SoldierDieState*>(state))
	{
		surfaceEntity = NULL;
		isAbSurface = 0;
		return;
	}
}

void Soldier::DynamicResolveNoCollision()
{
	if (dynamic_cast<SoldierDieState*>(state))
	{
		surfaceEntity = NULL;
		isAbSurface = 0;
		return;
	}
	
	if (isAbSurface)
	{
		if (surfaceEntity)
		{
			if (this->GetL() > surfaceEntity->GetR()
			||  this->GetR() < surfaceEntity->GetL())
			{
				if (!dynamic_cast<SoldierJumpState*>(state))
				{
					isAbSurface = 0; surfaceEntity = NULL;
					auto currentY = position.y; ChangeState(state, new SoldierJumpState(), this); position.y = currentY;
				}
			}
		}
	}
}

void Soldier::DynamicResolveOnCollision(AABBSweepResult aabbSweepResult)
{
	auto    terrainBlock  = dynamic_cast<TerrainBlock*>(aabbSweepResult.surfaceEntity);
	if     (terrainBlock)
	switch (terrainBlock->type)
	{

	case TERRAIN_BLOCK_TYPE::WALL:
	{
		if (aabbSweepResult.normalY != +0.0f)
		{
			position.y += aabbSweepResult.enTime * vy;
			isAbSurface = 0;
			surfaceEntity = NULL;
			if (!dynamic_cast<SoldierDieState*>(state))
			{
				ChangeState(state, new SoldierDieState(), this);
			}
			else
			{
				vy = +0.0f;
			}
		}
		else
		if (aabbSweepResult.normalX != +0.0f)
		{
			position.x += aabbSweepResult.enTime * vx;
			if (!dynamic_cast<SoldierDieState*>(state))
			{
				if (aabbSweepResult.normalX == -1.0f)
				{
					movingDirection = DIRECTION::LEFT;
				}
				else
				if (aabbSweepResult.normalX == +1.0f)
				{
					movingDirection = DIRECTION::RIGHT;
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
		if (dynamic_cast<SoldierDieState*>(state))
			return;

		if (aabbSweepResult.normalY == +1.0f)
		{
			position.y += aabbSweepResult.enTime * vy;
			isAbSurface = 0;
			surfaceEntity = NULL;
			ChangeState(state, new SoldierDieState(), this);
		}
		else
		{
		}
		return;
	}
	break;

	case TERRAIN_BLOCK_TYPE::THROUGHABLE:
	{
		if (dynamic_cast<SoldierDieState*>(state))
			return;

		if (aabbSweepResult.normalY == +1.0f)
		{
			if (dynamic_cast<SoldierJumpState*>(state))
			{
				if (terrainBlock->GetY() > position.y)
					return;
			}
			if (surfaceEntity)
			{
				if (abs(terrainBlock->GetY() - surfaceEntity->GetY() > 48.0f)) // size of 1 tile is 16 x 16 -> 48.0f = 3 tiles
					return;
			}
			position.y += aabbSweepResult.enTime * vy;
			isAbSurface = 1;
			surfaceEntity = terrainBlock;
			ChangeState(state, new SoldierRunState(), this);
		}
		else
		{
		}
		return;
	}
	break;

	case TERRAIN_BLOCK_TYPE::NON_THROUGHABLE:
	{
		if (dynamic_cast<SoldierDieState*>(state))
			return;

		if (aabbSweepResult.normalY == +1.0f)
		{
			position.y += aabbSweepResult.enTime * vy;
			isAbSurface = 1;
			surfaceEntity = terrainBlock;
			ChangeState(state, new SoldierRunState(), this);
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
		if (dynamic_cast<SoldierDieState*>(state))
		{
			surfaceEntity = NULL;
			isAbSurface = 0;
			return;
		}

		auto bridge = dynamic_cast<Bridge*>(aabbSweepResult.surfaceEntity);
		if  (bridge)
		{
			if (aabbSweepResult.normalY == +1.0f)
			{
				if (surfaceEntity)
				{
					if (abs(bridge->GetY() - surfaceEntity->GetY() > 48.0f)) // size of 1 tile is 16 x 16 -> 48.0f = 3 tiles
						return;
				}
				position.y += aabbSweepResult.enTime * vy;
				isAbSurface = 1;
				surfaceEntity = bridge;
				ChangeState(state, new SoldierRunState(), this);
			}
			return;
		}
	}
}

Soldier::~Soldier()
{
	Destroy(state);
	Destroy(updateState);
	Destroy(handleInputState);
}

void Soldier::Update()
{
	if (!dynamic_cast<SoldierDieState*>(state) && !dynamic_cast<SoldierJumpState*>(state))
	{
		FLOAT dx = +(position.x - target->GetX());
		FLOAT dy = -(position.y - target->GetY());

		FLOAT targetAngle = 0.0f;

		if (dx > 0.0f && dy < 0.0f)
		{
			targetAngle = -atan(abs(dx) / abs(dy)) * 180.0f / D3DX_PI;
		}
		else
		if (dx < 0.0f && dy < 0.0f)
		{
			targetAngle = +atan(abs(dx) / abs(dy)) * 180.0f / D3DX_PI;
		}
		else
		if (dx > 0.0f && dy > 0.0f)
		{
			targetAngle = +atan(abs(dx) / dy) * 180.0f / D3DX_PI - 180.0f;
		}
		else
		if (dx < 0.0f && dy > 0.0f)
		{
			targetAngle = -atan(abs(dx) / dy) * 180.0f / D3DX_PI + 180.0f;
		}

		if (abs(target->GetX() - position.x) >= 64.0f)
		{
			if (targetAngle < 0.0f)
			{
				movingDirection = DIRECTION::LEFT;
			}
			else
			if (targetAngle > 0.0f)
			{
				movingDirection = DIRECTION::RIGHT;
			}
			if (shootable)
			{
				if ((targetAngle <= +95.0f && targetAngle >= +85.0f)
				||  (targetAngle >= -95.0f && targetAngle <= -85.0f))
				{
					if (!dynamic_cast<SoldierShootState*>(state))
					{
						ChangeState(state, new SoldierShootState(), this);
					}
				}
				else 
				if (dynamic_cast<SoldierShootState*>(state))
				{
					ChangeState(state, new SoldierRunState(), this);
				}
			}
		}
		else
		if (abs(target->GetX() - position.x) <= 64.0f)
		{
			if (shootable)
			{
				if ((targetAngle <= +95.0f && targetAngle >= +85.0f)
				||  (targetAngle >= -95.0f && targetAngle <= -85.0f))
				{
					if (targetAngle < 0.0f)
					{
						movingDirection = DIRECTION::LEFT;
					}
					else
					if (targetAngle > 0.0f)
					{
						movingDirection = DIRECTION::RIGHT;
					}
					if (!dynamic_cast<SoldierLayDownState*>(state))
					{
						ChangeState(state, new SoldierLayDownState(), this);
					}
				}
				else
				if (dynamic_cast<SoldierLayDownState*>(state))
				{
					ChangeState(state, new SoldierRunState(), this);
				}
			}
		}
	}
	if (isDead) Sound::getInstance()->play("qexplode", false, 1);
	updateState = state->Update(*this);
}

void Soldier::Render()
{
	state->Render(*this);
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;

	if (updateState)
	{
		ChangeState(state, updateState, this);
		updateState = NULL;
	}
	if (handleInputState)
	{
		ChangeState(state, handleInputState, this);
		handleInputState = NULL;
	}
}

void Soldier::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void InsertSpriteSoldier(SPRITE_ID spriteId, INT left, INT top, INT right, INT bottom)
{
	// i write this function to shorten the fuction: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::LEFT, SOLDIER_TEXTURE_ID::SOLDIER_01);
}

void Soldier::LoadSprites()
{
	if (HasSprites<Soldier>::hasBeenLoaded.value) return;
	HasSprites<Soldier>::hasBeenLoaded.value = true;

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
	if (HasTextures<Soldier>::hasBeenLoaded.value) return;
	HasTextures<Soldier>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(SOLDIER_TEXTURE_ID::SOLDIER_01, L"Resources\\Textures\\Soldier.bmp");

	OutputDebugString(L"Soldier Textures Loaded Successfully\n");
}

void Soldier::LoadAnimations()
{
	if (HasAnimations<Soldier>::hasBeenLoaded.value) return;
	HasAnimations<Soldier>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(SOLDIER_ANIMATION_ID::RUN, 70,
		{
			{SOLDIER_SPRITE_ID::RUN_01,0},
			{SOLDIER_SPRITE_ID::RUN_02,0},
			{SOLDIER_SPRITE_ID::RUN_03,0},
			{SOLDIER_SPRITE_ID::RUN_04,0},
			{SOLDIER_SPRITE_ID::RUN_05,0},
			{SOLDIER_SPRITE_ID::RUN_06,0},
		});
	GraphicsHelper::InsertAnimation(SOLDIER_ANIMATION_ID::JUMP, 60,
		{
			{SOLDIER_SPRITE_ID::JUMP_01,0},
		});
	GraphicsHelper::InsertAnimation(SOLDIER_ANIMATION_ID::SHOOT, 150,
		{
			{SOLDIER_SPRITE_ID::SHOOT_01,0},
			{SOLDIER_SPRITE_ID::SHOOT_02,0},
		});
	GraphicsHelper::InsertAnimation(SOLDIER_ANIMATION_ID::LAY_DOWN, 150,
		{
			{SOLDIER_SPRITE_ID::LAY_DOWN_01,0},
		});
	GraphicsHelper::InsertAnimation(SOLDIER_ANIMATION_ID::DIE, 150,
		{
			{SOLDIER_SPRITE_ID::DIE_01,0},
		});

#pragma endregion Load Animations

	OutputDebugString(L"Soldier Animations Loaded Successfully\n");
}

SoldierState* Soldier::GetState()
{
	return state;
}

void Soldier::SetState(SoldierState* newState)
{
	ChangeState(state, newState, this);
	newState = NULL;
}

void Soldier::GoDead()
{
	ChangeState(state, new SoldierDieState(), this);
}
