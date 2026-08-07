#include "tileson.hpp"
#include "TerrainStage2.h"

TerrainStage2:: TerrainStage2() : Terrain(), Entity(), HasTextures(), HasSprites(), HasAnimations()
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
	SetAnimation(TERRAIN_STAGE_2_ANIMATION_ID(animationId), position, movingDirection, angle);
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;
}

void TerrainStage2::HandleInput(Input& input)
{
}

void TerrainStage2::LoadSprites()
{
	if (HasSprites<TerrainStage2>::hasBeenLoaded.value) return;
	HasSprites<TerrainStage2>::hasBeenLoaded.value = true;

	auto _tileset = (tson::Tileset*)tileset;
	for (auto& _tile : _tileset->getTiles())
	{
		std::string _id = std::to_string(_tile.getId()); _id = "Stage 2 Sprite " + FormatId(_id);
		auto& _rect = _tile.getDrawingRect();
		GraphicsHelper::InsertSprite
		(
			TERRAIN_STAGE_2_SPRITE_ID(_id), _rect.y, _rect.x, _rect.x + _rect.width, _rect.y + _rect.height,
			DIRECTION::LEFT, TERRAIN_STAGE_2_TEXTURE_ID("Stage 2 Texture 001")
		);
	}
		 _tileset = NULL;

	OutputDebugString(L"Stage 2 Sprites Loaded Successfully\n");
}

void TerrainStage2::LoadTextures()
{
	if (HasTextures<TerrainStage2>::hasBeenLoaded.value) return;
	HasTextures<TerrainStage2>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(TERRAIN_STAGE_2_TEXTURE_ID("Stage 2 Texture 001"), L"Resources\\Textures\\Stage2.bmp");

	OutputDebugString(L"Stage 2 Textures Loaded Successfully\n");
}

void TerrainStage2::LoadAnimations()
{
	if (HasAnimations<TerrainStage2>::hasBeenLoaded.value) return;
	HasAnimations<TerrainStage2>::hasBeenLoaded.value = true;

	auto _tileset = (tson::Tileset*)tileset;
	for (auto& _tile : _tileset->getTiles())
	{
		std::string  _id = std::to_string(_tile.getId()); std::string _animationId = "Stage 2 Animation " + FormatId(_id);
		std::vector<std::tuple<SPRITE_ID, TIME>> _frames;
		auto& _animation = _tile.getAnimation();
		if   (_animation.any())
		{
			for (auto& _frame : _animation.getFrames())
			{
				std::string _spriteId = std::to_string(_frame.getTileId()); _spriteId = "Stage 2 Sprite " + FormatId(_spriteId);
				_frames.push_back({ TERRAIN_STAGE_2_SPRITE_ID(_spriteId), _frame.getDuration() });
			}
		}
		else
		{
			std::string _spriteId = "Stage 2 Sprite " + FormatId(_id); _frames.push_back({ TERRAIN_STAGE_2_SPRITE_ID(_spriteId), 0 });
		}
		GraphicsHelper::InsertAnimation(TERRAIN_STAGE_2_ANIMATION_ID(_animationId), 300, _frames);
	}
		 _tileset = NULL;

	OutputDebugString(L"Stage 2 Animations Loaded Successfully\n");
}

void TerrainStage2::SetAnimationId(const std::string& animationId)
{
	this->animationId = "Stage 2 Animation " + FormatId(animationId);
}
