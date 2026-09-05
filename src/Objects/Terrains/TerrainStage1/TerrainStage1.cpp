#include "tileson.hpp"
#include "TerrainStage1.h"

TerrainStage1::TerrainStage1() : Terrain(), Entity(), HasTextures(), HasSprites(), HasAnimations()
{
}

TerrainStage1::~TerrainStage1()
{
}

void TerrainStage1::Update()
{
}

void TerrainStage1::Render()
{
	this->SetAnimation(TERRAIN_STAGE_1_ANIMATION_ID(this->_animationId), this->_position, this->_movingDirection, this->_angle);
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

void TerrainStage1::HandleInput(Input& input)
{
}

void TerrainStage1::LoadSprites()
{
	if (HasSprites<TerrainStage1>::_hasBeenLoaded) return;

	auto tsonTileset = static_cast<tson::Tileset*>(this->_tileset);
	if (!tsonTileset) return;

	HasSprites<TerrainStage1>::_hasBeenLoaded = true;

	for (auto& tile : tsonTileset->getTiles())
	{
		std::string id = std::to_string(tile.getId()); id = "Stage 1 Sprite " + FormatId(id);
		auto& rect = tile.getDrawingRect();
		GraphicsHelper::InsertSprite
		(
			TERRAIN_STAGE_1_SPRITE_ID(id),
			rect.y, rect.x, rect.x + rect.width, rect.y + rect.height,
			DIRECTION::LEFT, TERRAIN_STAGE_1_TEXTURE_ID("Stage 1 Texture 001")
		);
	}

	OutputDebugString(L"Stage 1 Sprites Loaded Successfully\n");
}

void TerrainStage1::LoadTextures()
{
	if (HasTextures<TerrainStage1>::_hasBeenLoaded) return;
	HasTextures<TerrainStage1>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(TERRAIN_STAGE_1_TEXTURE_ID("Stage 1 Texture 001"), L"Resources\\Textures\\Stage1.bmp");

	OutputDebugString(L"Stage 1 Textures Loaded Successfully\n");
}

void TerrainStage1::LoadAnimations()
{
	if (HasAnimations<TerrainStage1>::_hasBeenLoaded) return;

	auto tsonTileset = static_cast<tson::Tileset*>(this->_tileset);
	if (!tsonTileset) return;

	HasAnimations<TerrainStage1>::_hasBeenLoaded = true;

	for (auto& tile : tsonTileset->getTiles())
	{
		std::string id = std::to_string(tile.getId()); std::string animId = "Stage 1 Animation " + FormatId(id);
		std::vector<std::tuple<SPRITE_ID, TIME>> frames;
		auto& animation = tile.getAnimation();
		if (animation.any())
		{
			for (auto& frame : animation.getFrames())
			{
				std::string spriteId = std::to_string(frame.getTileId()); spriteId = "Stage 1 Sprite " + FormatId(spriteId);
				frames.push_back({ TERRAIN_STAGE_1_SPRITE_ID(spriteId), frame.getDuration() });
			}
		}
		else
		{
			std::string spriteId = "Stage 1 Sprite " + FormatId(id); frames.push_back({ TERRAIN_STAGE_1_SPRITE_ID(spriteId), 0 });
		}
		GraphicsHelper::InsertAnimation(TERRAIN_STAGE_1_ANIMATION_ID(animId), Constants::Graphics::DEFAULT_ANIMATION_DELAY_MILLISECONDS, std::move(frames));
	}

	OutputDebugString(L"Stage 1 Animations Loaded Successfully\n");
}

void TerrainStage1::SetAnimationId(const std::string& animationId)
{
	this->_animationId = "Stage 1 Animation " + FormatId(animationId);
}
