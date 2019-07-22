#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <GL/glu.h>
#include <math.h>
#include "helpFile.hpp"
using namespace sf;


int main()
{
        bool sRgb = false;
	    sf::ContextSettings contextSettings;
        contextSettings.depthBits = 24;
        contextSettings.sRgbCapable = sRgb;

	RenderWindow window(VideoMode(800, 600), "Minecraft C++",sf::Style::Default, contextSettings);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClearDepth(1.f);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 2000.f);
    glEnable(GL_TEXTURE_2D);

	ShowCursor(FALSE);

	////карта высот////
	Image im;  im.loadFromFile("resources/heightmap.png");
	printf("load heightmap");

	for(int x=0;x<256;x++)
 	 for(int z=0;z<256;z++)
	 {
		 int c = im.getPixel(x,z).r / 15;
         for(int y=0;y<c;y++)
	       if (y>c-3) mass[x][y][z]=1;

	 }
     printf("load heightmap done");


    ///////текстуры///////
	Texture t; t.loadFromFile("resources/cursor.png");
	Sprite s(t); s.setOrigin(8,8); s.setPosition(400,300);


	  GLuint skybox[6];
	skybox[0] = LoadTexture("resources/skybox/skybox_front.bmp");
	skybox[1] = LoadTexture("resources/skybox/skybox_back.bmp");
	skybox[2] = LoadTexture("resources/skybox/skybox_left.bmp");
    skybox[3] = LoadTexture("resources/skybox/skybox_right.bmp");
	skybox[4] = LoadTexture("resources/skybox/skybox_bottom.bmp");
	skybox[5] = LoadTexture("resources/skybox/skybox_top.bmp");


      GLuint box[6];
	box[0] = LoadTexture("resources/grassBox/side.jpg");
	box[1] = LoadTexture("resources/grassBox/side.jpg");
	box[2] = LoadTexture("resources/grassBox/side.jpg");
	box[3] = LoadTexture("resources/grassBox/side.jpg");
	box[4] = LoadTexture("resources/grassBox/bottom.jpg");
	box[5] = LoadTexture("resources/grassBox/top.jpg");
	//////////////////////////



	Clock clock;
	bool mLeft=0,mRight=0; // mouse buttons
    Player p(100,200,100);

	//////основной цикл/////////
	while (window.isOpen())
    {
		float time = clock.getElapsedTime().asMilliseconds();
		clock.restart();
		time = time/50;
		if (time>3) time=3;


        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if ((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Escape))
                window.close();

			if (event.type == Event::MouseButtonPressed)
				{if ((int)event.key.code == (int)Mouse::Right) mRight=1;
				 if ((int)event.key.code == (int)Mouse::Left)  mLeft=1;}
        }


glClear(GL_DEPTH_BUFFER_BIT);

p.keyboard();
p.update(time);


////-----------------------
POINT mousexy;
GetCursorPos(&mousexy);
int xt=window.getPosition().x+400;
int yt=window.getPosition().y+300;

angleX +=(xt - mousexy.x)/4; //4 — чувствительность
angleY +=(yt - mousexy.y)/4;

if (angleY<-89.0){angleY=-89.0;}
if (angleY>89.0){angleY=89.0;}

SetCursorPos(xt,yt);
////----------------------



        if (mRight || mLeft)
          {
			float x = p.x;
			float y = p.y+p.h/2;
            float z = p.z;

			int X,Y,Z,oldX,oldY,oldZ;
			int dist=0;
		    while(dist<120)  // радиус действия
		    {
		      dist++;

			 x += -sin(angleX/180*PI);    X=x/size;
             y +=  tan(angleY/180*PI);    Y=y/size;
             z += -cos(angleX/180*PI);    Z=z/size;

		     if (check(X,Y,Z))
			   if (mLeft) { mass[X][Y][Z]=0; break;}
			   else   {mass[oldX][oldY][oldZ]=1; break;}

			 oldX=X; oldY=Y; oldZ=Z;
			}
	      }

mLeft=mRight=0;


glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt(p.x,p.y+p.h/2,p.z,p.x-sin(angleX/180*PI),p.y+p.h/2+(tan(angleY/180*PI)),p.z-cos(angleX/180*PI), 0, 1, 0);


//////////рисуем боксы///////
   int R = 30;

   int X=p.x/size;
   int Y=p.y/size;
   int Z=p.z/size;

    for(int x=X-R; x<X+R; x++)
 	 for(int y=0;y<25;y++)
 	  for(int z=Z-R;z<Z+R;z++)
	  {
		  if (!check(x,y,z)) continue;

		   glTranslatef(size*x+size/2, size*y+size/2, size*z+size/2);

		   createBox(box, size/2);

  		   glTranslatef(-size*x-size/2, -size*y-size/2, -size*z-size/2);
	  }



	   glTranslatef(p.x, p.y, p.z);
	   createBox(skybox,1000);
	   glTranslatef(-p.x, -p.y, -p.z);


		window.pushGLStates();
		window.draw(s);      //рисуем курсор
        window.popGLStates();

        window.display();
    }

    return 0;
}
