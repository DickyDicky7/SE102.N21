#include "HasWeapons.h"

HasWeapons::HasWeapons(                        )
{
	firingTime = 000;
	firingRate = 150;
	this->bulletState = NULL;
}

HasWeapons::HasWeapons(BulletState* bulletState) : HasWeapons()
{
	this->bulletState = bulletState;
		  bulletState = NULL;
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
		Bullet* bullet = NULL;
		BulletState* newState = NULL;

		bullet = new Bullet();
		if (!this->bulletState)
			 this->bulletState = new BulletRState();

		bullet->SetX(x); bullet->SetY(y); bullet->SetVX(vx); bullet->SetVY(vy); bullet->SetAX(ax); bullet->SetAY(ay);
		bullet->SetAngle(angle); bullet->SetMovingDirection(movingDirection);

		if (dynamic_cast<BulletFState*>(bulletState))
		{
			newState = new BulletFState();
		}
		if (dynamic_cast<BulletLState*>(bulletState))
		{
			newState = new BulletLState();
		}
		if (dynamic_cast<BulletMState*>(bulletState))
		{
			newState = new BulletMState();
		}
		if (dynamic_cast<BulletRState*>(bulletState))
		{
			newState = new BulletRState();
		}
		if (dynamic_cast<BulletSState*>(bulletState))
		{
			newState = new BulletSState();

			for (auto& spreadDegree : BulletSState::spreadDegrees)
			{
				Bullet* spreadBullet = new Bullet();
				spreadBullet->SetMovingDirection(movingDirection);
				spreadBullet->SetAngle(angle);
				spreadBullet->SetX(x);
				spreadBullet->SetY(y);
				spreadBullet->SetVX(vx * std::cos(D3DXToRadian(spreadDegree)) - vy * std::sin(D3DXToRadian(spreadDegree)));
				spreadBullet->SetVY(vx * std::sin(D3DXToRadian(spreadDegree)) + vy * std::cos(D3DXToRadian(spreadDegree)));
				spreadBullet->SetAX(ax);
				spreadBullet->SetAY(ay);
				spreadBullet->SetState(new BulletSState()); bullets.push_back(spreadBullet);
			}
		}

		bullet->SetState(newState); bullets.push_back(bullet);
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

std::list<Bullet*> HasWeapons::bullets;