#pragma once
#include "../RenderState.h"

class RenderStateVulkan : public RenderState
{
public:
	RenderStateVulkan();

	void setWireFrame(bool);

	void set();
};