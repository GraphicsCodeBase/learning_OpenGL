#include "CameraClass.h"

void WorldTrans::SetScale(float Scale)
{
	m_Scale = Scale;
}

void WorldTrans::SetRotation(float x, float y, float z)
{
	//setting the rotation matrix
	m_Rotation.x = x;
	m_Rotation.y = y;
	m_Rotation.z = z;
}

void WorldTrans::SetPosition(float x, float y, float z)
{
	//setting the position.
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = z;
}

void WorldTrans::Rotate(float x, float y, float z)
{
	//setting the rotation matrix on realitive values.
	//providing the rotation in degrees.
	//it will change the current values you provide.
	m_Rotation.x += x;
	m_Rotation.y += y;
	m_Rotation.z += z;
}

Matrix4f WorldTrans::GetMatrix()
{
	//this function creates the scale , translation, and rotation matricies and combines them together.
	// it will then return the final world transform once done.
	Matrix4f Scale;
	Scale.InitScaleTransform(m_Scale, m_Scale, m_Scale);

	Matrix4f Rotation;
	Rotation.InitRotateTransform(m_Rotation.x, m_Rotation.y, m_Rotation.z);

	Matrix4f Translation;
	Translation.InitTranslationTransform(m_Pos.x, m_Pos.y, m_Pos.z);

	Matrix4f WorldTransform = Translation * Rotation * Scale;

	return WorldTransform;
	//for educational purporses , will optimise when needed.
}
