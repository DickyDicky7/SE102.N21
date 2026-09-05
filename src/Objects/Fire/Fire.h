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
	void HandleInput(Input& input) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	bool IsVulnerableToBullet() const override { return false; }
	bool IsEnemy() const override { return true; }
	bool IsLethalToTouch() const override { return true; }
	void SetTarget(const Bill* target) override { this->Enemy<Bill>::SetTarget(target); }
protected:
	float _distanceMove;
	float _x0;
	// Keyed off a flag rather than "_x0 == 0.0f": a fire whose patrol crosses
	// x = 0 would otherwise keep re-capturing its already displaced position as
	// the centre of that patrol, walking the beat by up to _distanceMove.
	bool  _hasCapturedOrigin;
};
