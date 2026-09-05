#include "Bullet.h"

void BulletBossStage2StateHead::SpawnBullets(float x, float y, float angle, float vx, float vy, float ax, float ay, DIRECTION movingDirection, std::vector<Bullet*>& bullets) const
{
	bullets.push_back(Bullet::Create(x, y, vx, vy, ax, ay, angle, movingDirection, true, new BulletBossStage2StateHead()));
	bullets.push_back(Bullet::Create(x, y, vx - 1.0f, vy, ax, ay, angle, movingDirection, true, new BulletBossStage2StateHead()));
	bullets.push_back(Bullet::Create(x, y, vx + 1.0f, vy, ax, ay, angle, movingDirection, true, new BulletBossStage2StateHead()));
}

