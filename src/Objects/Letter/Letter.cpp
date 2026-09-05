#include "Letter.h"
#include "tileson.hpp"

Letter::Letter(std::string_view codeName) : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	this->_letterSpriteId.reserve(Constants::Objects::Letter::LETTER_SPRITE_ID_RESERVE_CAPACITY + codeName.size());
	this->_letterSpriteId.append("Letter Sprite ");
	this->_letterSpriteId.append(codeName);
}

Letter::~Letter()
{
}

void Letter::Update()
{
}

void Letter::Render()
{
	GraphicsHelper::DrawSprite(GraphicsDatabase::sprites[LETTER_SPRITE_ID(this->_letterSpriteId)], this->_position, this->_movingDirection, this->_angle);
}

void Letter::HandleInput(Input& input)
{
}

void Letter::LoadSprites()
{
	if (HasSprites<Letter>::_hasBeenLoaded) return;
	HasSprites<Letter>::_hasBeenLoaded = true;

	tson::Tileson tileson;
	auto  map     = tileson.parse(fs::path("Resources/Fonts/Font1.json"));

	// Guard against tileson parse failure or empty tilesets
	if (!map || map->getStatus() != tson::ParseStatus::OK || map->getTilesets().empty())
	{
		OutputDebugStringA("Letter::LoadSprites: Font1.json parse failed or no tilesets - resources may be missing\n");
		return;
	}

	auto& tileset = map->getTilesets()[0];

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
	Letter::letterW = static_cast<float>(tileSize.x);
	Letter::letterH = static_cast<float>(tileSize.y);

	OutputDebugString(L"Letter Sprites Loaded Successfully\n");
}

void Letter::LoadTextures()
{
	if (HasTextures<Letter>::_hasBeenLoaded) return;
	HasTextures<Letter>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(LETTER_TEXTURE_ID("Letter Texture 001"), L"Resources\\Textures\\Font1.bmp");

	OutputDebugString(L"Letter Textures Loaded Successfully\n");
}

void Letter::LoadAnimations()
{
	if (HasAnimations<Letter>::_hasBeenLoaded) return;
	HasAnimations<Letter>::_hasBeenLoaded = true;

	OutputDebugString(L"Letter Animations Loaded Successfully\n");
}

float Letter::letterW = Constants::Objects::Letter::WIDTH;
float Letter::letterH = Constants::Objects::Letter::HEIGHT;


Text::~Text()
{
}

void Text::Render()
{
	for (auto& letter : this->_letters) letter.Render();
}

Text::Text(std::string_view text, float bottom, float left)
{
	this->_letters.reserve(text.size());
	int k = 1;
	for (int i = 0; std::cmp_less(i, text.size()); ++i, ++k)
	{
		Letter letter(std::string_view(&text[i], 1)); letter.SetX(left + (i + k) * Constants::Objects::Letter::SPACING_RATIO * Letter::letterW); letter.SetY(bottom);
		this->_letters.push_back(std::move(letter));
	}
}

Text::Text(const std::vector<std::string>& textList, float bottom, float left)
{
	this->_letters.reserve(textList.size());
	int k = 1;
	for (int i = 0; std::cmp_less(i, textList.size()); ++i, ++k)
	{
		Letter letter(textList[i]); letter.SetX(left + (i + k) * Constants::Objects::Letter::SPACING_RATIO * Letter::letterW); letter.SetY(bottom);
		this->_letters.push_back(std::move(letter));
	}
}

