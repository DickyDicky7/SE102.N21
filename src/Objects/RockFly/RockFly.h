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
	// truyen vao x bat dau va ket thuc
	RockFly(FLOAT, FLOAT);
	virtual ~RockFly();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

protected:
	FLOAT xBegin;
	FLOAT xEnd;
};