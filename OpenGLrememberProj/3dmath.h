#ifndef _3DMATH_H
#define _3DMATH_H// Эта структура используется для хранения 3д точек и векторов. Она использовалась
// в наших уроках по камере, вернитесь назад, чтобы узнать подробности.
struct CVector3
{
public:
	float x, y, z;
};
// Возвращает вектор, перпендикулярный двум переданным векторам (плоскости)
CVector3 Cross(CVector3 vVector1, CVector3 vVector2);

// Возвращает вектор между 2мя точками.
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2);

// Возвращает величину нормали или любого другого вектора
float Magnitude(CVector3 vNormal);

// Возвращает нормализованный вектор (его длинна становится равной 1)
CVector3 Normalize(CVector3 vNormal);

// Возвращает нормаль полигона (направление, куда повернут полигон)
CVector3 Normal(CVector3 vTriangle[]);

#endif