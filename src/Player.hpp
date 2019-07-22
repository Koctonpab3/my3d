#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <GL/glu.h>
#include <math.h>
#include "helpFile.hpp"
using namespace sf;

float angleX, angleY; // Углы поворота камеры


class Player
{
public:
   float x,y,z;
   float dx,dy,dz;
   float w,h,d;  // width, height, depth
   bool onGround;
   float speed;

   Player(float x0, float y0, float z0)
   {
    x=x0; y=y0; z=z0;
	dx=0; dy=0; dz=0;
	w=5; h=20; d=5; speed=5;
	onGround=false;
   }

  void update(float time)
   {
     if (!onGround) dy-=1.5*time;
	 onGround=0;

	 x+=dx*time;
     collision(dx,0,0);
     y+=dy*time;
     collision(0,dy,0);
     z+=dz*time;
     collision(0,0,dz);

	 dx=dz=0;
   }

  void collision(float Dx,float Dy,float Dz)
  {
    for (int X=(x-w)/size;X<(x+w)/size;X++)
    for (int Y=(y-h)/size;Y<(y+h)/size;Y++)
    for (int Z=(z-d)/size;Z<(z+d)/size;Z++)
      if (check(X,Y,Z))  {
        if (Dx>0)  x = X*size-w;
        if (Dx<0)  x = X*size+size+w;
		if (Dy>0)  y = Y*size-h;
        if (Dy<0) {y = Y*size+size+h; onGround=true; dy=0;}
		if (Dz>0)  z = Z*size-d;
        if (Dz<0)  z = Z*size+size+d;
                          }
  }

  void keyboard()
  {
		if (Keyboard::isKeyPressed(Keyboard::Space)) if (onGround) {onGround=false; dy=12;};

		if (Keyboard::isKeyPressed(Keyboard::W))
		{
		 dx =- sin(angleX/180*PI) * speed;
		 dz =- cos(angleX/180*PI) * speed;
		}

		if (Keyboard::isKeyPressed(Keyboard::S))
		{
		 dx = sin(angleX/180*PI) * speed;
		 dz = cos(angleX/180*PI) * speed;
		}

		if (Keyboard::isKeyPressed(Keyboard::D))
		{
		 dx = sin((angleX+90)/180*PI) * speed;
		 dz = cos((angleX+90)/180*PI) * speed;
		}

		if (Keyboard::isKeyPressed(Keyboard::A))
		{
		 dx = sin((angleX-90)/180*PI) * speed;
		 dz = cos((angleX-90)/180*PI) * speed;
		}
  }

};
