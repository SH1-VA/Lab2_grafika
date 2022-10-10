#ifndef _3DMATH_H
#define _3DMATH_H// ��� ��������� ������������ ��� �������� 3� ����� � ��������. ��� ��������������
// � ����� ������ �� ������, ��������� �����, ����� ������ �����������.
struct CVector3
{
public:
	float x, y, z;
};
// ���������� ������, ���������������� ���� ���������� �������� (���������)
CVector3 Cross(CVector3 vVector1, CVector3 vVector2);

// ���������� ������ ����� 2�� �������.
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2);

// ���������� �������� ������� ��� ������ ������� �������
float Magnitude(CVector3 vNormal);

// ���������� ��������������� ������ (��� ������ ���������� ������ 1)
CVector3 Normalize(CVector3 vNormal);

// ���������� ������� �������� (�����������, ���� �������� �������)
CVector3 Normal(CVector3 vTriangle[]);

#endif