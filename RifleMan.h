#pragma once
#include "HasTextures.h"

class RifleMan;


class RifleMan : public HasTextures<RifleMan>
{
public:
	RifleMan();
	virtual ~RifleMan();

	static void LoadTextures();
};
