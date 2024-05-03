#pragma once
#include <ogldev_math_3d.h>

class WorldTrans
{
public:
	WorldTrans() {}//constructor
	//setter functions. for scale rotation and position.
	void SetScale(float Scale);
	void SetRotation(float x, float y, float z);
	void SetPosition(float x, float y, float z);

	void Rotate(float x, float y, float z);

	//this function will generate the final world matrix
	//by combining all the set matrix.
	Matrix4f GetMatrix();

private:
	float m_Scale = 1.0f;
	Vector3f m_Rotation = Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f m_Pos = Vector3f(0.0f, 0.0f, 0.0f);
};