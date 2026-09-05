#include "TerrainBlock.h"

TerrainBlock::TerrainBlock() : Entity(), CollidableEntity(), _name(""), _type(TERRAIN_BLOCK_TYPE::NONE)
{
	CollidableEntity::_self = this;
}

TerrainBlock::~TerrainBlock()
{
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
