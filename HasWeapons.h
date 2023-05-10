#pragma once

#include "Common.h"
#include "Bullet.h"

class HasWeapons
{

public:

	HasWeapons(            );
	HasWeapons(BulletState*);
	virtual ~HasWeapons();
	
	virtual void Fire(                                                          ) = 0;
	virtual void Fire(FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, DIRECTION)    ;

	virtual void SetFiringTime(ULONGLONG);
	virtual void SetFiringRate(ULONGLONG);
	virtual ULONGLONG GetFiringTime() const;
	virtual ULONGLONG GetFiringRate() const;
	virtual std::list<Bullet*>& GetBullets();
	virtual BulletState*& GetBulletState(); template <class T> requires std::derived_from<T, BulletState> void SetBulletState(T*);

protected:

	ULONGLONG firingTime;
	ULONGLONG firingRate;
	BulletState* bulletState; std::list<Bullet*> bullets;

};

template <class T> requires std::derived_from<T, BulletState>
inline void HasWeapons::SetBulletState(T* bulletState)
{
	Destroy(this->bulletState);
			this->bulletState = bulletState;
}
