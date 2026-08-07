#include "TestingEntity.h"

void TestingEntity::LoadSprites()
{
	if (HasSprites<TestingEntity>::hasBeenLoaded.value) return;
	HasSprites<TestingEntity>::hasBeenLoaded.value = true;
	OutputDebugString(L"TestingEntity Load Sprites Successfully\n");
}

void TestingEntity::LoadTextures()
{
	if (HasTextures<TestingEntity>::hasBeenLoaded.value) return;
	HasTextures<TestingEntity>::hasBeenLoaded.value = true;
	OutputDebugString(L"TestingEntity Load Textures Successfully\n");
}

void TestingEntity::LoadAnimations()
{
	if (HasAnimations<TestingEntity>::hasBeenLoaded.value) return;
	HasAnimations<TestingEntity>::hasBeenLoaded.value = true;
	OutputDebugString(L"TestingEntity Load Animations Successfully\n");
}
