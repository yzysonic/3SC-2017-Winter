#pragma once
#include "Core.h"

struct Player
{
	Transform transform;
	SkinnedModel model;
	D3DXVECTOR3 dir;
	float targetAngle;

};

void InitPlayer(Player *_this);
void UpdatePlayer(Player *_this);
void DrawPlayer(Player *_this);
void UninitPlayer(Player *_this);