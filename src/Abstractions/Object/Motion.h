#pragma once

#include "Common.h"

namespace Motion
{

	struct UniformMotionInputParameters
	{
		FLOAT c;
		FLOAT v;
	};

	struct UniformMotionOutputParameters
	{
		FLOAT c;
		FLOAT v;
	};

	struct ProjectileMotionInputParameters
	{
		FLOAT x;
		FLOAT y;
		FLOAT v0;
		FLOAT θ;
		FLOAT t;
		FLOAT dt;
	};

	struct ProjectileMotionOutputParameters
	{
		FLOAT x;
		FLOAT y;
		FLOAT v0;
		FLOAT θ;
		FLOAT t;
		FLOAT dt;

		FLOAT vx;
		FLOAT vy;
	};

	struct OscillatoryMotionInputParameters
	{
		FLOAT c0;
		FLOAT t;
		FLOAT dt;
		FLOAT T;
		FLOAT A;
		FLOAT φ;
		// c0: vị trí lúc đầu
		// t: thời gian
		// dt: delta time
		// T: khoảng thời gian để quay hết 1 vòng (tính bằng giây)
		// A: Bán kính
		// φ: pha ban đầu của dao động (-π<φ<π)
	};

	struct OscillatoryMotionOutputParameters
	{
		FLOAT c0;
		FLOAT t;
		FLOAT dt;
		FLOAT T;
		FLOAT A;
		FLOAT φ;

		FLOAT c;
		FLOAT v;
		FLOAT a;
	};

	struct UniformCircularMotionInputParameters
	{
		FLOAT r;
		FLOAT ω;
		FLOAT dω;
		FLOAT xO;
		FLOAT yO;
	};

	struct UniformCircularMotionOutputParameters
	{
		FLOAT r;
		FLOAT ω;
		FLOAT dω;
		FLOAT xO;
		FLOAT yO;

		FLOAT x;
		FLOAT y;
	};

	struct UniformlyAcceleratedMotionInputParameters
	{
		FLOAT c;
		FLOAT v;
		FLOAT a;
		FLOAT t;
		FLOAT dt;
	};

	struct UniformlyAcceleratedMotionOutputParameters
	{
		FLOAT c;
		FLOAT v;
		FLOAT a;
		FLOAT t;
		FLOAT dt;
	};

	struct UniformlyDeceleratedMotionInputParameters
	{
		FLOAT c;
		FLOAT v;
		FLOAT a;
		FLOAT t;
		FLOAT dt;
	};

	struct UniformlyDeceleratedMotionOutputParameters
	{
		FLOAT c;
		FLOAT v;
		FLOAT a;
		FLOAT t;
		FLOAT dt;
	};

	UniformMotionOutputParameters CalculateUniformMotion(UniformMotionInputParameters);
	ProjectileMotionOutputParameters CalculateProjectileMotion(ProjectileMotionInputParameters);
	OscillatoryMotionOutputParameters CalculateOscillatoryMotion(OscillatoryMotionInputParameters);
	UniformCircularMotionOutputParameters CalculateUniformCircularMotion(UniformCircularMotionInputParameters);
	UniformlyAcceleratedMotionOutputParameters CalculateUniformlyAcceleratedMotion(UniformlyAcceleratedMotionInputParameters);
	UniformlyDeceleratedMotionOutputParameters CalculateUniformlyDeceleratedMotion(UniformlyDeceleratedMotionInputParameters);

};

