#pragma once
#include "TankEntry.h"

class CBot : public CTankEntry 
{
public:
	CBot();
	CBot(float x, float y, PCTSTR szImgName);
	virtual ~CBot();
};

