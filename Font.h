#pragma once


#include "Common.h"
#include "Entity.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"


class Font : public Entity, public HasTextures<Font>, public HasSprites<Font>, public HasAnimations<Font>
{

public:

	std::string fontSpriteId;
	Font(std::string);
	virtual ~Font();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

};

