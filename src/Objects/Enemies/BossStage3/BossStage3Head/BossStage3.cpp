#include "BossStage3.h"
#include "Explosion.h"
// O: open
// P: close

BossStage3::BossStage3() : Entity(), HasTextures(), HasSprites(), HasAnimations(), HasWeapons(new BulletBossStage2StateHead())
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position.x = Constants::Enemies::BossStage3::DEFAULT_SPAWN_X;
	this->_position.y = Constants::Enemies::BossStage3::DEFAULT_SPAWN_Y;
	this->SetDebugName(L"BossStage3\n");

	this->_updateState = nullptr;
	this->_handleInputState = nullptr;
	// Filled in by SetHandLeft/SetHandRight; IsHandsDead reads both unguarded.
	this->_boss3Stage3HandLeft  = nullptr;
	this->_boss3Stage3HandRight = nullptr;
	// set direction default is right
	this->_movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	this->_isFire = false;
	this->_state = new BossStage3CloseState();

	this->_hitCounts = Constants::Enemies::BossStage3::Head::HEALTH_POINTS;
	this->_isCounted = false;
	this->_enemyType = ENEMY_TYPE::BOSS;

	this->_firingRate = 0;
}

BossStage3::~BossStage3()
{
	this->_isFire = false;
	Destroy(this->_state);
	Destroy(this->_updateState);
	Destroy(this->_handleInputState);
}

void BossStage3::Update()
{
	if (this->IsDead())
	{
		Sound::GetInstance()->Play("boss2finaldestroy", false, 1);
	}

	if (this->IsHandsDead())
	{
		if (!this->_isCounted)
		{
			this->_hitCounts = Constants::Enemies::BossStage3::Head::HEALTH_POINTS; // Only vulnerable once both hands are destroyed
			this->_isCounted = true;
		}
	}
	else
	{
		this->_hitCounts = (std::numeric_limits<int>::max)(); // Invulnerable while a hand is still alive
	}
	DeferState(this->_updateState, this->_state->Update(*this));

	ApplyDeferredState(this->_state, this->_updateState, this);
	ApplyDeferredState(this->_state, this->_handleInputState, this);
}

