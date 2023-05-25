#pragma once

#include "Stage.h"

class Stage1 : public Stage
{

public:

	Stage1();
	virtual ~Stage1();

protected:

	virtual void LoadEntities(void*) override;

};

