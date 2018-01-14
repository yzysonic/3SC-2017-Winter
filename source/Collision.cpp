#include "Collision.h"

bool CollisionTestPolyAndLine(PolyCollider & collider, const Vector3 & linePos, const Vector3 & lineVec, Collision* pOutCollision)
{
	Vector3 normal;	// ポリゴンの法線

	//normal = Vector3::Cross((collider.vtx[1] - collider.vtx[0]), (collider.vtx[2] - collider.vtx[0])).normalized();	// 外積で法線を求めて正規化する
	normal = collider.normal;

	// 平面の正面から衝突していない
	if (Vector3::Dot(normal, lineVec) > 0)
		return false;

	Vector3 vecFromP0ToPos0 = linePos - collider.vtx[0];				// ポリゴン頂点0から線分の始点までのベクトル
	Vector3 vecFromP0ToPos1 = (linePos + lineVec) - collider.vtx[0];	// ポリゴン頂点0から線分の終点までのベクトル


	// 平面と線分の内積とって衝突している可能性を調べる
	// 求めた法線とベクトル２つ（線分の両端とポリゴン上の任意の点）の
	// 内積とって衝突している可能性を調べる

	// 法線と始点、終点との各内積
	float dotPos0ToNor = Vector3::Dot(vecFromP0ToPos0, normal);
	float dotPos1ToNor = Vector3::Dot(vecFromP0ToPos1, normal);

	// 衝突している場合、始点、終点が面の表裏に存在することになり、
	// なす角がそれぞれ鋭角、鈍角になるので内積がそれぞれ異なる符号
	// になる。異なる符号を掛け合わせると必ず負になることを利用
	if (!(dotPos0ToNor*dotPos1ToNor <= 0))
		return false;	// 当たっている可能性は無い(当たっていない)


	// 以下、詳細なチェック //

	// ポリゴンと線分の交点を求める

	// 内分比を求める
	float distance0 = fabsf(dotPos0ToNor);	// pos0から平面までの距離
	float distance1 = fabsf(dotPos1ToNor);	// pos1から平面までの距離
	float internalRatio = distance0 / (distance0 + distance1);	// 内分比


	// p0から交点へのベクトル
	Vector3 vecFromP0ToPosIntersection = ((1.0f - internalRatio)*vecFromP0ToPos0) + (internalRatio*vecFromP0ToPos1);


	// 交点を求める
	Vector3 posIntersectionPlaneToLine = collider.vtx[0] + vecFromP0ToPosIntersection;


	// 各辺と交点の関係を調べる //
	const int VertexNum = 3;

	// ポリゴンの各辺のベクトル
	Vector3 vec[VertexNum];
	vec[0] = collider.vtx[1] - collider.vtx[0];
	vec[1] = collider.vtx[2] - collider.vtx[1];
	vec[2] = collider.vtx[0] - collider.vtx[2];

	// 各頂点と交点とのベクトル
	Vector3 vecFromIntersection[VertexNum];
	vecFromIntersection[0] = posIntersectionPlaneToLine - collider.vtx[0];
	vecFromIntersection[1] = posIntersectionPlaneToLine - collider.vtx[1];
	vecFromIntersection[2] = posIntersectionPlaneToLine - collider.vtx[2];


	// ３つのベクトルに対し最終チェック
	for (int i = 0; i < VertexNum; i++)
	{
		// 各辺と交点へのベクトルの外積で法線を求めて正規化する
		Vector3 nor = Vector3::Cross(vec[i], vecFromIntersection[i]).normalized();
		// 内積で向きが合っているか調べる
		if (Vector3::Dot(nor, normal) < 0)
			return false;	// ぶつかってない

	}

	// ぶつかっている
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