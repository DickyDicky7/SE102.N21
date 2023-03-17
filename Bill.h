#pragma once

#include "Common.h"
#include "Entity.h"

class Bill : public Entity<Bill>
{

public:

	Bill();
	virtual ~Bill();
	void Update() override;

protected:

	GraphicsState* graphicsState;

};
