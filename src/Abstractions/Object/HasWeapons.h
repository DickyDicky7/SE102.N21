#pragma once

#include "Common.h"
#include "Bullet.h"

struct BulletSpawnParams
{
	float x = 0.0f;
	float y = 0.0f;
	float angle = 0.0f;
	float vx = 0.0f;
	float vy = 0.0f;
	float ax = 0.0f;
	float ay = 0.0f;
	DIRECTION movingDirection = DIRECTION::RIGHT;
};

class HasWeapons
{

public:

	HasWeapons(                          );
	HasWeapons(BulletState* bulletState  );
	virtual ~HasWeapons();

	virtual void Fire(                                                                                                                 ) = 0;
	virtual void Fire(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection                 )    ;
	virtual void Fire(const BulletSpawnParams& params                                                                                  )    ;

	virtual void SetFiringTime(ULONGLONG firingTime);
	virtual void SetFiringRate(ULONGLONG firingRate);
	virtual ULONGLONG GetFiringTime() const;
	virtual ULONGLONG GetFiringRate() const;
	static std::vector<Bullet*>& GetBullets();
	virtual BulletState*& GetBulletState(); template <class T> requires std::derived_from<T, BulletState> void SetBulletState(T* bulletState);

protected:

	ULONGLONG _firingTime;
	ULONGLONG _firingRate;
	BulletState* _bulletState;
	static std::vector<Bullet*> _bullets;

};

template <class T> requires std::derived_from<T, BulletState>
inline void HasWeapons::SetBulletState(T* bulletState)
{
	Destroy(this->_bulletState);
			this->_bulletState = bulletState;
}
