#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"

class Fire : public Entity
	, public HasTextures<Fire>, public HasSprites<Fire>, public HasAnimations<Fire>, public Enemy<Bill>
{
public:
	Fire();
	virtual ~Fire();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

protected:
	FLOAT distanceMove;
	FLOAT x0;
};