#include "Bill.h"

Bill::Bill() : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	self = this;

	this->vx = 1;
	this->vy = 1;
	this->ax = 0.1;
	this->ay = 0.1;
	this->position.x = SCREEN_WIDTH / 4;
	this->position.y = SCREEN_HEIGHT / 2 - 50;

	this->updateState = NULL;
	this->handleInputState = NULL;
	this->state = new BillNormalState();
}

Bill::~Bill()
{
}

void Bill::LoadAssets(LPDIRECT3DDEVICE9 *d3ddev, LPD3DXSPRITE *spriteHandler)
{
	D3DXCreateSprite(*d3ddev, &*spriteHandler);
	GraphicsHelper::device = *d3ddev;
	GraphicsHelper::spriteHandler = *spriteHandler;

	// TEXURES
	GraphicsHelper::InsertTexures(BILL, L"Resources\\Textures\\MainCharacter2.bmp");
	// SPRITES
	GraphicsHelper::InsertSprites(BILL_NORMAL_01, 30, 1, 25, 65, BILL);

	GraphicsHelper::InsertSprites(BILL_NORMAL_SHOT_01, 30, 1, 25, 65, BILL);
	GraphicsHelper::InsertSprites(BILL_NORMAL_SHOT_02, 30, 1, 25, 65, BILL);

	GraphicsHelper::InsertSprites(BILL_STRAIGHT_UP_01, 19, 51, 66, 65, BILL);

	GraphicsHelper::InsertSprites(BILL_SHOT_STRAIGHT_UP_01, 19, 51, 66, 65, BILL);
	GraphicsHelper::InsertSprites(BILL_SHOT_STRAIGHT_UP_02, 19, 67, 82, 65, BILL);

	GraphicsHelper::InsertSprites(BILL_LAYDOWN_01, 48, 83, 116, 65, BILL);

	GraphicsHelper::InsertSprites(BILL_JUMP_01, 44, 117, 134, 65, BILL);
	GraphicsHelper::InsertSprites(BILL_JUMP_02, 44, 135, 155, 65, BILL);
	GraphicsHelper::InsertSprites(BILL_JUMP_03, 44, 156, 173, 65, BILL);
	GraphicsHelper::InsertSprites(BILL_JUMP_04, 44, 174, 194, 65, BILL);

	GraphicsHelper::InsertSprites(BILL_RUN_01, 66, 2, 23, 102, BILL);
	GraphicsHelper::InsertSprites(BILL_RUN_02, 66, 24, 41, 102, BILL);
	GraphicsHelper::InsertSprites(BILL_RUN_03, 66, 42, 61, 102, BILL);
	GraphicsHelper::InsertSprites(BILL_RUN_04, 66, 62, 83, 102, BILL);
	GraphicsHelper::InsertSprites(BILL_RUN_05, 66, 84, 101, 102, BILL);
	GraphicsHelper::InsertSprites(BILL_RUN_06, 66, 102, 122, 102, BILL);

	GraphicsHelper::InsertSprites(BILL_RUN_SHOT_ANGLE_UP_01, 66, 123, 144, 102, BILL);
	GraphicsHelper::InsertSprites(BILL_RUN_SHOT_ANGLE_UP_02, 66, 145, 162, 102, BILL);
	GraphicsHelper::InsertSprites(BILL_RUN_SHOT_ANGLE_UP_03, 66, 163, 183, 102, BILL);

	GraphicsHelper::InsertSprites(BILL_RUN_SHOT_ANGLE_DOWN_01, 66, 184, 206, 102, BILL);
	GraphicsHelper::InsertSprites(BILL_RUN_SHOT_ANGLE_DOWN_02, 66, 207, 228, 102, BILL);
	GraphicsHelper::InsertSprites(BILL_RUN_SHOT_ANGLE_DOWN_03, 66, 229, 251, 102, BILL);

	GraphicsHelper::InsertSprites(BILL_DEAD_01, 107, 2, 18, 131, BILL);
	GraphicsHelper::InsertSprites(BILL_DEAD_02, 112, 19, 42, 131, BILL);
	GraphicsHelper::InsertSprites(BILL_DEAD_03, 107, 43, 59, 131, BILL);
	GraphicsHelper::InsertSprites(BILL_DEAD_04, 120, 60, 93, 131, BILL);

	GraphicsHelper::InsertSprites(BILL_BEGIN_SWIM_01, 115, 94, 111, 131, BILL);

	GraphicsHelper::InsertSprites(BILL_DIVE_01, 123, 112, 129, 131, BILL);

	GraphicsHelper::InsertSprites(BILL_SWIM_01, 115, 130, 147, 131, BILL);

	GraphicsHelper::InsertSprites(BILL_SWIM_SHOT_ANGLE_UP_01, 113, 148, 168, 131, BILL);

	GraphicsHelper::InsertSprites(BILL_SWIM_SHOT_STRAIGHT_UP_01, 103, 169, 187, 131, BILL);

	GraphicsHelper::InsertSprites(BILL_SWIM_SHOT_01, 113, 191, 217, 131, BILL);

	GraphicsHelper::InsertSprites(BILL_RUN_SHOT_01, 189, 155, 181, 224, BILL);
	GraphicsHelper::InsertSprites(BILL_RUN_SHOT_02, 189, 184, 208, 224, BILL);
	GraphicsHelper::InsertSprites(BILL_RUN_SHOT_03, 189, 210, 236, 224, BILL);

	// ANIMATION
	GraphicsHelper::InsertAnimation(BILL_NORMAL, 150,
		{
			{BILL_NORMAL_01,0},
		}
	);
	GraphicsHelper::InsertAnimation(BILL_NORMAL_SHOT, 150,
		{
			{BILL_NORMAL_SHOT_01,0},
			{BILL_NORMAL_SHOT_02,0},
		}
	);
	GraphicsHelper::InsertAnimation(BILL_STRAIGHT_UP, 150,
		{
			{BILL_STRAIGHT_UP_01,0},
		}
	);
	GraphicsHelper::InsertAnimation(BILL_SHOT_STRAIGHT_UP, 150,
		{
			{BILL_SHOT_STRAIGHT_UP_01,0},
			{BILL_SHOT_STRAIGHT_UP_02,0},
		}
	);
	GraphicsHelper::InsertAnimation(BILL_LAYDOWN, 150,
		{
			{BILL_LAYDOWN_01,0},
		}
	);
	GraphicsHelper::InsertAnimation(BILL_JUMP, 60,
		{
			{BILL_JUMP_01,0},
			{BILL_JUMP_02,0},
			{BILL_JUMP_03,0},
			{BILL_JUMP_04,0},
		}
	);
	GraphicsHelper::InsertAnimation(BILL_RUN, 70,
		{
			{BILL_RUN_01,0},
			{BILL_RUN_02,0},
			{BILL_RUN_03,0},
			{BILL_RUN_04,0},
			{BILL_RUN_05,0},
			{BILL_RUN_06,0},
		}
	);
	GraphicsHelper::InsertAnimation(BILL_RUN_SHOT_ANGLE_UP, 150,
		{
			{BILL_RUN_SHOT_ANGLE_UP_01,0},
			{BILL_RUN_SHOT_ANGLE_UP_02,0},
			{BILL_RUN_SHOT_ANGLE_UP_03,0},
		}
	);
	GraphicsHelper::InsertAnimation(BILL_DEAD, 150,
		{
			{BILL_DEAD_01,0},
			{BILL_DEAD_02,0},
			{BILL_DEAD_03,0},
			{BILL_DEAD_04,0},
		}
	);
	GraphicsHelper::InsertAnimation(BILL_BEGIN_SWIM, 150,
		{
			{BILL_BEGIN_SWIM_01,0},
		}
	);
	GraphicsHelper::InsertAnimation(BILL_DIVE, 150,
		{
			{BILL_DIVE_01,0},
		}
	);
	GraphicsHelper::InsertAnimation(BILL_SWIM, 150,
		{
			{BILL_SWIM_01,0},
		}
	);
	GraphicsHelper::InsertAnimation(BILL_SWIM_SHOT_ANGLE_UP, 150,
		{
			{BILL_SWIM_SHOT_ANGLE_UP_01,0},
		}
	);
	GraphicsHelper::InsertAnimation(BILL_SWIM_SHOT_STRAIGHT_UP, 150,
		{
			{BILL_SWIM_SHOT_STRAIGHT_UP_01,0},
		}
	);
	GraphicsHelper::InsertAnimation(BILL_SWIM_SHOT, 150,
		{
			{BILL_SWIM_SHOT_01,0},
		}
	);
	GraphicsHelper::InsertAnimation(BILL_RUN_SHOT, 150,
		{
			{BILL_RUN_SHOT_01,0},
			{BILL_RUN_SHOT_02,0},
			{BILL_RUN_SHOT_03,0},
		}
	);
}

