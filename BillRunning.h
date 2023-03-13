#pragma once

#include "BaseSprites.h"

class BillRunning : public BaseSprites
{

public:

	BillRunning();
	BillRunning(int, int, int, int, int, int, int, int, int);

	void BuildSurfaces(LPDIRECT3DDEVICE9);

};

