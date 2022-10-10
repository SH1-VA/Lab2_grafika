#include "3dmath.h"

#include <math.h>// * ���������� ������� �������� *
// ����� ����� ������� ��������, ��� ����� ����� ��������� cross-a �� ����
// �������� ����� ��������. � �����, ��� ��, ��� ��� ����� ��� ��������� �����������
// ���� ������ ������������. � ����� ������, ������ � ��� ������ ����������� � ������.
// ������ ������� � ����� ������ �� �����. ��� ����� ������ ������ �����������.
// ����, ���� 2 ������� ������������, �� ����� ����� ������, ������� ���������������
// � ��������.
// ������, � ����������� �� ������� ���������� ������, ������� ����� ����������� �
// �����-�� �� ������ ��������. ��� ������� ������ ������, � ����� ������� ������������
// ������� � ������ ����������� ���.
// ������ �������� �������������� ������ � ����� �������. ������ �� ����� ������ ���
// ������������ �������, ������� �� �����. �����������, ���� � ��� ���� �����-������ 3�
// ������, ����� �� ��� ������������ ���������� ������� � ���������? ������� ���,
// ��� ������������.
//

/////////////////////////////////////// CROSS \\\\\*
/////
/////   ���������� ������, ���������������� 2� ����������.
/////
/////////////////////////////////////// CROSS \\\\\*

CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vNormal;                       // �������������� ������

	// ��� ���, ���� � ��� ���� 2 ������� (2 ������� ��������), � ��� ���� ���������.
	// cross ������� ������, ���������������� ���������, ������������ 2�� ���������.
	// ������� � �������� ������, �� ������ ��� �����������:

	// �������� X ��� ������� ����������� ���:  (V1.y * V2.z) � (V1.z * V2.y)
	vNormal.x = ((vVector1.y * vVector2.z) -(vVector1.z * vVector2.y));

	// �������� Y ��� ������� ����������� ���:  (V1.z * V2.x) � (V1.x * V2.z)
	vNormal.y = ((vVector1.z * vVector2.x) -(vVector1.x * vVector2.z));

	// �������� Z ��� ������� ����������� ���:  (V1.x * V2.y) � (V1.y * V2.x)
	vNormal.z = ((vVector1.x * vVector2.y) -(vVector1.y * vVector2.x));

	return vNormal;  // ���������� ��������� (�����������, ���� ��������� ������� � �������)
}

/////////////////////////////////////// VECTOR \\\\\*
/////
/////   ���������� ������ ����� 2�� �������.
/////
/////////////////////////////////////// VECTOR \\\\\*

CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2)
{
	CVector3 vVector = { 0 };

	// ����� �������� ������ ����� 2 ������� (�����������), ����� ������� ������
	// ����� �� ������.

	vVector.x = vPoint1.x - vPoint2.x;
	vVector.y = vPoint1.y - vPoint2.y;
	vVector.z = vPoint1.z - vPoint2.z;

	// ������ ���������� ���������� ���������

	return vVector;
}

/////////////////////////////////////// MAGNITUDE \\\\\*
/////
/////   ���������� �������� �������
/////
/////////////////////////////////////// MAGNITUDE \\\\\*

float Magnitude(CVector3 vNormal)
{
	return (float)sqrt((vNormal.x * vNormal.x) +
		(vNormal.y * vNormal.y) +
		(vNormal.z * vNormal.z));
}

/////////////////////////////////////// NORMALIZE \\\\\*
/////
/////   ���������� ��������������� ������ (� ������� 1)
/////
/////////////////////////////////////// NORMALIZE \\\\\*

CVector3 Normalize(CVector3 vNormal)
{
	float magnitude = Magnitude(vNormal);

	vNormal.x /= magnitude;
	vNormal.y /= magnitude;
	vNormal.z /= magnitude;

	return vNormal;
}

/////////////////////////////////////// NORMAL \\\\\*
/////
/////   ���������� ������� ��������
/////
/////////////////////////////////////// NORMAL \\\\\*

CVector3 Normal(CVector3 vTriangle[])
{
	CVector3 vVector1 = Vector(vTriangle[2], vTriangle[0]);
	CVector3 vVector2 = Vector(vTriangle[1], vTriangle[0]);

	// � ������� ���������� ��� ������� � �����������. �� �������� vVector1 � vVector2 � ���
	// �������. ������, ���� 2 ������� ������������, �� ����� �������� �� ��� cross().
	// (*���������*) �����: ������ �������� �� ������� ��� ������������, � ������ � �����
	// �������. ���� �� �������� �� �������, ������� ����� ��������� � ���������������
	// �������. � ����� ������ �� ������� ������� ������ �������� ������ �������.

	CVector3 vNormal = Cross(vVector1, vVector2);

	// ������, ���� ����������� �������, �������� ������� ��������� ����. ������ �
	// ������ ����������, ��� ����� ���� ����� �������. �� ������� � ������ 1, ���
	// ���������� ������������. ����� ������� ���, �� ����� ������� �� � ������.
	// �� � ��� ����� ������? �� ���������� ��� �������: magnitude = sqrt(x^2 + y^2 + z^2)

	vNormal = Normalize(vNormal);

	// ������ ������ ���������������� �������� =)
	// (*����������*) ���� �� ������ �������, ��� �������� ������������, ���������������
	// ���������� �����. �� �������, ��� ������ ������� �� ������������. � ����� ����������
	// ������ ������������ ��� �������. � ���������, �������, ������ ������ �������
	// (�������, ����� (0,0,0)), ���� �� � �����������, ��� ������ ����� ����� 1.

	return vNormal;
}