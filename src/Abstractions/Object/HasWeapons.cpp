#include "HasWeapons.h"

HasWeapons::HasWeapons(                        )
{
	this->_firingTime = 0;
	this->_firingRate = Constants::Weapons::DEFAULT_FIRING_RATE_MILLISECONDS;
	this->_bulletState = nullptr;
}

HasWeapons::HasWeapons(BulletState* bulletState) : HasWeapons()
{
	this->_bulletState = bulletState;
}

HasWeapons::~HasWeapons()
{
	Destroy(this->_bulletState);
}

void HasWeapons::Fire(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection)
{
	ULONGLONG now = GetTickCount64();
	if (this->_firingTime == 0) this->_firingTime = now;
	if (now - this->_firingTime >= this->_firingRate)
	{
		if (!this->_bulletState)
			this->_bulletState = new BulletRState();

		this->_bulletState->SpawnBullets(x, y, angle, vx, vy, ax, ay, movingDirection, _bullets);
		this->_firingTime = now;
	}
}

void HasWeapons::Fire(const BulletSpawnParams& params)
{
	this->Fire(params.x, params.y, params.angle, params.vx, params.vy, params.ax, params.ay, params.movingDirection);
}

void HasWeapons::SetFiringTime(ULONGLONG firingTime)
{
	this->_firingTime = firingTime;
}

void HasWeapons::SetFiringRate(ULONGLONG firingRate)
{
	this->_firingRate = firingRate;
}

ULONGLONG HasWeapons::GetFiringTime() const
{
	return this->_firingTime;
}

ULONGLONG HasWeapons::GetFiringRate() const
{
	return this->_firingRate;
}

BulletState*& HasWeapons::GetBulletState()
{
	return this->_bulletState;
}

std::vector<Bullet*>& HasWeapons::GetBullets()
{
	return _bullets;
}

std::vector<Bullet*> HasWeapons::_bullets;
