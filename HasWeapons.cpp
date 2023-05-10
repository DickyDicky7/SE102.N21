#include "HasWeapons.h"

HasWeapons::HasWeapons(                        )
{
	firingTime = 0;
	firingRate = 150;
	this->bulletState = NULL;
}

HasWeapons::HasWeapons(BulletState* bulletState)
{
	firingTime = 0;
	firingRate = 150;
	this->bulletState = bulletState;
}

HasWeapons::~HasWeapons()
{
	for (auto& bullet : bullets) Destroy(bullet); bullets.clear();
	Destroy(bulletState);
}

void HasWeapons::Fire(FLOAT x, FLOAT y, FLOAT angle, FLOAT vx, FLOAT vy, FLOAT ax, FLOAT ay, DIRECTION movingDirection)
{
	ULONGLONG now = GetTickCount64(); if (firingTime == 0) firingTime = now; if (now - firingTime > firingRate)
	{
		BulletState* copyBulletState = new BulletRState(); *copyBulletState = *this->bulletState; Bullet* bullet = new Bullet(copyBulletState);
		bullet->SetX(x); bullet->SetY(y); bullet->SetVX(vx); bullet->SetVY(vy); bullet->SetAX(ax); bullet->SetAY(ay);
		bullet->SetAngle(angle); bullet->SetMovingDirection(movingDirection); bullets.push_back(bullet);
		firingTime = now;
	}
}

void HasWeapons::SetFiringTime(ULONGLONG firingTime)
{
	this->firingTime = firingTime;
}

void HasWeapons::SetFiringRate(ULONGLONG firingRate)
{
	this->firingRate = firingRate;
}

ULONGLONG HasWeapons::GetFiringTime() const
{
	return firingTime;
}

ULONGLONG HasWeapons::GetFiringRate() const
{
	return firingRate;
}

BulletState*& HasWeapons::GetBulletState()
{
	return bulletState;
}

std::list<Bullet*>& HasWeapons::GetBullets()
{
	return bullets;
}
