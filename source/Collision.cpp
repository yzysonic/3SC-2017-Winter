#include "Collision.h"

bool CollisionTestPolyAndLine(PolyCollider & collider, const Vector3 & linePos, const Vector3 & lineVec, Collision* pOutCollision)
{
	Vector3 normal;	// �|���S���̖@��

	//normal = Vector3::Cross((collider.vtx[1] - collider.vtx[0]), (collider.vtx[2] - collider.vtx[0])).normalized();	// �O�ςŖ@�������߂Đ��K������
	normal = collider.normal;

	// ���ʂ̐��ʂ���Փ˂��Ă��Ȃ�
	if (Vector3::Dot(normal, lineVec) > 0)
		return false;

	Vector3 vecFromP0ToPos0 = linePos - collider.vtx[0];				// �|���S�����_0��������̎n�_�܂ł̃x�N�g��
	Vector3 vecFromP0ToPos1 = (linePos + lineVec) - collider.vtx[0];	// �|���S�����_0��������̏I�_�܂ł̃x�N�g��


	// ���ʂƐ����̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
	// ���߂��@���ƃx�N�g���Q�i�����̗��[�ƃ|���S����̔C�ӂ̓_�j��
	// ���ςƂ��ďՓ˂��Ă���\���𒲂ׂ�

	// �@���Ǝn�_�A�I�_�Ƃ̊e����
	float dotPos0ToNor = Vector3::Dot(vecFromP0ToPos0, normal);
	float dotPos1ToNor = Vector3::Dot(vecFromP0ToPos1, normal);

	// �Փ˂��Ă���ꍇ�A�n�_�A�I�_���ʂ̕\���ɑ��݂��邱�ƂɂȂ�A
	// �Ȃ��p�����ꂼ��s�p�A�݊p�ɂȂ�̂œ��ς����ꂼ��قȂ镄��
	// �ɂȂ�B�قȂ镄�����|�����킹��ƕK�����ɂȂ邱�Ƃ𗘗p
	if (!(dotPos0ToNor*dotPos1ToNor <= 0))
		return false;	// �������Ă���\���͖���(�������Ă��Ȃ�)


	// �ȉ��A�ڍׂȃ`�F�b�N //

	// �|���S���Ɛ����̌�_�����߂�

	// ����������߂�
	float distance0 = fabsf(dotPos0ToNor);	// pos0���畽�ʂ܂ł̋���
	float distance1 = fabsf(dotPos1ToNor);	// pos1���畽�ʂ܂ł̋���
	float internalRatio = distance0 / (distance0 + distance1);	// ������


	// p0�����_�ւ̃x�N�g��
	Vector3 vecFromP0ToPosIntersection = ((1.0f - internalRatio)*vecFromP0ToPos0) + (internalRatio*vecFromP0ToPos1);


	// ��_�����߂�
	Vector3 posIntersectionPlaneToLine = collider.vtx[0] + vecFromP0ToPosIntersection;


	// �e�ӂƌ�_�̊֌W�𒲂ׂ� //
	const int VertexNum = 3;

	// �|���S���̊e�ӂ̃x�N�g��
	Vector3 vec[VertexNum];
	vec[0] = collider.vtx[1] - collider.vtx[0];
	vec[1] = collider.vtx[2] - collider.vtx[1];
	vec[2] = collider.vtx[0] - collider.vtx[2];

	// �e���_�ƌ�_�Ƃ̃x�N�g��
	Vector3 vecFromIntersection[VertexNum];
	vecFromIntersection[0] = posIntersectionPlaneToLine - collider.vtx[0];
	vecFromIntersection[1] = posIntersectionPlaneToLine - collider.vtx[1];
	vecFromIntersection[2] = posIntersectionPlaneToLine - collider.vtx[2];


	// �R�̃x�N�g���ɑ΂��ŏI�`�F�b�N
	for (int i = 0; i < VertexNum; i++)
	{
		// �e�ӂƌ�_�ւ̃x�N�g���̊O�ςŖ@�������߂Đ��K������
		Vector3 nor = Vector3::Cross(vec[i], vecFromIntersection[i]).normalized();
		// ���ςŌ����������Ă��邩���ׂ�
		if (Vector3::Dot(nor, normal) < 0)
			return false;	// �Ԃ����ĂȂ�

	}

	// �Ԃ����Ă���
	if (pOutCollision)
	{
		pOutCollision->normal = normal;
		pOutCollision->offset = lineVec * (internalRatio - 1.0f);
		pOutCollision->point = linePos + vecFromP0ToPosIntersection;
	}
	collider.isCollidinig = true;
	return true;

}

bool CollisionTestMeshAndLine(MeshCollider & collider, const Vector3 & linePos, const Vector3 & lineVec, Collision * pOutCollision)
{
	for (int i = 0; i < collider.polyNum; i++)
	{
		if (CollisionTestPolyAndLine(collider.polyList[i], linePos, lineVec, pOutCollision))
			return true;
	}
	return false;
}

bool CollisionTestSphere(SphereCollider & collider_a, SphereCollider & collider_b, Collision * pOutCollision)
{
	float distance = (collider_a.pos-collider_b.pos).sqrLength();
	if (distance > (collider_a.radius + collider_b.radius) * (collider_a.radius + collider_b.radius))
		return false;

	if (pOutCollision)
	{
		pOutCollision->normal = (collider_a.pos - collider_b.pos).normalized();
		pOutCollision->offset = (collider_a.radius + collider_b.radius - distance) * pOutCollision->normal;
	}

	return true;
}

void CollisionResponse(const Collision *collision, Rigidbody *rb)
{
	Vector3 move = -collision->offset;
	move = move - Vector3::Dot(move, collision->normal)*collision->normal;
	rb->pos += collision->offset + move + 0.001f*collision->normal;
	rb->parent->pos = rb->pos;
	auto t = rb->vel + (1.0f + rb->bounciness)*Vector3::Dot(-rb->vel, collision->normal)*collision->normal;
	rb->vel = rb->vel + (1.0f + rb->bounciness)*Vector3::Dot(-rb->vel, collision->normal)*collision->normal;

	float dot_nor_force = Vector3::Dot(rb->net_force, collision->normal);
	//if (dot_nor_force < 0)
	//	rb->force += -dot_nor_force*collision->normal;
	
}