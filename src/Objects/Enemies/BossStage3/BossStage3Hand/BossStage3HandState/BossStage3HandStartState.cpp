#include <cmath>
#include "BossStage3Hand.h"

BossStage3HandStartState::BossStage3HandStartState(BossStage3Hand& bossStage3Hand) : BossStage3HandState()
{
	// Moved out of the destructor, where the stores were dead and left the
	// object uninitialised between construction and Enter().
	this->_direction = D3DXVECTOR2(0.0f, 0.0f);
	this->_speedFrame = 0.0f;
	this->_distance = 0.0f;
	this->_delayFrame = 0;
}

BossStage3HandStartState::~BossStage3HandStartState()
{
}


void BossStage3HandStartState::Exit(BossStage3Hand& bossStage3Hand)
{
}

void BossStage3HandStartState::Enter(BossStage3Hand& bossStage3Hand)
{
	this->_delayFrame = Constants::Enemies::BossStage3::Hand::START_STATE_DELAY_FRAMES;

	if (bossStage3Hand.GetMovingDirection() == DIRECTION::LEFT)
	{
		this->_direction = D3DXVECTOR2(std::cos(D3DXToRadian(Constants::Enemies::BossStage3::Hand::START_DIRECTION_ANGLE_LEFT_DEGREES)), std::sin(D3DXToRadian(Constants::Enemies::BossStage3::Hand::START_DIRECTION_ANGLE_LEFT_DEGREES))); // Direction of travel
	}
	else
	{
		this->_direction = D3DXVECTOR2(std::cos(D3DXToRadian(Constants::Enemies::BossStage3::Hand::START_DIRECTION_ANGLE_RIGHT_DEGREES)), std::sin(D3DXToRadian(Constants::Enemies::BossStage3::Hand::START_DIRECTION_ANGLE_RIGHT_DEGREES))); // Direction of travel
	}

	this->_distance = Constants::Enemies::BossStage3::Hand::START_DISTANCE_TILES * Constants::Screen::TILE_SIZE; // Distance to travel
	this->_speedFrame = this->_distance / D3DXVec2Length(&this->_direction); // Distance per frame
	float t = Constants::Screen::TILE_SIZE / D3DXVec2Length(&this->_direction); // Time to cover one tile - used to stagger the next joint's delay

	for (size_t i = 4; i > 0; i--)
	{
		bossStage3Hand.GetJoint(i)->MoveBy(this->_direction.x, this->_direction.y, static_cast<int>(this->_speedFrame - static_cast<float>(4 - i) * t), static_cast<int>(static_cast<float>(4 - i) * t));
	}
}



BossStage3HandState* BossStage3HandStartState::Update(BossStage3Hand& bossStage3Hand)
{
	if (this->_delayFrame > 0)
	{
		this->_delayFrame--;
		return nullptr;
	}

	for (size_t i = 0; i < Constants::Enemies::BossStage3::Hand::TOTAL_JOINTS_COUNT; i++)
	{
		bossStage3Hand.GetJoint(i)->Update();
	}

	if (!bossStage3Hand.GetJoint(4)->IsMoveBy())
	{
		return new BossStage3HandWaveState(bossStage3Hand);
	}
	return nullptr;
}

BossStage3HandState* BossStage3HandStartState::HandleInput(BossStage3Hand& bossStage3Hand, Input& input)
{
	return nullptr;
}
