#include "Font.h"
#include "tileson.hpp"

Font::Font(std::string codeName) : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	fontSpriteId = "Font Sprite " + codeName;
}

Font::~Font()
{
}

void Font::Update()
{
}

void Font::Render()
{
	GraphicsHelper::DrawSprite(GraphicsDatabase::sprites[FONT_SPRITE_ID(fontSpriteId)], position, movingDirection, angle);
}

void Font::HandleInput(Input& input)
{
}

void Font::LoadSprites()
{
	if (HasSprites<Font>::hasBeenLoaded.value) return;
	HasSprites<Font>::hasBeenLoaded.value = true;

	tson::Tileson tileson;
	auto  map     = tileson.parse(fs::path("Resources/Fonts/Font1.json"));
	auto& tileset = map.get()->getTilesets()[0];

	for (auto& tile : tileset.getTiles())
	{
		auto& classType  = tile.getClassType();
		if   (classType != "")
		{
			auto& rect = tile.getDrawingRect();
			GraphicsHelper::InsertSprite
			(
				FONT_SPRITE_ID("Font Sprite " + classType), rect.y, rect.x, rect.x + rect.width, rect.y + rect.height,
				DIRECTION::LEFT, FONT_TEXTURE_ID("Font Texture 001")
			);
		}
	}

	OutputDebugString(L"Font Sprites Loaded Successfully\n");
}

void Font::LoadTextures()
{
	if (HasTextures<Font>::hasBeenLoaded.value) return;
	HasTextures<Font>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(FONT_TEXTURE_ID("Font Texture 001"), L"Resources\\Textures\\Font1.bmp");

	OutputDebugString(L"Font Textures Loaded Successfully\n");
}

void Font::LoadAnimations()
{
	if (HasAnimations<Font>::hasBeenLoaded.value) return;
	HasAnimations<Font>::hasBeenLoaded.value = true;

	OutputDebugString(L"Font Animations Loaded Successfully\n");
}
