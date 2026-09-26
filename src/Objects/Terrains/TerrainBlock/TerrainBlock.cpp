#include "TerrainBlock.h"

TerrainBlock::TerrainBlock() : Entity(), CollidableEntity(), _name(""), _type(TERRAIN_BLOCK_TYPE::NONE)
{
	CollidableEntity::_self = this;
}

TerrainBlock::~TerrainBlock()
{
}

void TerrainBlock::SetTerrainType(TERRAIN_BLOCK_TYPE type)
{
	this->_type = type;
}

TERRAIN_BLOCK_TYPE TerrainBlock::GetTerrainType() const
{
	return this->_type;
}

void TerrainBlock::SetEntityName(std::string_view entityName)
{
	this->_name = entityName;
}

std::string TerrainBlock::GetEntityName() const
{
	return this->_name;
}

CollidableEntity* TerrainBlock::AsCollidable()
{
	return this;
}

void TerrainBlock::Update()
{
}

void TerrainBlock::Render()
{
}

void TerrainBlock::HandleInput(Input& input)
{
}

void TerrainBlock::StaticResolveNoCollision()
{
}

void TerrainBlock::StaticResolveOnCollision(AABBSweepResult aabbSweepResult)
{
}

void TerrainBlock::DynamicResolveNoCollision()
{
}

void TerrainBlock::DynamicResolveOnCollision(AABBSweepResult aabbSweepResult)
{
}
