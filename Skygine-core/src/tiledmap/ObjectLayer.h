#pragma once

#include <vector>

#include "Layer.h"
#include "TiledObject.h"
#include "../graphics/TextureManager.h"

class ObjectLayer : public Layer
{
	std::vector<TiledObject> m_objects;
	float m_scale;

public:

	ObjectLayer(std::vector<TiledObject> objects, float scale);

	void update() override;
	void render() override;
};
