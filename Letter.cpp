#include "Letter.h"
#include "tileson.hpp"

Letter::Letter(std::string codeName) : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	letterSpriteId = "Letter Sprite " + codeName;
}

Letter::~Letter()
{
}

void Letter::Update()
{
}

void Letter::Render()
{
	GraphicsHelper::DrawSprite(GraphicsDatabase::sprites[LETTER_SPRITE_ID(letterSpriteId)], position, movingDirection, angle);
}

void Letter::HandleInput(Input& input)
{
}

void Letter::LoadSprites()
{
	if (HasSprites<Letter>::hasBeenLoaded.value) return;
	HasSprites<Letter>::hasBeenLoaded.value = true;

	tson::Tileson tileson;
	auto  map     = tileson.parse(fs::path("Resources/Fonts/Font1.json"));
	auto& tileset = map.get()->getTilesets()[0];

	for (auto& tile : tileset.getTiles())
	{
		auto& classType = tile.getClassType();
		if   (classType != "")
		{
			auto& rect = tile.getDrawingRect();
			GraphicsHelper::InsertSprite
			(
				LETTER_SPRITE_ID("Letter Sprite " + classType), rect.y, rect.x, rect.x + rect.width, rect.y + rect.height,
				DIRECTION::LEFT, LETTER_TEXTURE_ID("Letter Texture 001")
			);
		}
	}

	auto& tileSize  = tileset.getTileSize();
	Letter::letterW = FLOAT(tileSize.x);
	Letter::letterH = FLOAT(tileSize.y);

	OutputDebugString(L"Letter Sprites Loaded Successfully\n");
}

void Letter::LoadTextures()
{
	if (HasTextures<Letter>::hasBeenLoaded.value) return;
	HasTextures<Letter>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(LETTER_TEXTURE_ID("Letter Texture 001"), L"Resources\\Textures\\Font1.bmp");

	OutputDebugString(L"Letter Textures Loaded Successfully\n");
}

void Letter::LoadAnimations()
{
	if (HasAnimations<Letter>::hasBeenLoaded.value) return;
	HasAnimations<Letter>::hasBeenLoaded.value = true;

	OutputDebugString(L"Letter Animations Loaded Successfully\n");
}

FLOAT Letter::letterW;
FLOAT Letter::letterH;


Text::~Text()
{
}

void Text::Render()
{
	for (auto& letter : letters) letter.Render();
}

Text::Text(std::string letters, FLOAT bottom, FLOAT left)
{
	for (int i = 0, k = 1; std::cmp_less(i, letters.size()); ++i, ++k)
	{
		Letter letter(std::string(1, letters[i])); letter.SetX(left + (i + k) * 0.5f * Letter::letterW); letter.SetY(bottom);
		this->letters.push_back(letter);
	}
}

Text::Text(std::vector<std::string> letters, FLOAT bottom, FLOAT left)
{
	for (int i = 0, k = 1; std::cmp_less(i, letters.size()); ++i, ++k)
	{
		Letter letter(letters[i]); letter.SetX(left + (i + k) * 0.5f * Letter::letterW); letter.SetY(bottom);
		this->letters.push_back(letter);
	}
}

