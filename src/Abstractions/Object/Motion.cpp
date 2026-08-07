#include "Motion.h"

namespace Motion
{

	UniformMotionOutputParameters CalculateUniformMotion(UniformMotionInputParameters uniformMotionInputParameters)
	{
		auto [c, v] = uniformMotionInputParameters;

		FLOAT _c_ = c + v;

		return { _c_, v };
	}

	ProjectileMotionOutputParameters CalculateProjectileMotion(ProjectileMotionInputParameters projectileMotionInputParameters)
	{
		auto [x, y, v0, θ, t, dt] = projectileMotionInputParameters;

		FLOAT _t_  = t + dt;
		FLOAT _x_  = x + v0 * cos(D3DXToRadian(θ)) * t;
		FLOAT _y_  = y + v0 * sin(D3DXToRadian(θ)) * t - 9.8f * pow(t, 2.0f) / 2.0f;
		FLOAT _vx_ =	 v0 * cos(D3DXToRadian(θ))				 ;
		FLOAT _vy_ =	 v0 * sin(D3DXToRadian(θ))	   - 9.8f * t;

		return  { _x_, _y_, v0, θ, _t_, dt, _vx_, _vy_ };
	}

	OscillatoryMotionOutputParameters CalculateOscillatoryMotion(OscillatoryMotionInputParameters oscillatoryMotionInputParameters)
	{
		auto [c0, t, dt, T, A, φ] = oscillatoryMotionInputParameters;

		FLOAT ω = 2.0f * D3DX_PI / T;
		FLOAT angle = ω * t + D3DXToRadian(φ);

		FLOAT _t_ = t + dt;
		FLOAT _c_ = c0 + A * cos(angle);
		FLOAT _v_ = -A * pow(ω, 1.0f) * sin(angle);
		FLOAT _a_ = -A * pow(ω, 2.0f) * cos(angle);

		return  { c0, _t_, dt, T, A, φ, _c_, _v_, _a_ };
	}

	UniformCircularMotionOutputParameters CalculateUniformCircularMotion(UniformCircularMotionInputParameters uniformCircularMotionInputParameters)
	{
		auto [r, ω, dω, xO, yO] = uniformCircularMotionInputParameters;

		FLOAT _ω_ = ω + dω;
		FLOAT _x_ = xO + r * cos(D3DXToRadian(ω));
		FLOAT _y_ = yO + r * sin(D3DXToRadian(ω));

		return { r, _ω_, dω, xO, yO, _x_, _y_ };
	}

	UniformlyAcceleratedMotionOutputParameters CalculateUniformlyAcceleratedMotion(UniformlyAcceleratedMotionInputParameters uniformlyAcceleratedMotionInputParameters)
	{
		auto [c, v, a, t, dt] = uniformlyAcceleratedMotionInputParameters;

		if (v * a <= 0)
		{
			a *= -1.0f;
		}
		FLOAT _t_ = t + dt;
		FLOAT _v_ = v + a * t;
		FLOAT _c_ = c + v * t + a * pow(t, 2.0f) / 2.0f;

		return { _c_, _v_, a, _t_, dt };
	}

	UniformlyDeceleratedMotionOutputParameters CalculateUniformlyDeceleratedMotion(UniformlyDeceleratedMotionInputParameters uniformlyDeceleratedMotionInputParameters)
	{
		auto [c, v, a, t, dt] = uniformlyDeceleratedMotionInputParameters;

		if (v * a >= 0)
		{
			a *= -1.0f;
		}
		FLOAT _t_ = t + dt;
		FLOAT _v_ = v + a * t;
		FLOAT _c_ = c + v * t + a * pow(t, 2.0f) / 2.0f;

		return { _c_, _v_, a, _t_, dt };
	}

};

