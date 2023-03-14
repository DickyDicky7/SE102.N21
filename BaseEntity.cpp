#include "BaseEntity.h"

BaseEntity* BaseEntity::SetPositionX(FLOAT x) { position.x = x; return this; }
BaseEntity* BaseEntity::SetPositionY(FLOAT y) { position.y = y; return this; }
BaseEntity* BaseEntity::SetPositionZ(FLOAT z) { position.z = z; return this; }
BaseEntity* BaseEntity::SetDistanceX(FLOAT distanceX) { this->distanceX = distanceX; return this; }
BaseEntity* BaseEntity::SetDistanceY(FLOAT distanceY) { this->distanceY = distanceY; return this; }
BaseEntity* BaseEntity::SetDistanceZ(FLOAT distanceZ) { this->distanceZ = distanceZ; return this; }

FLOAT BaseEntity::GetDistanceX() const { return distanceX; }
FLOAT BaseEntity::GetDistanceY() const { return distanceY; }
FLOAT BaseEntity::GetDistanceZ() const { return distanceZ; }
FLOAT BaseEntity::GetPositionX() const { return position.x; }
FLOAT BaseEntity::GetPositionY() const { return position.y; }
FLOAT BaseEntity::GetPositionZ() const { return position.z; }