#pragma once
#include "Core.h"
#include "Shadow.h"

enum PlayerState
{
	PlayerStateIdle,
	PlayerStateRunning,
	PlayerStateJumping,
	PlayerStateKick,
	PlayerStateDeath,
	PlayerStateOutOfField,
	PlayerStateMax,
};

struct Player
{
	Transform transform;
	Rigidbody rigidbody;
	SkinnedModel model;
	Vector3 dir;
	Shadow shadow;
	PlayerState state;
	SphereCollider collider_foot;
	Bone *bone_foot;
	Model sphere;
	float targetAngle;
	float state_timer;
	bool isMoveable;
};

void InitPlayer(Player *_this);
void UpdatePlayer(Player *_this);
void DrawPlayer(Player *_this);
void SetPlayerState(Player *_this, PlayerState state);
void UninitPlayer(Player *_this);