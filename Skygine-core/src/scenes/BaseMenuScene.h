#pragma once

#include "BaseScene.h"

class BaseMenuScene : public BaseScene
{
public:

	virtual void update(float delta) = 0;
	virtual void draw() = 0;
	virtual void dispose() = 0;
};
