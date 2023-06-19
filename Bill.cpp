#include "Bill.h"
#include "Item.h"
#include "Bridge.h"
#include "Falcon.h"
#include "RockFly.h"
#include "AirCraft.h"
#include "TerrainBlock.h"

Bill::Bill() : Entity(), HasTextures(), HasSprites(), HasAnimations(), CollidableEntity(), HasWeapons(new BulletSState()), livesLeft(NULL)
{
	CollidableEntity::self = (Entity*)this;

	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->angle = 0;
	this->position.x = 50;
	this->position.y = 00;

	this->state = NULL;
	this->updateState = NULL;
	this->handleInputState = NULL;

	//
	this->name = L"Bill\n";
	//

	if (!state)
	{
		 state = new BillBeginState();
		 state->Enter(*this);
	}
}

Bill::~Bill()
{
	Destroy(state);
	Destroy(updateState);
	Destroy(handleInputState);
}

void Bill::Update()
{
	updateState = state->Update(*this);
}

void Bill::Render()
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

//int i = 1;
void Bill::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);

	//if (input.IsKey(DIK_RSHIFT))
	//{
	//	if (i == 1) SetBulletState(new BulletRState);
	//	if (i == 2) SetBulletState(new BulletFState);
	//	if (i == 3) SetBulletState(new BulletLState);
	//	if (i == 4) SetBulletState(new BulletMState);
	//	if (i == 5) SetBulletState(new BulletSState);
	//	i++;
	//	if (i == 6) i = 1;
	//}
}

void Bill::LoadSprites()
{
	if (HasSprites<Bill>::hasBeenLoaded.value) return;
	HasSprites<Bill>::hasBeenLoaded.value = true;

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
	if (HasTextures<Bill>::hasBeenLoaded.value) return;
	HasTextures<Bill>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(BILL_TEXTURE_ID::BILL_01, L"Resources\\Textures\\BillAndLance.bmp");

	OutputDebugString(L"Bill Textures Loaded Successfully\n");
}

