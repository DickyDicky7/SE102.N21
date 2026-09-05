#pragma once

#include "Common.h"

namespace Motion
{

	struct UniformMotionInputParameters
	{
		float coordinate;
		float velocity;
	};

	struct UniformMotionOutputParameters
	{
		float coordinate;
		float velocity;
	};

	struct ProjectileMotionInputParameters
	{
		float positionX;
		float positionY;
		float initialVelocity;
		float launchAngleDegrees;
		float elapsedTime;
		float deltaTime;
	};

	struct ProjectileMotionOutputParameters
	{
		float positionX;
		float positionY;
		float initialVelocity;
		float launchAngleDegrees;
		float elapsedTime;
		float deltaTime;

		float velocityX;
		float velocityY;
	};

	struct OscillatoryMotionInputParameters
	{
		float initialCoordinate;
		float elapsedTime;
		float deltaTime;
		float period;
		float amplitude;
		float initialPhaseDegrees;
		// initialCoordinate: initial position
		// elapsedTime: elapsed time
		// deltaTime: delta time
		// period: period (in seconds)
		// amplitude: amplitude / radius
		// initialPhaseDegrees: initial phase (-pi < phi < pi in degrees)
	};

	struct OscillatoryMotionOutputParameters
	{
		float initialCoordinate;
		float elapsedTime;
		float deltaTime;
		float period;
		float amplitude;
		float initialPhaseDegrees;

		float coordinate;
		float velocity;
		float acceleration;
	};

	struct UniformCircularMotionInputParameters
	{
		float radius;
		float angleDegrees;
		float deltaAngleDegrees;
		float centerPositionX;
		float centerPositionY;
	};

	struct UniformCircularMotionOutputParameters
	{
		float radius;
		float angleDegrees;
		float deltaAngleDegrees;
		float centerPositionX;
		float centerPositionY;

		float positionX;
		float positionY;
	};

	struct UniformlyAcceleratedMotionInputParameters
	{
		float coordinate;
		float velocity;
		float acceleration;
		float elapsedTime;
		float deltaTime;
	};

	struct UniformlyAcceleratedMotionOutputParameters
	{
		float coordinate;
		float velocity;
		float acceleration;
		float elapsedTime;
		float deltaTime;
	};

	struct UniformlyDeceleratedMotionInputParameters
	{
		float coordinate;
		float velocity;
		float acceleration;
		float elapsedTime;
		float deltaTime;
	};

	struct UniformlyDeceleratedMotionOutputParameters
	{
		float coordinate;
		float velocity;
		float acceleration;
		float elapsedTime;
		float deltaTime;
	};

	UniformMotionOutputParameters CalculateUniformMotion(UniformMotionInputParameters uniformMotionInputParameters);
	ProjectileMotionOutputParameters CalculateProjectileMotion(ProjectileMotionInputParameters projectileMotionInputParameters);
	OscillatoryMotionOutputParameters CalculateOscillatoryMotion(OscillatoryMotionInputParameters oscillatoryMotionInputParameters);
	UniformCircularMotionOutputParameters CalculateUniformCircularMotion(UniformCircularMotionInputParameters uniformCircularMotionInputParameters);
	UniformlyAcceleratedMotionOutputParameters CalculateUniformlyAcceleratedMotion(UniformlyAcceleratedMotionInputParameters acceleratedMotionInputParameters);
	UniformlyDeceleratedMotionOutputParameters CalculateUniformlyDeceleratedMotion(UniformlyDeceleratedMotionInputParameters deceleratedMotionInputParameters);

};

