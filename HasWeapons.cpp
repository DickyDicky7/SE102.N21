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
			bullet->isEnemy = 0;
			newState = new BulletFState();
		}
		else
		if (dynamic_cast<BulletLState*>(bulletState))
		{
			bullet->isEnemy = 0;
			newState = new BulletLState();
		}
		else
		if (dynamic_cast<BulletMState*>(bulletState))
		{
			bullet->isEnemy = 0;
			newState = new BulletMState();
		}
		else
		if (dynamic_cast<BulletRState*>(bulletState))
		{
			bullet->isEnemy = 0;
			newState = new BulletRState();
		}
		else
		if (dynamic_cast<BulletSState*>(bulletState))
		{
			bullet->isEnemy = 0;
			newState = new BulletSState();

			for (auto& spreadDegree : BulletSState::spreadDegrees)
			{
				Bullet* spreadBullet = new Bullet();
				spreadBullet->isEnemy = 0;
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
		else
		if (dynamic_cast<BulletEnemyState*>(bulletState))
		{
			bullet->isEnemy = 1;
			newState = new BulletEnemyState();
		}
		else
		if (dynamic_cast<BulletBossStage1State*>(bulletState))
		{
			bullet->isEnemy = 1;
			newState = new BulletBossStage1State();
		}
		else
		if (dynamic_cast<BulletBossStage2State*>(bulletState))
		{
			bullet->isEnemy = 1;
			newState = new BulletBossStage2State();

			//for (int i = 0; i < 2; i++)
			//{
			//	Bullet* spreadBullet = new Bullet();
			//	spreadBullet->isEnemy = 1;
			//	spreadBullet->SetMovingDirection(movingDirection);
			//	spreadBullet->SetAngle(angle);
			//	spreadBullet->SetX(x);
			//	spreadBullet->SetY(y);
			//	spreadBullet->SetVY(vy);
			//	spreadBullet->SetAX(ax);
			//	spreadBullet->SetAY(ay);
			//	if(i == 0) spreadBullet->SetVX(vx - 1.0f); // trai
			//	else spreadBullet->SetVX(vx + 1.0f); // phai
			//	spreadBullet->SetState(new BulletBossStage2State()); bullets.push_back(spreadBullet);
			//}
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