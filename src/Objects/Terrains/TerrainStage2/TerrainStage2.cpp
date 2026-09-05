#include "tileson.hpp"
#include "TerrainStage2.h"

TerrainStage2::TerrainStage2() : Terrain(), Entity(), HasTextures(), HasSprites(), HasAnimations()
{
}

TerrainStage2::~TerrainStage2()
{
}

void TerrainStage2::Update()
{
}

void TerrainStage2::Render()
{
	this->SetAnimation(TERRAIN_STAGE_2_ANIMATION_ID(this->_animationId), this->_position, this->_movingDirection, this->_angle);
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

void TerrainStage2::HandleInput(Input& input)
{
}

void TerrainStage2::LoadSprites()
{
	if (HasSprites<TerrainStage2>::_hasBeenLoaded) return;

	auto tsonTileset = static_cast<tson::Tileset*>(this->_tileset);
	if (!tsonTileset) return;

	HasSprites<TerrainStage2>::_hasBeenLoaded = true;

	for (auto& tile : tsonTileset->getTiles())
	{
		std::string id = std::to_string(tile.getId()); id = "Stage 2 Sprite " + FormatId(id);
		auto& rect = tile.getDrawingRect();
		GraphicsHelper::InsertSprite
		(
			TERRAIN_STAGE_2_SPRITE_ID(id),
			rect.y, rect.x, rect.x + rect.width, rect.y + rect.height,
			DIRECTION::LEFT, TERRAIN_STAGE_2_TEXTURE_ID("Stage 2 Texture 001")
		);
	}

	OutputDebugString(L"Stage 2 Sprites Loaded Successfully\n");
}

void TerrainStage2::LoadTextures()
{
	if (HasTextures<TerrainStage2>::_hasBeenLoaded) return;
	HasTextures<TerrainStage2>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(TERRAIN_STAGE_2_TEXTURE_ID("Stage 2 Texture 001"), L"Resources\\Textures\\Stage2.bmp");

	OutputDebugString(L"Stage 2 Textures Loaded Successfully\n");
}

void TerrainStage2::LoadAnimations()
{
	if (HasAnimations<TerrainStage2>::_hasBeenLoaded) return;

	auto tsonTileset = static_cast<tson::Tileset*>(this->_tileset);
	if (!tsonTileset) return;

	HasAnimations<TerrainStage2>::_hasBeenLoaded = true;

	for (auto& tile : tsonTileset->getTiles())
	{
		std::string id = std::to_string(tile.getId()); std::string animId = "Stage 2 Animation " + FormatId(id);
		std::vector<std::tuple<SPRITE_ID, TIME>> frames;
		auto& animation = tile.getAnimation();
		if (animation.any())
		{
			for (auto& frame : animation.getFrames())
			{
				std::string spriteId = std::to_string(frame.getTileId()); spriteId = "Stage 2 Sprite " + FormatId(spriteId);
				frames.push_back({ TERRAIN_STAGE_2_SPRITE_ID(spriteId), frame.getDuration() });
			}
		}
		else
		{
			std::string spriteId = "Stage 2 Sprite " + FormatId(id); frames.push_back({ TERRAIN_STAGE_2_SPRITE_ID(spriteId), 0 });
		}
		GraphicsHelper::InsertAnimation(TERRAIN_STAGE_2_ANIMATION_ID(animId), Constants::Graphics::DEFAULT_ANIMATION_DELAY_MILLISECONDS, std::move(frames));
	}

	OutputDebugString(L"Stage 2 Animations Loaded Successfully\n");
}

void TerrainStage2::SetAnimationId(const std::string& animationId)
{
	this->_animationId = "Stage 2 Animation " + FormatId(animationId);
}
