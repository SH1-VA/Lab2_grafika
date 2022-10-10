#include "3dmath.h"

#include "Render.h"

#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "GUItextRectangle.h"

bool textureMode = true;
bool lightMode = true;

//����� ��� ��������� ������
class CustomCamera : public Camera
{
public:
	//��������� ������
	double camDist;
	//���� �������� ������
	double fi1, fi2;

	
	//������� ������ �� ���������
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//������� ������� ������, ������ �� ����� ��������, ���������� �������
	void SetUpCamera()
	{
		//�������� �� ������� ������ ������
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//������� ��������� ������
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //������� ������ ������


//����� ��� ��������� �����
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//��������� ������� �����
		pos = Vector3(1, 1, 3);
	}

	
	//������ ����� � ����� ��� ���������� �����, ���������� �������
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();
		
		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//����� �� ��������� ����� �� ����������
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//������ ���������
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// ��������� ��������� �����
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// �������������� ����������� �����
		// ������� ��������� (���������� ����)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// ��������� ������������ �����
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// ��������� ���������� ������������ �����
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //������� �������� �����




//������ ���������� ����
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//������ ���� ������ ��� ������� ����� ������ ����
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//������� ���� �� ���������, � ����� ��� ����
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;

}

void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}



GLuint texId;

//����������� ����� ������ ��������
void initRender(OpenGL *ogl)
{
	//��������� �������

	//4 ����� �� �������� �������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//��������� ������ ��������� �������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//�������� ��������
	glEnable(GL_TEXTURE_2D);
	

	//������ ����������� ���������  (R G B)
	RGBTRIPLE *texarray;

	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	char *texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

	
	
	//���������� �� ��� ��������
	glGenTextures(1, &texId);
	//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
	glBindTexture(GL_TEXTURE_2D, texId);

	//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//�������� ������
	free(texCharArray);
	free(texarray);

	//������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//������ � ���� ����������� � "������"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// ������������ �������� : �� ����� ����� ����� 1
	glEnable(GL_NORMALIZE);

	// ���������� ������������� ��� �����
	glEnable(GL_LINE_SMOOTH); 


	//   ������ ��������� ���������
	//  �������� GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  ������� � ���������� �������� ���������(�� ���������), 
	//                1 - ������� � ���������� �������������� ������� ��������       
	//                �������������� ������� � ���������� ��������� ����������.    
	//  �������� GL_LIGHT_MODEL_AMBIENT - ������ ������� ���������, 
	//                �� ��������� �� ���������
	// �� ��������� (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}





void Render(OpenGL *ogl)
{



	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	//��������������
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//��������� ���������
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;


	//�������
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//��������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//����������
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		//������ �����
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//���� ���� �������, ��� ����������� (����������� ���������)
	glShadeModel(GL_SMOOTH);
	//===================================
	//������� ���  


	//������ ��������� ���������� ��������
	/*double A[2] = { -4, -4 };
	double B[2] = { 4, -4 };
	double C[2] = { 4, 4 };
	double D[2] = { -4, 4 };

	glBindTexture(GL_TEXTURE_2D, texId);

	glColor3d(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);

	glNormal3d(0, 0, 1);
	glTexCoord2d(0, 0);
	glVertex2dv(A);
	glTexCoord2d(1, 0);
	glVertex2dv(B);
	glTexCoord2d(1, 1);
	glVertex2dv(C);
	glTexCoord2d(0, 1);
	glVertex2dv(D);

	glEnd();*/
	//����� ��������� ���������� ��������

	//������ ��������� ������

	CVector3 A1{ 0, 0, 0 };
	CVector3 A2{ 7, -4, 0 };
	CVector3 A3{ 9, 4, 0 };
	CVector3 A4{ 3, 8, 0 };
	CVector3 A5{ -1, 6, 0 };
	CVector3 A6{ -1, 2, 0 };
	CVector3 A7{ -9, 1, 0 };
	CVector3 A8{ -3, -5, 0 };

	CVector3 B1{ 0, 0, 1 };
	CVector3 B2{ 7, -4, 1 };
	CVector3 B3{ 9, 4, 1 };
	CVector3 B4{ 3, 8, 1 };
	CVector3 B5{ -1, 6, 1 };
	CVector3 B6{ -1, 2, 1 };
	CVector3 B7{ -9, 1, 1 };
	CVector3 B8{ -3, -5, 1 };

	//����
	glPushMatrix();
	glTranslated(0, 0, 1);
	glBegin(GL_TRIANGLE_FAN);
	CVector3 normTempVec = Normalize(Cross(Vector(A1, A2), Vector(A1, A3)));
	glNormal3d(normTempVec.x, normTempVec.y, normTempVec.z);
	glColor3d(0, 0.7, 0.3);
	glVertex3f(A1.x, A1.y, A1.z);
	glVertex3f(A2.x, A2.y, A2.z);
	glVertex3f(A3.x, A3.y, A3.z);
	glVertex3f(A4.x, A4.y, A4.z);
	glVertex3f(A5.x, A5.y, A5.z);
	glVertex3f(A6.x, A6.y, A6.z);
	glVertex3f(A7.x, A7.y, A7.z);
	glVertex3f(A8.x, A8.y, A8.z);
	glEnd();
	glPopMatrix();

	//���
	glPushMatrix();
	glTranslated(0, 0, 0);
	glBegin(GL_TRIANGLE_FAN);
	normTempVec = Normalize(Cross(Vector(A1, A3), Vector(A1, A2)));
	glNormal3d(normTempVec.x, normTempVec.y, normTempVec.z);
	glColor3d(0, 0.7, 0.3);
	glVertex3f(A1.x, A1.y, A1.z);
	glVertex3f(A2.x, A2.y, A2.z);
	glVertex3f(A3.x, A3.y, A3.z);
	glVertex3f(A4.x, A4.y, A4.z);
	glVertex3f(A5.x, A5.y, A5.z);
	glVertex3f(A6.x, A6.y, A6.z);
	glVertex3f(A7.x, A7.y, A7.z);
	glVertex3f(A8.x, A8.y, A8.z);
	glEnd();
	glPopMatrix();

	//������� �����
	glColor3d(0, 0.3, 0.7);
	glBegin(GL_QUAD_STRIP);
	normTempVec = Normalize(Cross(Vector(A1, A2), Vector(A1, B1)));
	glNormal3d(normTempVec.x, normTempVec.y, normTempVec.z);
	glVertex3f(A1.x, A1.y, A1.z);
	glVertex3f(B1.x, B1.y, B1.z);
	glVertex3f(A2.x, A2.y, A2.z);
	glVertex3f(B2.x, B2.y, B2.z);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	normTempVec = Normalize(Cross(Vector(A2, A3), Vector(A2, B2)));
	glNormal3d(normTempVec.x, normTempVec.y, normTempVec.z);
	glVertex3f(A2.x, A2.y, A2.z);
	glVertex3f(B2.x, B2.y, B2.z);
	glVertex3f(A3.x, A3.y, A3.z);
	glVertex3f(B3.x, B3.y, B3.z);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	normTempVec = Normalize(Cross(Vector(A3, A4), Vector(A3, B3)));
	glNormal3d(normTempVec.x, normTempVec.y, normTempVec.z);
	glVertex3f(A3.x, A3.y, A3.z);
	glVertex3f(B3.x, B3.y, B3.z);
	glVertex3f(A4.x, A4.y, A4.z);
	glVertex3f(B4.x, B4.y, B4.z);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	normTempVec = Normalize(Cross(Vector(A4, A5), Vector(A4, B4)));
	glNormal3d(normTempVec.x, normTempVec.y, normTempVec.z);
	glVertex3f(A4.x, A4.y, A4.z);
	glVertex3f(B4.x, B4.y, B4.z);
	glVertex3f(A5.x, A5.y, A5.z);
	glVertex3f(B5.x, B5.y, B5.z);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	normTempVec = Normalize(Cross(Vector(A5, A6), Vector(A5, B5)));
	glNormal3d(normTempVec.x, normTempVec.y, normTempVec.z);
	glVertex3f(A5.x, A5.y, A5.z);
	glVertex3f(B5.x, B5.y, B5.z);
	glVertex3f(A6.x, A6.y, A6.z);
	glVertex3f(B6.x, B6.y, B6.z);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	normTempVec = Normalize(Cross(Vector(A6, A7), Vector(A6, B6)));
	glNormal3d(normTempVec.x, normTempVec.y, normTempVec.z);
	glVertex3f(A6.x, A6.y, A6.z);
	glVertex3f(B6.x, B6.y, B6.z);
	glVertex3f(A7.x, A7.y, A7.z);
	glVertex3f(B7.x, B7.y, B7.z);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	normTempVec = Normalize(Cross(Vector(A7, A8), Vector(A7, B7)));
	glNormal3d(normTempVec.x, normTempVec.y, normTempVec.z);
	glVertex3f(A7.x, A7.y, A7.z);
	glVertex3f(B7.x, B7.y, B7.z);
	glVertex3f(A8.x, A8.y, A8.z);
	glVertex3f(B8.x, B8.y, B8.z);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	normTempVec = Normalize(Cross(Vector(A8, A1), Vector(A8, B8)));
	glNormal3d(normTempVec.x, normTempVec.y, normTempVec.z);
	glVertex3f(A8.x, A8.y, A8.z);
	glVertex3f(B8.x, B8.y, B8.z);
	glVertex3f(A1.x, A1.y, A1.z);
	glVertex3f(B1.x, B1.y, B1.z);
	glEnd();
	//����� ��������� ������


   //��������� ������ ������

	
	glMatrixMode(GL_PROJECTION);	//������ �������� ������� ��������. 
	                                //(���� ��������� ��������, ����� �� ������������.)
	glPushMatrix();   //��������� ������� ������� ������������� (������� ��������� ������������� ��������) � ���� 				    
	glLoadIdentity();	  //��������� ��������� �������
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //������� ����� ������������� ��������

	glMatrixMode(GL_MODELVIEW);		//������������� �� �����-��� �������
	glPushMatrix();			  //��������� ������� ������� � ���� (��������� ������, ����������)
	glLoadIdentity();		  //���������� �� � ������

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //������� ����� ��������� ��� ������� ������ � �������� ������.
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;
	ss << "T - ���/���� �������" << std::endl;
	ss << "L - ���/���� ���������" << std::endl;
	ss << "F - ���� �� ������" << std::endl;
	ss << "G - ������� ���� �� �����������" << std::endl;
	ss << "G+��� ������� ���� �� ���������" << std::endl;
	ss << "�����. �����: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "�����. ������: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "��������� ������: R="  << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
	
	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //��������������� ������� �������� � �����-��� �������� �� �����.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}