void Bill::Update()
{
	updateState = state->Update(*this);
}

void Bill::Render()
{
	state->Render(*this);
	if (updateState)
	{
		state->Exit(*this);
		delete state;
		state = updateState;
		state->Enter(*this);
		updateState = NULL;
	}
	if (handleInputState)
	{
		state->Exit(*this);
		delete state;
		state = handleInputState;
		state->Enter(*this);
		handleInputState = NULL;
	}
}

void Bill::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void Bill::LoadSprites()
{
	if (HasSprites<Bill>::hasBeenLoaded.value) return;
	HasSprites<Bill>::hasBeenLoaded.value = true;

#pragma region Load Sprites

	GraphicsDatabase::sprites.insert({ BILL_NORMAL_01, GraphicsHelper::CreateSprite(30 ,1  ,25 ,65 ,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_NORMAL_SHOT_01, GraphicsHelper::CreateSprite(30 ,1  ,25 ,65 ,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_NORMAL_SHOT_02, GraphicsHelper::CreateSprite(30 ,1  ,25 ,65 ,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_STRAIGHT_UP_01, GraphicsHelper::CreateSprite(19 ,51 ,66 ,65 ,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_SHOT_STRAIGHT_UP_01, GraphicsHelper::CreateSprite(19 ,51 ,66 ,65 ,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_SHOT_STRAIGHT_UP_02, GraphicsHelper::CreateSprite(19 ,67 ,82 ,65 ,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_LAYDOWN_01, GraphicsHelper::CreateSprite(48 ,83 ,116,65 ,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_JUMP_01, GraphicsHelper::CreateSprite(44 ,117,134,65 ,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_JUMP_02, GraphicsHelper::CreateSprite(44 ,135,155,65 ,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_JUMP_03, GraphicsHelper::CreateSprite(44 ,156,173,65 ,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_JUMP_04, GraphicsHelper::CreateSprite(44 ,174,194,65 ,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_RUN_01, GraphicsHelper::CreateSprite(66 ,2  ,23 ,102,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_RUN_02, GraphicsHelper::CreateSprite(66 ,24 ,41 ,102,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_RUN_03, GraphicsHelper::CreateSprite(66 ,42 ,61 ,102,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_RUN_04, GraphicsHelper::CreateSprite(66 ,62 ,83 ,102,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_RUN_05, GraphicsHelper::CreateSprite(66 ,84 ,101,102,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_RUN_06, GraphicsHelper::CreateSprite(66 ,102,122,102,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_RUN_SHOT_ANGLE_UP_01, GraphicsHelper::CreateSprite(66 ,123,144,102,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_RUN_SHOT_ANGLE_UP_02, GraphicsHelper::CreateSprite(66 ,145,162,102,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_RUN_SHOT_ANGLE_UP_03, GraphicsHelper::CreateSprite(66 ,163,183,102,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_RUN_SHOT_ANGLE_DOWN_01, GraphicsHelper::CreateSprite(66 ,184,206,102,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_RUN_SHOT_ANGLE_DOWN_02, GraphicsHelper::CreateSprite(66 ,207,228,102,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_RUN_SHOT_ANGLE_DOWN_03, GraphicsHelper::CreateSprite(66 ,229,251,102,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_DEAD_01, GraphicsHelper::CreateSprite(107,2  ,18 ,131,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_DEAD_02, GraphicsHelper::CreateSprite(112,19 ,42 ,131,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_DEAD_03, GraphicsHelper::CreateSprite(107,43 ,59 ,131,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_DEAD_04, GraphicsHelper::CreateSprite(120,60 ,93 ,131,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_BEGIN_SWIM_01, GraphicsHelper::CreateSprite(115,94 ,111,131,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_DIVE_01, GraphicsHelper::CreateSprite(123,112,129,131,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_SWIM_01, GraphicsHelper::CreateSprite(115,130,147,131,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_SWIM_SHOT_ANGLE_UP_01, GraphicsHelper::CreateSprite(113,148,168,131,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_SWIM_SHOT_STRAIGHT_UP_01, GraphicsHelper::CreateSprite(103,169,187,131,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_SWIM_SHOT_01, GraphicsHelper::CreateSprite(113,191,217,131,BILL) });

	GraphicsDatabase::sprites.insert({ BILL_RUN_SHOT_01, GraphicsHelper::CreateSprite(189,155,181,224,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_RUN_SHOT_02, GraphicsHelper::CreateSprite(189,184,208,224,BILL) });
	GraphicsDatabase::sprites.insert({ BILL_RUN_SHOT_03, GraphicsHelper::CreateSprite(189,210,236,224,BILL) });

#pragma endregion Load Sprites

	OutputDebugString(L"Bill Sprites Loaded Successfully\n");
}

void Bill::LoadTextures()
{
	if (HasTextures<Bill>::hasBeenLoaded.value) return;
	HasTextures<Bill>::hasBeenLoaded.value = true;

	GraphicsDatabase::textures.insert({ BILL, GraphicsHelper::CreateTexture(L"Resources\\Textures\\MainCharacter2.bmp") });

	OutputDebugString(L"Bill Textures Loaded Successfully\n");
}

void Bill::LoadAnimations()
{
	if (HasAnimations<Bill>::hasBeenLoaded.value) return;
	HasAnimations<Bill>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsDatabase::animations.insert
	({ BILL_NORMAL, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_NORMAL_01,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_NORMAL_SHOT, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_NORMAL_SHOT_01,0},
			{BILL_NORMAL_SHOT_02,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_STRAIGHT_UP, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_STRAIGHT_UP_01,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_SHOT_STRAIGHT_UP, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_SHOT_STRAIGHT_UP_01,0},
			{BILL_SHOT_STRAIGHT_UP_02,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_LAYDOWN, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_LAYDOWN_01,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_JUMP, GraphicsHelper::CreateAnimation
	(60,
		{
			{BILL_JUMP_01,0},
			{BILL_JUMP_02,0},
			{BILL_JUMP_03,0},
			{BILL_JUMP_04,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_RUN, GraphicsHelper::CreateAnimation
	(70,
		{
			{BILL_RUN_01,0},
			{BILL_RUN_02,0},
			{BILL_RUN_03,0},
			{BILL_RUN_04,0},
			{BILL_RUN_05,0},
			{BILL_RUN_06,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_RUN_SHOT_ANGLE_UP, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_RUN_SHOT_ANGLE_UP_01,0},
			{BILL_RUN_SHOT_ANGLE_UP_02,0},
			{BILL_RUN_SHOT_ANGLE_UP_03,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_RUN_SHOT_ANGLE_DOWN, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_RUN_SHOT_ANGLE_DOWN_01,0},
			{BILL_RUN_SHOT_ANGLE_DOWN_02,0},
			{BILL_RUN_SHOT_ANGLE_DOWN_03,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_DEAD, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_DEAD_01,0},
			{BILL_DEAD_02,0},
			{BILL_DEAD_03,0},
			{BILL_DEAD_04,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_BEGIN_SWIM, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_BEGIN_SWIM_01,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_DIVE, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_DIVE_01,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_SWIM, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_SWIM_01,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_SWIM_SHOT_ANGLE_UP, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_SWIM_SHOT_ANGLE_UP_01,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_SWIM_SHOT_STRAIGHT_UP, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_SWIM_SHOT_STRAIGHT_UP_01,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_SWIM_SHOT, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_SWIM_SHOT_01,0},
		}
	) });

	GraphicsDatabase::animations.insert
	({ BILL_RUN_SHOT, GraphicsHelper::CreateAnimation
	(150,
		{
			{BILL_RUN_SHOT_01,0},
			{BILL_RUN_SHOT_02,0},
			{BILL_RUN_SHOT_03,0},
		}
	) });

#pragma endregion Load Animations

	OutputDebugString(L"Bill Animations Loaded Successfully\n");
}