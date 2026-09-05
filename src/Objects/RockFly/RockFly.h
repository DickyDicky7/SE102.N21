#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"

class RockFly : public Entity, public Enemy<Bill>
	, public HasTextures<RockFly>, public HasSprites<RockFly>, public HasAnimations<RockFly>
{
public:
	// Takes the start and end x coordinates
	RockFly(float xBegin, float xEnd);
	virtual ~RockFly();
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	bool IsRockFly() const override { return true; }
	bool IsWalkableSurface() const override { return true; }
	bool RidesWithSurface() const override { return true; }
	bool IsEnemy() const override { return true; }
	void SetTarget(const Bill* target) override { this->Enemy<Bill>::SetTarget(target); }
protected:
	float _xBegin;
	float _xEnd;
};
