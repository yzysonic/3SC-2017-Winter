#include "CollisionTest.h"

Vector3 CollisionTestPolyAndLine(const PolyCollider & collider, const Vector3 & linePos, const Vector3 & lineVec)
{
	Vector3 normal;	// �|���S���̖@��

	normal = Vector3::Cross((collider.vtx[1] - collider.vtx[0]), (collider.vtx[2] - collider.vtx[0])).normalized();	// �O�ςŖ@�������߂Đ��K������


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
		return Vector3::zero;	// �������Ă���\���͖���(�������Ă��Ȃ�)


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
			return Vector3::zero;	// �Ԃ����ĂȂ�

	}

	// �Ԃ����Ă���
	//return vecFromP0ToPos1 - posIntersectionPlaneToLine;
	return lineVec * (internalRatio - 1.0f);

}