void BossStage3::Render()
{
	this->_state->Render(*this);
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

void BossStage3::HandleInput(Input& input)
{
	DeferState(this->_handleInputState, this->_state->HandleInput(*this, input));
}

void InsertSpriteBoss3(SPRITE_ID spriteId, int left, int top, int right, int bottom)
{
	// i write this function to shorten the function: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, BOSS_STAGE_3_TEXTURE_ID::BOSS_STAGE_3);
}

void BossStage3::LoadSprites()
{
	if (HasSprites<BossStage3>::_hasBeenLoaded) return;
	HasSprites<BossStage3>::_hasBeenLoaded = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteBoss3(BOSS_STAGE_3_SPRITE_ID::MIDDLE_01, 6, 5, 96 + 6, 72 + 5);
	InsertSpriteBoss3(BOSS_STAGE_3_SPRITE_ID::MIDDLE_02, 112, 5, 96 + 112, 72 + 5);

	InsertSpriteBoss3(BOSS_STAGE_3_SPRITE_ID::OPEN_1, 218, 5, 96 + 218, 81 + 5);
	InsertSpriteBoss3(BOSS_STAGE_3_SPRITE_ID::OPEN_2, 324, 5, 96 + 324, 81 + 5);
	InsertSpriteBoss3(BOSS_STAGE_3_SPRITE_ID::OPEN_3, 430, 5, 96 + 430, 81 + 5);

	InsertSpriteBoss3(BOSS_STAGE_3_SPRITE_ID::CLOSE_1, 6, 96, 96 + 6, 62 + 96);
	InsertSpriteBoss3(BOSS_STAGE_3_SPRITE_ID::CLOSE_2, 112, 96, 96 + 112, 62 + 96);
	InsertSpriteBoss3(BOSS_STAGE_3_SPRITE_ID::CLOSE_3, 219, 96, 96 + 219, 62 + 96);

#pragma endregion Load Sprites

	OutputDebugString(L"BossStage3 Sprites Loaded Successfully\n");
}

void BossStage3::LoadTextures()
{
	if (HasTextures<BossStage3>::_hasBeenLoaded) return;
	HasTextures<BossStage3>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(BOSS_STAGE_3_TEXTURE_ID::BOSS_STAGE_3, L"Resources\\Textures\\BossStage3.bmp");

	OutputDebugString(L"BossStage3 Textures Loaded Successfully\n");
}

void BossStage3::LoadAnimations()
{
	if (HasAnimations<BossStage3>::_hasBeenLoaded) return;
	HasAnimations<BossStage3>::_hasBeenLoaded = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(BOSS_STAGE_3_ANIMATION_ID::MIDDLE, Constants::Enemies::BossStage3::ANIMATION_DELAY_MILLISECONDS,
		{
			{BOSS_STAGE_3_SPRITE_ID::MIDDLE_01,0},
			{BOSS_STAGE_3_SPRITE_ID::MIDDLE_02,0},
		});

	GraphicsHelper::InsertAnimation(BOSS_STAGE_3_ANIMATION_ID::OPEN, Constants::Enemies::BossStage3::ANIMATION_DELAY_MILLISECONDS,
		{
			{BOSS_STAGE_3_SPRITE_ID::OPEN_1,0},
			{BOSS_STAGE_3_SPRITE_ID::OPEN_2,0},
			{BOSS_STAGE_3_SPRITE_ID::OPEN_3,0},
		});
	GraphicsHelper::InsertAnimation(BOSS_STAGE_3_ANIMATION_ID::CLOSE, Constants::Enemies::BossStage3::ANIMATION_DELAY_MILLISECONDS,
		{
			{BOSS_STAGE_3_SPRITE_ID::CLOSE_1,0},
			{BOSS_STAGE_3_SPRITE_ID::CLOSE_2,0},
			{BOSS_STAGE_3_SPRITE_ID::CLOSE_3,0},
		});
#pragma endregion Load Animations

	OutputDebugString(L"BossStage3 Animations Loaded Successfully\n");
}

void BossStage3::Fire()
{
	if (this->_state && this->_state->CanFire())
	{
		this->HasWeapons::Fire(this->_position.x, this->_position.y, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, this->_movingDirection);
	}
}

bool BossStage3::IsHandsDead() const
{
	return this->_boss3Stage3HandLeft && this->_boss3Stage3HandRight
		&& this->_boss3Stage3HandLeft->IsDead() && this->_boss3Stage3HandRight->IsDead();
}

void BossStage3::ProcessSpecialDeathEffects(std::vector<Entity*>& effectEntities)
{
	Sound::GetInstance()->Stop();
	Sound::GetInstance()->Play("boss2finaldestroy", false, 1);
	float X = this->GetL() + this->GetW() * Constants::Enemies::BossStage3::Head::EXPLOSION_GRID_OFFSET_RATIO;
	float Y = this->GetT() - this->GetH() * Constants::Enemies::BossStage3::Head::EXPLOSION_GRID_OFFSET_RATIO;
	for (int i = Constants::Enemies::BossStage3::Head::DEATH_EFFECT_GRID_MIN_COL; i <= Constants::Enemies::BossStage3::Head::DEATH_EFFECT_GRID_MAX_COL; i++)
	{
		Explosion* subExplosion = new Explosion(new ExplosionType3State());
		subExplosion->SetX(X + i * this->GetW() * Constants::Enemies::BossStage3::Head::EXPLOSION_GRID_OFFSET_RATIO);
		subExplosion->SetY(Y);
		effectEntities.push_back(subExplosion);
		if (i >= Constants::Enemies::BossStage3::Head::DEATH_EFFECT_VERTICAL_MIN_COL && i <= Constants::Enemies::BossStage3::Head::DEATH_EFFECT_VERTICAL_MAX_COL)
		{
			for (int k = +1; k <= Constants::Enemies::BossStage3::Head::DEATH_EFFECT_VERTICAL_DEPTH; k++)
			{
				Explosion* subSubExplosion = new Explosion(new ExplosionType3State());
				subSubExplosion->SetX(X + i * this->GetW() * Constants::Enemies::BossStage3::Head::EXPLOSION_GRID_OFFSET_RATIO);
				subSubExplosion->SetY(Y - k * this->GetH() * Constants::Enemies::BossStage3::Head::EXPLOSION_GRID_OFFSET_RATIO);
				effectEntities.push_back(subSubExplosion);
			}
		}
	}
}
