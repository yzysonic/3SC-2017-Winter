#include "Island.h"

void InitIsland(Island* _this)
{
	InitModel(&_this->model, "island.x");

	_this->model.transform.pos.y = -0.01f;
	UpdateTransformMtx(&_this->model.transform);

	// �R���C�_�[�p�̃��f����ǂݍ���
	Model model_collider;
	InitModel(&model_collider, "island_collider.x");

	// �R���C�_�[�̏�����
	InitMeshColliderByModel(&_this->collider, &model_collider);

	UninitModel(&model_collider);
}

void UpdateIsland(Island* _this)
{
}

void DrawIsland(Island* _this)
{
	DrawModel(&_this->model);
}

void UninitIsland(Island* _this)
{
	UninitModel(&_this->model);
	UninitMeshCollider(&_this->collider);
}
