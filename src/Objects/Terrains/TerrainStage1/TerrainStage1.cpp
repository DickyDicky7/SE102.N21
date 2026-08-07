#include "tileson.hpp"
#include "TerrainStage1.h"

TerrainStage1:: TerrainStage1() : Terrain(), Entity(), HasTextures(), HasSprites(), HasAnimations()
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
	SetAnimation(TERRAIN_STAGE_1_ANIMATION_ID(animationId), position, movingDirection, angle);
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;
}

void TerrainStage1::HandleInput(Input& input)
{
}

void TerrainStage1::LoadSprites()
{
	if (HasSprites<TerrainStage1>::hasBeenLoaded.value) return;
	HasSprites<TerrainStage1>::hasBeenLoaded.value = true;

	auto _tileset = (tson::Tileset*)tileset;
	for (auto& _tile : _tileset->getTiles())
	{
		std::string _id = std::to_string(_tile.getId()); _id = "Stage 1 Sprite " + FormatId(_id);
		auto& _rect = _tile.getDrawingRect();
		GraphicsHelper::InsertSprite
		(
			TERRAIN_STAGE_1_SPRITE_ID(_id), _rect.y, _rect.x, _rect.x + _rect.width, _rect.y + _rect.height,
			DIRECTION::LEFT, TERRAIN_STAGE_1_TEXTURE_ID("Stage 1 Texture 001")
		);
	}
		 _tileset = NULL;

	OutputDebugString(L"Stage 1 Sprites Loaded Successfully\n");
}

void TerrainStage1::LoadTextures()
{
	if (HasTextures<TerrainStage1>::hasBeenLoaded.value) return;
	HasTextures<TerrainStage1>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(TERRAIN_STAGE_1_TEXTURE_ID("Stage 1 Texture 001"), L"Resources\\Textures\\Stage1.bmp");

	OutputDebugString(L"Stage 1 Textures Loaded Successfully\n");
}

void TerrainStage1::LoadAnimations()
{
	if (HasAnimations<TerrainStage1>::hasBeenLoaded.value) return;
	HasAnimations<TerrainStage1>::hasBeenLoaded.value = true;

	auto _tileset = (tson::Tileset*)tileset;
	for (auto& _tile : _tileset->getTiles())
	{
		std::string  _id = std::to_string(_tile.getId()); std::string _animationId = "Stage 1 Animation " + FormatId(_id);
		std::vector<std::tuple<SPRITE_ID, TIME>> _frames;
		auto& _animation = _tile.getAnimation();
		if   (_animation.any())
		{
			for (auto& _frame : _animation.getFrames())
			{
				std::string _spriteId = std::to_string(_frame.getTileId()); _spriteId = "Stage 1 Sprite " + FormatId(_spriteId);
				_frames.push_back({ TERRAIN_STAGE_1_SPRITE_ID(_spriteId), _frame.getDuration() });
			}
		}
		else
		{
			std::string _spriteId = "Stage 1 Sprite " + FormatId(_id); _frames.push_back({ TERRAIN_STAGE_1_SPRITE_ID(_spriteId), 0 });
		}
		GraphicsHelper::InsertAnimation(TERRAIN_STAGE_1_ANIMATION_ID(_animationId), 300, _frames);
	}
		 _tileset = NULL;

	OutputDebugString(L"Stage 1 Animations Loaded Successfully\n");
}

void TerrainStage1::SetAnimationId(const std::string& animationId)
{
	this->animationId = "Stage 1 Animation " + FormatId(animationId);
}
