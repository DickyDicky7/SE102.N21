#include "TestingEntity.h"

void TestingEntity::LoadSprites()
{
	if (HasSprites<TestingEntity>::_hasBeenLoaded) return;
	HasSprites<TestingEntity>::_hasBeenLoaded = true;
	OutputDebugString(L"TestingEntity Load Sprites Successfully\n");
}

void TestingEntity::LoadTextures()
{
	if (HasTextures<TestingEntity>::_hasBeenLoaded) return;
	HasTextures<TestingEntity>::_hasBeenLoaded = true;
	OutputDebugString(L"TestingEntity Load Textures Successfully\n");
}

void TestingEntity::LoadAnimations()
{
	if (HasAnimations<TestingEntity>::_hasBeenLoaded) return;
	HasAnimations<TestingEntity>::_hasBeenLoaded = true;
	OutputDebugString(L"TestingEntity Load Animations Successfully\n");
}
