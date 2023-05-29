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
	Letter(std::string);
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	static FLOAT letterW;
	static FLOAT letterH;
	 std::string letterSpriteId;

};


class Text
{

public:

	virtual ~Text();
	virtual void Render();
	Text(std::string, FLOAT, FLOAT);
	Text(std::vector<std::string>, FLOAT, FLOAT);

protected:

	std::list<Letter> letters;

};

