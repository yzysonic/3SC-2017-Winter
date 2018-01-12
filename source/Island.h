#pragma once

#include "Core.h"
#include "Model.h"

struct Island
{
	Model model;
	MeshCollider collider;
};

void InitIsland(Island* _this);
void UpdateIsland(Island* _this);
void DrawIsland(Island* _this);
void UninitIsland(Island* _this);