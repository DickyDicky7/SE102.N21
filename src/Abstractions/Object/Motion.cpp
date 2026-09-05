#include <cmath>
#include "Motion.h"

namespace Motion
{

	template <typename OutType>
	static OutType CalculateVariableMotionInternal(
		float coordinate,
		float velocity,
		float acceleration,
		float elapsedTime,
		float deltaTime,
		bool isAccelerating)
	{
		float adjustedAcceleration = acceleration;
		if (isAccelerating ? (velocity * acceleration <= 0.0f) : (velocity * acceleration >= 0.0f))
		{
			adjustedAcceleration = -acceleration;
		}
		float nextElapsedTime = elapsedTime + deltaTime;
		float nextVelocity = velocity + adjustedAcceleration * elapsedTime;
		float nextCoordinate = coordinate + velocity * elapsedTime + adjustedAcceleration * elapsedTime * elapsedTime * Constants::Physics::KINEMATIC_DISPLACEMENT_COEFFICIENT;

		return OutType{ nextCoordinate, nextVelocity, adjustedAcceleration, nextElapsedTime, deltaTime };
	}

	UniformMotionOutputParameters CalculateUniformMotion(UniformMotionInputParameters uniformMotionInputParameters)
	{
		auto [coordinate, velocity] = uniformMotionInputParameters;
		float nextCoordinate = coordinate + velocity;
		return { nextCoordinate, velocity };
	}

	ProjectileMotionOutputParameters CalculateProjectileMotion(ProjectileMotionInputParameters projectileMotionInputParameters)
	{
		auto [positionX, positionY, initialVelocity, launchAngleDegrees, elapsedTime, deltaTime] = projectileMotionInputParameters;

		float radians = D3DXToRadian(launchAngleDegrees);
		float cosRadians = std::cos(radians);
		float sinRadians = std::sin(radians);

		float nextElapsedTime = elapsedTime + deltaTime;
		float nextPositionX = positionX + initialVelocity * cosRadians * elapsedTime;
		float nextPositionY = positionY + initialVelocity * sinRadians * elapsedTime - (Constants::Physics::GRAVITY_ACCELERATION * Constants::Physics::KINEMATIC_DISPLACEMENT_COEFFICIENT) * elapsedTime * elapsedTime;
		float velocityX = initialVelocity * cosRadians;
		float velocityY = initialVelocity * sinRadians - Constants::Physics::GRAVITY_ACCELERATION * elapsedTime;

		return { nextPositionX, nextPositionY, initialVelocity, launchAngleDegrees, nextElapsedTime, deltaTime, velocityX, velocityY };
	}

	OscillatoryMotionOutputParameters CalculateOscillatoryMotion(OscillatoryMotionInputParameters oscillatoryMotionInputParameters)
	{
		auto [initialCoordinate, elapsedTime, deltaTime, period, amplitude, initialPhaseDegrees] = oscillatoryMotionInputParameters;

		float angularFrequency = (period != 0.0f) ? (Constants::Physics::TURN_RADIANS / period) : 0.0f;
		float currentPhaseRadians = angularFrequency * elapsedTime + D3DXToRadian(initialPhaseDegrees);

		float nextElapsedTime = elapsedTime + deltaTime;
		float coordinate = initialCoordinate + amplitude * std::cos(currentPhaseRadians);
		float velocity = -amplitude * angularFrequency * std::sin(currentPhaseRadians);
		float acceleration = -amplitude * angularFrequency * angularFrequency * std::cos(currentPhaseRadians);

		return { initialCoordinate, nextElapsedTime, deltaTime, period, amplitude, initialPhaseDegrees, coordinate, velocity, acceleration };
	}

	UniformCircularMotionOutputParameters CalculateUniformCircularMotion(UniformCircularMotionInputParameters uniformCircularMotionInputParameters)
	{
		auto [radius, angleDegrees, deltaAngleDegrees, centerPositionX, centerPositionY] = uniformCircularMotionInputParameters;

		float nextAngleDegrees = angleDegrees + deltaAngleDegrees;
		float positionX = centerPositionX + radius * std::cos(D3DXToRadian(angleDegrees));
		float positionY = centerPositionY + radius * std::sin(D3DXToRadian(angleDegrees));

		return { radius, nextAngleDegrees, deltaAngleDegrees, centerPositionX, centerPositionY, positionX, positionY };
	}

	UniformlyAcceleratedMotionOutputParameters CalculateUniformlyAcceleratedMotion(UniformlyAcceleratedMotionInputParameters acceleratedMotionInputParameters)
	{
		return CalculateVariableMotionInternal<UniformlyAcceleratedMotionOutputParameters>(
			acceleratedMotionInputParameters.coordinate,
			acceleratedMotionInputParameters.velocity,
			acceleratedMotionInputParameters.acceleration,
			acceleratedMotionInputParameters.elapsedTime,
			acceleratedMotionInputParameters.deltaTime,
			true
		);
	}

	UniformlyDeceleratedMotionOutputParameters CalculateUniformlyDeceleratedMotion(UniformlyDeceleratedMotionInputParameters deceleratedMotionInputParameters)
	{
		return CalculateVariableMotionInternal<UniformlyDeceleratedMotionOutputParameters>(
			deceleratedMotionInputParameters.coordinate,
			deceleratedMotionInputParameters.velocity,
			deceleratedMotionInputParameters.acceleration,
			deceleratedMotionInputParameters.elapsedTime,
			deceleratedMotionInputParameters.deltaTime,
			false
		);
	}

};


