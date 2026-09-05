#pragma once


#include "Common.h"
#include "Entity.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"


class Letter : public Entity, public HasTextures<Letter>, public HasSprites<Letter>, public HasAnimations<Letter>
{

public:

	virtual ~Letter();
	Letter(std::string_view codeName);
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	static float letterW;
	static float letterH;

protected:

	std::string _letterSpriteId;

};


class Text
{

public:

	virtual ~Text();
	virtual void Render();
	Text(std::string_view text, float bottom, float left);
	Text(const std::vector<std::string>& textList, float bottom, float left);

protected:

	std::vector<Letter> _letters;

};