void Bill::LoadAnimations()
{
	if (HasAnimations<Bill>::hasBeenLoaded.value) return;
	HasAnimations<Bill>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::RUN, 100,
	{
		{BILL_SPRITE_ID::RUN_01,0},
		{BILL_SPRITE_ID::RUN_02,0},
		{BILL_SPRITE_ID::RUN_03,0},
		{BILL_SPRITE_ID::RUN_04,0},
		{BILL_SPRITE_ID::RUN_05,0},
		{BILL_SPRITE_ID::RUN_06,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::JUMP, 60,
	{
		{BILL_SPRITE_ID::JUMP_01,0},
		{BILL_SPRITE_ID::JUMP_02,0},
		{BILL_SPRITE_ID::JUMP_03,0},
		{BILL_SPRITE_ID::JUMP_04,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::DEAD, 300,
	{
		{BILL_SPRITE_ID::DEAD_01,0},
		{BILL_SPRITE_ID::DEAD_02,0},
		{BILL_SPRITE_ID::DEAD_03,0},
		{BILL_SPRITE_ID::DEAD_04,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::DIVE, 100,
	{
		{BILL_SPRITE_ID::DIVE_01,0},
		{BILL_SPRITE_ID::DIVE_02,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::FALL, 100,
	{
		{BILL_SPRITE_ID::FALL_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::BEGIN, 100,
	{
		{BILL_SPRITE_ID::BEGIN_01,0},
		{BILL_SPRITE_ID::BEGIN_02,0},
		{BILL_SPRITE_ID::BEGIN_03,0},
		{BILL_SPRITE_ID::BEGIN_04,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::NORMAL, 100,
	{
		{BILL_SPRITE_ID::NORMAL_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::LAYDOWN, 100,
	{
		{BILL_SPRITE_ID::LAYDOWN_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SWIM_RUN, 100,
	{
		{BILL_SPRITE_ID::SWIM_RUN_01,0},
		{BILL_SPRITE_ID::SWIM_RUN_02,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::RUN_SHOT, 100,
	{
		{BILL_SPRITE_ID::RUN_SHOT_01,0},
		{BILL_SPRITE_ID::RUN_SHOT_02,0},
		{BILL_SPRITE_ID::RUN_SHOT_03,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SWIM_SHOT, 100,
	{
		{BILL_SPRITE_ID::SWIM_SHOT_01,0},
		{BILL_SPRITE_ID::SWIM_SHOT_02,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::BEGIN_SWIM, 100,
	{
		{BILL_SPRITE_ID::BEGIN_SWIM_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SWIM_NORMAL, 100,
	{
		{BILL_SPRITE_ID::SWIM_NORMAL_01,0},
		{BILL_SPRITE_ID::SWIM_NORMAL_02,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::NORMAL_SHOT, 100,
	{
		{BILL_SPRITE_ID::NORMAL_SHOT_01,0},
		{BILL_SPRITE_ID::NORMAL_SHOT_02,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::STRAIGHT_UP, 100,
	{
		{BILL_SPRITE_ID::STRAIGHT_UP_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SHOT_STRAIGHT_UP, 100,
	{
		{BILL_SPRITE_ID::SHOT_STRAIGHT_UP_01,0},
		{BILL_SPRITE_ID::SHOT_STRAIGHT_UP_02,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::RUN_SHOT_ANGLE_UP, 100,
	{
		{BILL_SPRITE_ID::RUN_SHOT_ANGLE_UP_01,0},
		{BILL_SPRITE_ID::RUN_SHOT_ANGLE_UP_02,0},
		{BILL_SPRITE_ID::RUN_SHOT_ANGLE_UP_03,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SWIM_SHOT_ANGLE_UP, 100,
	{
		{BILL_SPRITE_ID::SWIM_SHOT_ANGLE_UP_01,0},
		{BILL_SPRITE_ID::SWIM_SHOT_ANGLE_UP_02,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::RUN_SHOT_ANGLE_DOWN, 100,
	{
		{BILL_SPRITE_ID::RUN_SHOT_ANGLE_DOWN_01,0},
		{BILL_SPRITE_ID::RUN_SHOT_ANGLE_DOWN_02,0},
		{BILL_SPRITE_ID::RUN_SHOT_ANGLE_DOWN_03,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SWIM_SHOT_STRAIGHT_UP, 100,
	{
		{BILL_SPRITE_ID::SWIM_SHOT_STRAIGHT_UP_01,0},
		{BILL_SPRITE_ID::SWIM_SHOT_STRAIGHT_UP_02,0},
	});

#pragma endregion Load Animations

	OutputDebugString(L"Bill Animations Loaded Successfully\n");
}

void Bill::Fire                    (                               )
{
	if (dynamic_cast<BillJumpState*>(state))
	{
		if (movingDirection == DIRECTION::RIGHT)
			HasWeapons::Fire(position.x + w / 2.0f, position.y + h * 0.5f, 0.0f, +3.0f, 0.0f, 0.0f, 0.0f, movingDirection);
		else
			HasWeapons::Fire(position.x - w / 2.0f, position.y + h * 0.5f, 0.0f, -3.0f, 0.0f, 0.0f, 0.0f, movingDirection);
	}
	else
	if (dynamic_cast<BillLayDownState*>(state))
	{
		if (movingDirection == DIRECTION::RIGHT)
			HasWeapons::Fire(position.x + w / 2.0f, position.y + h * 0.4f, 0.0f, +3.0f, 0.0f, 0.0f, 0.0f, movingDirection);
		else
			HasWeapons::Fire(position.x - w / 2.0f, position.y + h * 0.4f, 0.0f, -3.0f, 0.0f, 0.0f, 0.0f, movingDirection);
	}
	else
	if (dynamic_cast<BillRunShotAngleDownState*>(state))
	{
		if (movingDirection == DIRECTION::RIGHT)
			HasWeapons::Fire(position.x + w / 2.0f, position.y + h * 0.4f, 0.0f, +3.0f, -2.0f, 0.0f, 0.0f, movingDirection);
		else
			HasWeapons::Fire(position.x - w / 2.0f, position.y + h * 0.4f, 0.0f, -3.0f, -2.0f, 0.0f, 0.0f, movingDirection);
	}
	else
	if (dynamic_cast<BillNormalShotState*>(state) || dynamic_cast<BillRunShotState*>(state))
	{
		if (movingDirection == DIRECTION::RIGHT)
			HasWeapons::Fire(position.x + w / 2.0f, position.y + h * 0.6f, 0.0f, +3.0f, 0.0f, 0.0f, 0.0f, movingDirection);
		else
			HasWeapons::Fire(position.x - w / 2.0f, position.y + h * 0.6f, 0.0f, -3.0f, 0.0f, 0.0f, 0.0f, movingDirection);
	}
	else
	if (dynamic_cast<BillShotStraightUpState*>(state) || dynamic_cast<BillSwimShotStraightUpState*>(state))
	{
		if (movingDirection == DIRECTION::RIGHT)
			HasWeapons::Fire(position.x + w / 2.0f * 0.6f, position.y + h, 0.0f, 0.0f, +3.0f, 0.0f, 0.0f, movingDirection);
		else
			HasWeapons::Fire(position.x - w / 2.0f * 0.6f, position.y + h, 0.0f, 0.0f, +3.0f, 0.0f, 0.0f, movingDirection);
	}
	else
	if (dynamic_cast<BillRunShotAngleUpState*>(state) || dynamic_cast<BillSwimShotAngleUpState*>(state))
	{
		if (movingDirection == DIRECTION::RIGHT)
			HasWeapons::Fire(position.x + w / 2.0f, position.y + h * 0.9f, 0.0f, +3.0f, +2.0f, 0.0f, 0.0f, movingDirection);
		else
			HasWeapons::Fire(position.x - w / 2.0f, position.y + h * 0.9f, 0.0f, -3.0f, +2.0f, 0.0f, 0.0f, movingDirection);
	}
	else
	if (dynamic_cast<BillSwimNormalShotState*>(state) || dynamic_cast<BillSwimRunShotState*>(state))
	{
		if (movingDirection == DIRECTION::RIGHT)
			HasWeapons::Fire(position.x + w / 2.0f, position.y + h * 0.2f, 0.0f, +3.0f, 0.0f, 0.0f, 0.0f, movingDirection);
		else
			HasWeapons::Fire(position.x - w / 2.0f, position.y + h * 0.2f, 0.0f, -3.0f, 0.0f, 0.0f, 0.0f, movingDirection);
	}
	else
	{
		HasWeapons::Fire(GetR(), GetT() * 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, movingDirection);
	}
}

void Bill::StaticResolveNoCollision(                               )
{
	if (dynamic_cast<BillDeadState*>(state))
	{
		surfaceEntity = NULL;
		return;
	}
}

void Bill::StaticResolveOnCollision(AABBSweepResult aabbSweepResult)
{
	if (dynamic_cast<BillDeadState*>(state))
	{
		surfaceEntity = NULL;
		return;
	}
}

void Bill::DynamicResolveNoCollision(                               )
{
	if (dynamic_cast<BillDeadState*>(state))
	{
		surfaceEntity = NULL;
		return;
	}

	if (isAbSurface)
	{
		if (surfaceEntity)
		{
			if (dynamic_cast<RockFly*>(surfaceEntity)
			&&  dynamic_cast<BillNormalState*>(state))
			{
				position.x = surfaceEntity->GetX();
			}
			else
			if (this->GetL() > surfaceEntity->GetR()
			||  this->GetR() < surfaceEntity->GetL())
			{
				if (!dynamic_cast<BillJumpState*>(state))
				{
					isAbSurface = 0; surfaceEntity = NULL;
					auto currentY = position.y; ChangeState(state, new BillFallState(new BillNormalState()), this); position.y = currentY;
				}
			}
		}
	}
}

void Bill::DynamicResolveOnCollision(AABBSweepResult aabbSweepResult)
{
	auto    terrainBlock  = dynamic_cast<TerrainBlock*>(aabbSweepResult.surfaceEntity);
	if     (terrainBlock)
	switch (terrainBlock->type)
	{

	case TERRAIN_BLOCK_TYPE::WALL:
	{
		if (aabbSweepResult.normalX != +0.0f)
		{
			position.x += aabbSweepResult.enTime * vx;
		}
		else
		if (aabbSweepResult.normalY != +0.0f)
		{
			position.y += aabbSweepResult.enTime * vy;
			isAbSurface = 0;
			surfaceEntity = NULL;
			if (!dynamic_cast<BillDeadState*>(state))
			{
				ChangeState(state, new BillDeadState(), this);
			}
			else
			{
				vy = +0.0f;
			}
		}
		return;
	}
	break;

	case TERRAIN_BLOCK_TYPE::WATER:
	{
		if (dynamic_cast<BillDeadState*>(state))
			return;

		if (aabbSweepResult.normalY == +1.0f)
		{
			position.y += aabbSweepResult.enTime * vy;
			isAbSurface = 1;
			surfaceEntity = terrainBlock;
			ChangeState(state, new BillBeginSwimState(), this);
		}
		else
		{
		}
		return;
	}
	break;

	case TERRAIN_BLOCK_TYPE::THROUGHABLE:
	{
		if (dynamic_cast<BillDeadState*>(state))
			return;

		if (aabbSweepResult.normalY == +1.0f)
		{
			if (dynamic_cast<BillFallState*>(state))
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
			ChangeState(state, new BillNormalState(), this);
		}
		else
		{
		}
		return;
	}
	break;

	case TERRAIN_BLOCK_TYPE::NON_THROUGHABLE:
	{
		if (dynamic_cast<BillDeadState*>(state))
			return;

		if (aabbSweepResult.normalY == +1.0f)
		{
			position.y += aabbSweepResult.enTime * vy;
			isAbSurface = 1;
			surfaceEntity = terrainBlock;
			ChangeState(state, new BillNormalState(), this);
		}
		else
		if (aabbSweepResult.normalX != +0.0f)
		{
			ChangeState(state, new BillNormalState(), this);
			position.y = terrainBlock->GetT();
			vy = -1.0f;
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
		if (dynamic_cast<BillDeadState*>(state))
		{
			surfaceEntity = NULL;
			isAbSurface = 0;
			return;
		}

		auto item = dynamic_cast<Item*>(aabbSweepResult.surfaceEntity);
		if  (item)
		{
			item->isDead = 1;
			switch (item->type)
			{

			case ITEM_TYPE::B:
				 Sound::getInstance()->play("weaponB", false, 1);
			break;

			case ITEM_TYPE::F:
				 Sound::getInstance()->play("weaponF", false, 1);
				 SetBulletState(new BulletFState);
			break;

			case ITEM_TYPE::L:
				 Sound::getInstance()->play("weaponL", false, 1);
				 SetBulletState(new BulletLState);
			break;

			case ITEM_TYPE::M:
				 Sound::getInstance()->play("weaponM", false, 1);
				 SetBulletState(new BulletMState);
			break;

			case ITEM_TYPE::R:
				 Sound::getInstance()->play("weaponR", false, 1);
				 SetBulletState(new BulletRState);
			break;

			case ITEM_TYPE::S:
				 Sound::getInstance()->play("weaponS", false, 1);
				 SetBulletState(new BulletSState);
			break;

			case ITEM_TYPE::I:
				Sound::getInstance()->play("weaponD", false, 1);
			break;

			}
			return;
		}

		auto bullet = dynamic_cast<Bullet*>(aabbSweepResult.surfaceEntity);
		if  (bullet)
		{
		if  (bullet->isEnemy)
			 ChangeState(state, new BillDeadState(), this);
			 return;
		}

		auto rockFly = dynamic_cast<RockFly*>(aabbSweepResult.surfaceEntity);
		if  (rockFly)
		{
			if (aabbSweepResult.normalY == +1.0f)
			{
				if (surfaceEntity)
				{
					if (abs(rockFly->GetY() - surfaceEntity->GetY() > 48.0f)) // size of 1 tile is 16 x 16 -> 48.0f = 3 tiles
						return;
				}
				position.y += aabbSweepResult.enTime * vy;
				isAbSurface = 1;
				surfaceEntity = rockFly;
				ChangeState(state, new BillNormalState(), this);
			}
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
				ChangeState(state, new BillNormalState(), this);
			}
			return;
		}

		auto falcon = dynamic_cast<Falcon*>(aabbSweepResult.surfaceEntity);
		if  (falcon)
		{
			return;
		}

		auto aircraft = dynamic_cast<AirCraft*>(aabbSweepResult.surfaceEntity);
		if  (aircraft)
		{
			return;
		}

		if (dynamic_cast<Enemy<Bill>*>(aabbSweepResult.surfaceEntity))
		{
			ChangeState(state, new BillDeadState(), this);
			return;
		}
	}

	//_RPT1
	//(
	//	0, "collided: %d, contactX: %f, contactY: %f\nnormalX: %f, normalY: %f, entryTime: %f, exitTime: %f\n\n",
	//	aabbSweepResult.isCollided,
	//	aabbSweepResult.contactX,
	//	aabbSweepResult.contactY,
	//	aabbSweepResult.normalX,
	//	aabbSweepResult.normalY,
	//	aabbSweepResult.enTime,
	//	aabbSweepResult.exTime
	//);
}
