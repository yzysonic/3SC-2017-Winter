#pragma once
#include "Core.h"

enum PlayerState
{
	PlayerStateIdle,
	PlayerStateRunning,
	PlayerStateKick,
	PlayerStateMax,
};

struct Player
{
	Transform transform;
	Rigidbody rigidbody;
	SkinnedModel model;
	D3DXVECTOR3 dir;
	Vector3 last_pos;
	PlayerState state;
	float targetAngle;

};

void InitPlayer(Player *_this);
void UpdatePlayer(Player *_this);
void DrawPlayer(Player *_this);
void SetPlayerState(Player *_this, PlayerState state);
void UninitPlayer(Player *_this);