#pragma once

#include "Stage.h"

class Stage2 : public Stage
{

public:

	Stage2();
	virtual ~Stage2();

protected:

	virtual void LoadEntities(void*) override;

};

