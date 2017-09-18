#include <windows.h>   // use as needed for your system
#include <gl/Gl.h>
#include <gl/glu.h>
#include "glut.h"
#include <cstdlib>  // for rand()
#include <iostream>
#include <set>
#include <ctime>
#include <string>
#include <vector>
double PI=3.14159265359;
double t=0;
double gr=.01;
GLdouble XMAX=480.0;//for coord width/height calculations
GLdouble sqsize=0;//GLdouble representing current side length of tiles
GLint depth=0;//int representing the depth of mirroring the 0 depth=>1 unit(a randomized matrix of tiles) 1 depth =>4 mirrored units 2 depth=>16 mirrored units n depth=>(2^n)^2 units
GLdouble delta;//used for the arc equations for the increments
GLdouble divi=1500.0;//the number of increments the arc equation should use gets smaller when tiles get smaller
GLdouble hsqsize=0;//half sqsize. used alot so made sense to make a variable for it insead of calculating it a bunch of times
GLdouble R=0;//used for arc equation for the radius of arc. made sense to make variable instead of calcualating each time since it is used alot
std::vector<std::vector<GLint>> gg;//matrix of ints that represents tiles it will be a 3 to 4 digit number the hundreds/thousands digit(s) represent the tile type, 
								//the tens digit is an int 0-3 that represents the rotation about the z axis of the tile(0=0 degrees,1=90 degrees,2=180 degrees,3=270 degrees)
								//the ones digit represents the flip about the y axis(0=0 degrees, 1=180 degrees)
//std::vector<std::vector<float>> toparc;//intsead of calculating the points for the arcs every time they are drawn I decided to make an array for them and only calculate them when the tiles change (pixel) size
//std::vector<std::vector<float>> leftarc;//and instead of calculating each side arc each time I decided to calculate the bottom arc and just rotate it
std::vector<std::vector<GLdouble>> botarc;
//std::vector<std::vector<float>> rightarc;
GLint refreshnum=100000000;//used for idle function to control animation rate
GLdouble sqs=1;//represents the number of tiles in a unit
GLint on=0;// controls when idle runs
GLint anind=0;//used to help animation
GLboolean *subs=new GLboolean[68];//used to help menu
GLboolean idlego=false;//used to troggle animation on/off
GLboolean sp=true;//used in an attemp to make sure the key hittings dont over lap calls.
std::set<GLint> tiles;//set containing the current list of tiles avaiable to the program to randomly select
//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
void myInit(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);      // set the bg color
	glMatrixMode(GL_MODELVIEW);           // set up appropriate matrices- to be explained 
	//glLoadIdentity();
	//gluOrtho2D(0, std::pow(2.0,depth)*XMAX,0, std::pow(2.0,depth)*XMAX);
}

//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
//void Arctop()
//{
//	float yo=sqsize;
//	float xo=0;
//	toparc[divi][0]=hsqsize;
//	toparc[divi][1]=hsqsize;
//	for(int z=(int)divi-1;z>=0;z--)
//	{
//		toparc[z][0]=((float)z)*delta-hsqsize;
//		toparc[z][1]=yo-sqrt(pow(R,2)-pow(((float)z)*delta-xo-hsqsize,2));
//	}
//}
void Arcbot()
{
	GLdouble yo=-sqsize;
	GLdouble xo=0;
	botarc[0][0]=-hsqsize;
	botarc[0][1]=-hsqsize;
	for(GLint z=1;z<=(GLint)divi;z++)
	{
		botarc[z][0]=((GLdouble)z)*delta-hsqsize;
		botarc[z][1]=sqrt(pow(R,2)-pow(((GLdouble)z)*delta-xo-hsqsize,2))+yo;
	}
}
//void Arcleft()
//{
//	float yo=0;
//	float xo=-sqsize;
//	leftarc[divi][0]=-hsqsize;
//	leftarc[divi][1]=hsqsize;
//	for(int z=(int)divi-1;z>=0;z--)
//	{
//		leftarc[z][0]=sqrt(pow(R,2)-pow(((float)z)*delta-yo-hsqsize,2))+xo;
//		leftarc[z][1]=((float)z)*delta-hsqsize;
//	}
//}
//void Arcright()
//{
//	float yo=0;
//	float xo=sqsize;
//	rightarc[0][0]=hsqsize;
//	rightarc[0][1]=-hsqsize;
//	for(int z=1;z<=(int)divi;z++)
//	{
//		rightarc[z][0]=xo-sqrt(pow(R,2)-pow(((float)z)*delta-yo-hsqsize,2));
//		rightarc[z][1]=((float)z)*delta-hsqsize;
//	}
//}
//void bArctop()
//{
//	glVertex2d(toparc[divi][0],toparc[divi][1]);
//	for(int z=(int)divi-1;z>=0;z--)
//	{
//		glVertex2d(toparc[z][0],toparc[z][1]);
//	}
//	/*float yo=ycorn+sqsize+sqsize/2.0;
//	float xo=xcorn+sqsize/2.0;
//	glVertex2d(xcorn+sqsize,ycorn+sqsize);
//	for(int z=(int)divi-1;z>=0;z--)
//	{
//		glVertex2d(xcorn+((float)z)*delta,yo-sqrt(pow(R,2)-pow(xcorn+((float)z)*delta-xo,2)));
//	}*/
//}
void bArcbot()
{
	glVertex2d(botarc[0][0],botarc[0][1]);
	for(GLint z=1;z<=(GLint)divi;z++)
	{
		glVertex2d(botarc[z][0],botarc[z][1]);
	}
}
//void bArcleft()
//{
//	float yo=0;
//	float xo=-sqsize;
//	glVertex2d(leftarc[divi][0],leftarc[divi][1]);
//	for(int z=(int)divi-1;z>=0;z--)
//	{
//		glVertex2d(leftarc[z][0],leftarc[z][1]);
//	}
//}
//void bArcright()
//{
//	glVertex2d(rightarc[0][0],rightarc[0][1]);
//	for(int z=1;z<=(int)divi;z++)
//	{
//		glVertex2d(rightarc[z][0],rightarc[z][1]);
//	}
//}



//A=arc,S=side line, D=Diagonal line, t=tangent,p=parallel, e=diagnal that make triangle with a "t",
//v=a diagonla that makes a -> in the corner of a t, a= when a diagnol makes a triangle with an arc tail,
//s=when a diagonal makes a triangle with a side as a tail.
//1
void drawBA1()
{
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
}
//2
void drawBA2t()
{
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();

	/*glBegin(GL_LINE_STRIP);
		bArcleft();
		bArcbot();
	glEnd()*/;
}
//3
void drawBA2p()
{
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	/*glBegin(GL_LINE_STRIP);
		bArcleft();
	glEnd();
	glBegin(GL_LINE_STRIP);
		bArcright();
	glEnd();*/
}
//4
void drawBA3()
{
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	bArcbot();
	//	bArcright();
	//glEnd();
} 
//5
void drawBA4()
{
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glRotated(270,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	bArcbot();
	//	bArcright();
	//	bArctop();
	//glEnd();
} 
//6
void drawBS1()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,-hsqsize);
	glEnd();
}
//7
void drawBS2t()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,-hsqsize);
	glEnd();
} 
//8
void drawBS2p()
{
	glBegin(GL_LINES);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
	glEnd();
} 
//9
void drawBS3()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
	glEnd();
} 
//10
void drawBS4()
{
	glBegin(GL_LINE_LOOP);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
	glEnd();
} 
//11
void drawBD1()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
	glEnd();
}
//12
void drawBD2()
{
	glBegin(GL_LINES);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
}
//13
void drawBS1D1()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
	glEnd();
}
//14
void drawBS1D2()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
}
//15
void drawBS2pD1()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
}
//16
void drawBS2tD1v()
{
	glBegin(GL_LINES);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
	glEnd();
}
//17
void drawBS2tD1e()
{
	glBegin(GL_LINE_LOOP);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,-hsqsize);
	glEnd();
}
//18
void drawBS2pD2()
{
	glBegin(GL_LINE_LOOP);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(hsqsize,-hsqsize);
	glEnd();
}
//19
void drawBS2tD2()
{
	glBegin(GL_LINE_LOOP);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,-hsqsize);
	glEnd();
	glBegin(GL_LINES);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
	glEnd();
}
//20
void drawBS3D1()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
	glEnd();
}
//21
void drawBS3D2()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
	glEnd();
}
//22
void drawBS4D1()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
	glEnd();
}
//23
void drawBS4D2()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
	glEnd();
	glBegin(GL_LINES);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
}
//24
void drawBA1S1t()
{
	glBegin(GL_LINE_STRIP);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
	glEnd();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	glVertex2f(hsqsize,-hsqsize);
	//glEnd();
}
//25
void drawBA1S1tD1v( )
{
	glBegin(GL_LINE_STRIP);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
	glEnd();
	glBegin(GL_LINES);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	glVertex2f(hsqsize,-hsqsize);
	//glEnd();
	//glBegin(GL_LINES);
	//	glVertex2f(-hsqsize,-hsqsize);
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
}
//26
void drawBA1S1tD1e( )
{
	glBegin(GL_LINE_LOOP);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
	glEnd();
	//glBegin(GL_LINE_LOOP);
	//	bArcleft();
	//	glVertex2f(hsqsize,-hsqsize);
	//glEnd();
}
//27
void drawBA1S1tD2( )
{	
	glBegin(GL_LINE_LOOP);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
	glEnd();
	glBegin(GL_LINES);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
	//glBegin(GL_LINE_LOOP);
	//	bArcleft();
	//	glVertex2f(hsqsize,-hsqsize);
	//glEnd();
	//glBegin(GL_LINES);
	//	glVertex2f(-hsqsize,-hsqsize);
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
}
//28
void drawBA1S1p( )
{
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glBegin(GL_LINES);
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//glEnd();
	//glBegin(GL_LINES);
	//	glVertex2f(hsqsize,hsqsize);
	//	glVertex2f(hsqsize,-hsqsize);
	//glEnd();
}
//29
void drawBA1S1pD1( )
{	
	glBegin(GL_LINE_STRIP);
		bArcbot();
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(hsqsize,hsqsize);
	glEnd();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	glVertex2f(hsqsize,hsqsize);
	//	glVertex2f(hsqsize,-hsqsize);
	//glEnd();
}
//30
void drawBA1S1pD2( )
{
	glBegin(GL_LINE_LOOP);
		bArcbot();
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(hsqsize,hsqsize);
	glEnd();
	//glBegin(GL_LINE_LOOP);
	//	bArcleft();
	//	glVertex2f(hsqsize,hsqsize);
	//	glVertex2f(hsqsize,-hsqsize);
	//glEnd();
}
//31
void drawBA1D1( )
{
	glBegin(GL_LINE_STRIP);
		bArcbot();
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
}
//32
void drawBA1D2( )
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,hsqsize);
		bArcbot();
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
}
//33
void drawBA1S2t( )
{	
	glBegin(GL_LINE_STRIP);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	glVertex2f(hsqsize,-hsqsize);
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
}
//34
void drawBA1S2tD1a( )
{	
	glBegin(GL_LINE_STRIP);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(hsqsize,-hsqsize);
	glEnd();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	glVertex2f(hsqsize,-hsqsize);
	//	glVertex2f(hsqsize,hsqsize);
	//	glVertex2f(-hsqsize,-hsqsize);
	//glEnd();
}
//35
void drawBA1S2tD1s( )
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,hsqsize);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
	//glBegin(GL_LINE_STRIP);
	//	glVertex2f(hsqsize,-hsqsize);
	//	bArcleft();
	//	glVertex2f(hsqsize,-hsqsize);
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
}
//36
void drawBA1S2tD2( )
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,hsqsize);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(hsqsize,-hsqsize);
	glEnd();
	//glBegin(GL_LINE_STRIP);
	//	glVertex2f(hsqsize,-hsqsize);
	//	bArcleft();
	//	glVertex2f(hsqsize,-hsqsize);
	//	glVertex2f(hsqsize,hsqsize);
	//	glVertex2f(-hsqsize,-hsqsize);
	//glEnd();
}
//37
void drawBA1S2p( )
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(-hsqsize,hsqsize);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
	glEnd();
}
//38
void drawBA1S2pD1( )
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(-hsqsize,hsqsize);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
	glEnd();
}
//39
void drawBA1S2pD2( )
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(-hsqsize,hsqsize);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
	glEnd();
}
//40
void drawBA1S3( )
{
	glBegin(GL_LINE_LOOP);
		glVertex2d(-hsqsize,hsqsize);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
	glEnd();
}
//41
void drawBA1S3D1( )
{
	glBegin(GL_LINE_LOOP);
		glVertex2d(-hsqsize,hsqsize);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
	glEnd();
	glBegin(GL_LINES);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
	glEnd();
}
//42
void drawBA1S3D2( )
{
	glBegin(GL_LINE_LOOP);
		glVertex2d(-hsqsize,hsqsize);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
	glEnd();
	glBegin(GL_LINES);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(hsqsize,-hsqsize);
	glEnd();
}
//43
void drawBA2tD1v()
{
	glBegin(GL_LINE_STRIP);
		bArcbot();
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	bArcbot();
	//glEnd();
	//glBegin(GL_LINES);
	//	glVertex2f(-hsqsize,-hsqsize);
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
}
//44
void drawBA2tD1e()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,hsqsize);
		bArcbot();
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_LOOP);
	//	bArcleft();
	//	bArcbot();
	//glEnd();
}
//45
void drawBA2tD2()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,hsqsize);
		bArcbot();
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_LOOP);
	//	bArcleft();
	//	bArcbot();
	//glEnd();
	//glBegin(GL_LINES);
	//	glVertex2f(-hsqsize,-hsqsize);
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
}
//46
void drawBA2tS1()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(-hsqsize,hsqsize);
		bArcbot();
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	bArcbot();
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
}
//47
void drawBA2tS1D1a()
{
	glBegin(GL_LINE_LOOP);
		bArcbot();
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	bArcbot();
	//	glVertex2f(hsqsize,hsqsize);
	//	glVertex2f(-hsqsize,-hsqsize);
	//glEnd();
}
//48
void drawBA2tS1D1s()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(hsqsize,hsqsize);
		bArcbot();
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_STRIP);
	//	glVertex2f(hsqsize,-hsqsize);
	//	bArcleft();
	//	bArcbot();
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
}
//49
void drawBA2tS1D2()
{
	glBegin(GL_LINE_LOOP);
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(hsqsize,hsqsize);
		bArcbot();
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_STRIP);
	//	glVertex2f(hsqsize,-hsqsize);
	//	bArcleft();
	//	bArcbot();
	//	glVertex2f(hsqsize,hsqsize);
	//	glVertex2f(-hsqsize,-hsqsize);
	//glEnd();
}
//50
void drawBA2tS2()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,hsqsize);
		bArcbot();
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_LOOP);
	//	bArcleft();
	//	bArcbot();
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
}
//51
void drawBA2tS2D1v()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,hsqsize);
		bArcbot();
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_LOOP);
	//	bArcleft();
	//	bArcbot();
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
	//glBegin(GL_LINES);
	//	glVertex2f(-hsqsize,-hsqsize);
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
}
//52
void drawBA2tS2D1e()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,hsqsize);
		bArcbot();
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_LOOP);
	//	bArcleft();
	//	bArcbot();
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
	//glBegin(GL_LINES);
	//	glVertex2f(hsqsize,-hsqsize);
	//	glVertex2f(-hsqsize,hsqsize);
	//glEnd();
}
//53
void drawBA2tS2D2()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,hsqsize);
		bArcbot();
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_LOOP);
	//	bArcleft();
	//	bArcbot();
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
	//glBegin(GL_LINES);
	//	glVertex2f(hsqsize,-hsqsize);
	//	glVertex2f(-hsqsize,hsqsize);
	//	glVertex2f(-hsqsize,-hsqsize);
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
}
//54
void drawBA2pD1()
{
	glBegin(GL_LINE_STRIP);
		bArcbot();
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
	//glBegin(GL_LINE_STRIP);
	//	bArcright();
	//glEnd();
}
//55
void drawBA2pD2()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,hsqsize);
		bArcbot();
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
	//glBegin(GL_LINE_STRIP);
	//	glVertex2f(-hsqsize,hsqsize);
	//	bArcright();
	//glEnd();
}
//56
void drawBA2pS1()
{
	glBegin(GL_LINE_STRIP);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
	glEnd();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	bArcright();
	//glEnd();
}
//57
void drawBA2pS1D1()
{
	glBegin(GL_LINE_LOOP);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
	glEnd();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	bArcright();
	//	glVertex2f(-hsqsize,-hsqsize);
	//glEnd();
}
//58
void drawBA2pS1D2()
{
	glBegin(GL_LINE_LOOP);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
	glEnd();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_STRIP);
	//	glVertex2f(hsqsize,-hsqsize);
	//	bArcleft();
	//	bArcright();
	//	glVertex2f(-hsqsize,-hsqsize);
	//glEnd();
}
//59
void drawBA2pS2()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(-hsqsize,hsqsize);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
	glEnd();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_LOOP);
	//	bArcleft();
	//	bArcright();
	//glEnd();
}
//60
void drawBA2pS2D1()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(-hsqsize,hsqsize);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
	glEnd();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_LOOP);
	//	bArcleft();
	//	bArcright();
	//glEnd();
	//glBegin(GL_LINES);
	//	glVertex2f(hsqsize,-hsqsize);
	//	glVertex2f(-hsqsize,hsqsize);
	//glEnd();
}
//61
void drawBA2pS2D2()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,-hsqsize);
		glVertex2d(-hsqsize,hsqsize);
		bArcbot();
		glVertex2d(hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
	glEnd();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_LOOP);
	//	bArcleft();
	//	bArcright();
	//glEnd();
	//glBegin(GL_LINES);
	//	glVertex2f(hsqsize,-hsqsize);
	//	glVertex2f(-hsqsize,hsqsize);
	//	glVertex2f(-hsqsize,-hsqsize);
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
}
//62
void drawBA3D1()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,hsqsize);
		bArcbot();
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	bArcbot();
	//	bArcright();
	//	glVertex2f(-hsqsize,-hsqsize);
	//glEnd();
}
//63
void drawBA3D2()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,hsqsize);
		bArcbot();
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_STRIP);
	//	glVertex2f(hsqsize,-hsqsize);
	//	bArcleft();
	//	bArcbot();
	//	bArcright();
	//	glVertex2f(-hsqsize,-hsqsize);
	//glEnd();
}
//64
void drawBA3S1()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(-hsqsize,hsqsize);
		bArcbot();
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_LOOP);
	//	bArcleft();
	//	bArcbot();
	//	bArcright();
	//glEnd();
}
//65
void drawBA3S1D1()
{
	glBegin(GL_LINE_LOOP);
		glVertex2d(-hsqsize,hsqsize);
		bArcbot();
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_LOOP);
	//	bArcleft();
	//	bArcbot();
	//	bArcright();
	//glEnd();
	//glBegin(GL_LINES);
	//	glVertex2f(-hsqsize,-hsqsize);
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
}
//66
void drawBA3S1D2()
{
	glBegin(GL_LINE_STRIP);
		bArcbot();
		glVertex2d(-hsqsize,hsqsize);
		glVertex2d(-hsqsize,-hsqsize);
		glVertex2d(hsqsize,hsqsize);
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_LOOP);
	//	bArcleft();
	//	bArcbot();
	//	bArcright();
	//glEnd();
	//glBegin(GL_LINES);
	//	glVertex2f(hsqsize,-hsqsize);
	//	glVertex2f(-hsqsize,hsqsize);
	//	glVertex2f(-hsqsize,-hsqsize);
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
}
//67
void drawBA4D1()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,hsqsize);
		bArcbot();
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glRotated(270,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	bArcbot();
	//	bArcright();
	//	bArctop();
	//	glVertex2f(hsqsize,-hsqsize);
	//glEnd();
}
//68
void drawBA4D2()
{
	glBegin(GL_LINE_STRIP);
		glVertex2d(hsqsize,hsqsize);
		bArcbot();
		glVertex2d(-hsqsize,hsqsize);
	glEnd();
	glPushMatrix();
	glRotated(90,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glRotated(180,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glRotated(270,0,0,1);
	glBegin(GL_LINE_STRIP);
		bArcbot();
	glEnd();
	glPopMatrix();
	//glBegin(GL_LINE_STRIP);
	//	bArcleft();
	//	bArcbot();
	//	bArcright();
	//	bArctop();
	//	glVertex2f(hsqsize,-hsqsize);
	//glEnd();
	//glBegin(GL_LINES);
	//	glVertex2f(-hsqsize,-hsqsize);
	//	glVertex2f(hsqsize,hsqsize);
	//glEnd();
}
void drawselect(GLint x)//switch structure for selecting which tile to draw
{
	switch(x)
	{
		case 1:
			drawBA1();
			return;
		case 2:
			drawBA2t();
			return;
		case 3:
			drawBA2p();
			return;
		case 4:
			drawBA3();
			return;
		case 5:
			drawBA4();
			return;
		case 6:
			drawBS1();
			return;
		case 7:
			drawBS2t();
			return;
		case 8:
			drawBS2p();
			return;
		case 9:
			drawBS3();
			return;
		case 10:
			drawBS4();
			return;
		case 11:
			drawBD1();
			return;
		case 12:
			drawBD2();
			return;
		case 13:
			drawBS1D1();
			return;
		case 14:
			drawBS1D2();
			return;
		case 15:
			drawBS2pD1();
			return;
		case 16:
			drawBS2tD1v();
			return;
		case 17:
			drawBS2tD1e();
			return;
		case 18:
			drawBS2pD2();
			return;
		case 19:
			drawBS2tD2();
			return;
		case 20:
			drawBS3D1();
			return;
		case 21:
			drawBS3D2();
			return;
		case 22:
			drawBS4D1();
			return;
		case 23:
			drawBS4D2();
			return;
		case 24:
			drawBA1S1t();
			return;
		case 25:
			drawBA1S1tD1v();
			return;
		case 26:
			drawBA1S1tD1e();
			return;
		case 27:
			drawBA1S1tD2();
			return;
		case 28:
			drawBA1S1p();
			return;
		case 29:
			drawBA1S1pD1();
			return;
		case 30:
			drawBA1S1pD2();
			return;
		case 31:
			drawBA1D1();
			return;
		case 32:
			drawBA1D2();
			return;
		case 33:
			drawBA1S2t();
			return;
		case 34:
			drawBA1S2tD1a();
			return;
		case 35:
			drawBA1S2tD1s();
			return;
		case 36:
			drawBA1S2tD2();
			return;
		case 37:
			drawBA1S2p();
			return;
		case 38:
			drawBA1S2pD1();
			return;
		case 39:
			drawBA1S2pD2();
			return;
		case 40:
			drawBA1S3();
			return;
		case 41:
			drawBA1S3D1();
			return;
		case 42:
			drawBA1S3D2();
			return;
		case 43:
			drawBA2tD1v();
			return;
		case 44:
			drawBA2tD1e();
			return;
		case 45:
			drawBA2tD2();
			return;
		case 46:
			drawBA2tS1();
			return;
		case 47:
			drawBA2tS1D1a();
			return;
		case 48:
			drawBA2tS1D1s();
			return;
		case 49:
			drawBA2tS1D2();
			return;
		case 50:
			drawBA2tS2();
			return;
		case 51:
			drawBA2tS2D1v();
			return;
		case 52:
			drawBA2tS2D1e();
			return;
		case 53:
			drawBA2tS2D2();
			return;
		case 54:
			drawBA2pD1();
			return;
		case 55:
			drawBA2pD2();
			return;
		case 56:
			drawBA2pS1();
			return;
		case 57:
			drawBA2pS1D1();
			return;
		case 58:
			drawBA2pS1D2();
			return;
		case 59:
			drawBA2pS2();
			return;
		case 60:
			drawBA2pS2D1();
			return;
		case 61:
			drawBA2pS2D2();
			return;
		case 62:
			drawBA3D1();
			return;
		case 63:
			drawBA3D2();
			return;
		case 64:
			drawBA3S1();
			return;
		case 65:
			drawBA3S1D1();
			return;
		case 66:
			drawBA3S1D2();
			return;
		case 67:
			drawBA4D1();
			return;
		case 68:
			drawBA4D2();
			return;
	}
}
void transform(GLint rot,GLint flip,GLint select,GLdouble x, GLdouble y)//does the opropriate translateion,rotation and flip and calls drawselect to draw the tile.
{
			GLdouble xx=x+hsqsize;
			GLdouble yy=y+hsqsize;
			glTranslated(xx,yy,0);
			if(flip!=0)
				glRotated(180.0*flip,0,1,0);
			if(rot!=0)
				glRotated(90.0*rot,0,0,1);
			drawselect(select);
}
void tilematrixexpandndraw()//only used initially and when the "remake" is selected in menu. create a randomized unit and draws the corresponding scene.
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glColor3f(1,1,1);
	glLoadIdentity();
	GLint sn=std::pow(2.0,depth);
	GLint del=sn*sqs;//used to resize matrix to the full floor matrix that is composed of the unit and its mirrors
	gluOrtho2D(0, XMAX*sn,0, XMAX*sn);
	if(del!=gg.size())//if a new size resize
	{
		gg.resize(del,std::vector<GLint>(del,0));
		for(GLint z=0;z<gg.size()&&gg[z].size()<del;z++)
			gg[z].resize(del,0);
		for(GLint z=gg.size()-1;z>-1&&gg[z].size()>del;z--)
			gg[z].resize(del,0);
	}
	GLint temp=0;//used in 'decoding' the tile matrix ints
	GLint select=0;//int rep tile 
	GLint rot=0;//int rep rot
	GLint flip=0;//int rep flip
	GLdouble x2XMAX=2*XMAX;//since this is a frequent calculation added this to lower the number of operations in the quadrupel loop
	GLint tem=std::pow(2.0,depth-1);// used for the inner loops bounds it represents the number of units on a side of a floor
	GLdouble xx0=0.00;//x coord of tile in original unit
	GLdouble yy0=0.00;//y coord of tile in original unit
	GLdouble xx1=0.00;//x coord of tile in first diagonal mirrored unit
	GLdouble yy1=0.00;//y coord of tile in first diagonal mirrored unit
	GLint lala=0;//used for the index of populating gg[] with the floor
	GLint rara=0;//used for the index of populating gg with the floor
	if(depth!=0)//checks to see if depth isnt zero
		for(GLint y=0;y<sqs;y++)//outer 2 loops traverse the original unit gg[y][x] is the current tile in the original unit which 
		{					//the inner loops will mirror outward into each of the other units 
			for(GLint x=0;x<sqs;x++)
			{
				temp=gg[y][x];//grabs the int representing the tile in the original unit and decodes it. then calculates the initial tile locations
				flip=temp%2;
				rot=(temp/10)%10%4;
				select=(temp/100);
				xx0=((GLdouble)x)*sqsize;
				yy0=((GLdouble)y)*sqsize;
				xx1=x2XMAX-(xx0+sqsize);
				yy1=x2XMAX-(yy0+sqsize);
				for(GLint m=0; m<tem;m++)//mirrors and draws out the tile into each unit
				{
					rara=2*m*sqs+y;
					for(GLint n=0;n<tem;n++)
					{
						glPushMatrix();
						lala=2*n*sqs+x;
						gg[rara][lala]=select*100+rot*10+flip;
						transform(rot,flip,select,(xx0+n*x2XMAX),(yy0+m*x2XMAX));
						glPopMatrix();
						flip=(flip+1)%2;
						glPushMatrix();
						lala=2*(n+1)*sqs-1-x;
						gg[rara][lala]=select*100+rot*10+flip;
						transform(rot,flip,select,xx1+n*x2XMAX,yy0+m*x2XMAX);
						glPopMatrix();
						flip=(flip+1)%2;
					}
					rot=(rot+2)%4;
					flip=(flip+1)%2;
					rara=2*(m+1)*sqs-1-y;
					for(GLint n=0;n<tem;n++)
					{
						glPushMatrix();
						lala=2*n*sqs+x;
						gg[rara][lala]=select*100+rot*10+flip;
						transform(rot,flip,select,xx0+n*x2XMAX,yy1+m*x2XMAX);
						glPopMatrix();
						flip=(flip+1)%2;
						glPushMatrix();
						lala=2*(n+1)*sqs-1-x;
						gg[rara][lala]=select*100+rot*10+flip;
						transform(rot,flip,select,xx1+n*x2XMAX,yy1+m*x2XMAX);
						glPopMatrix();
						flip=(flip+1)%2;
					}
					rot=(rot+2)%4;
					flip=(flip+1)%2;
				}
			}
		}
	else//if depth is 0 just draws the original unit
		for(GLint i=0;i<sqs;i++)
			for(GLint ii=0;ii<sqs;ii++)
			{
				select=(gg[i][ii]/100);
				rot=((gg[i][ii]/10)%10)%4;
				flip=gg[i][ii]%2;
				glPushMatrix();
				transform(rot,flip,select,((GLdouble)ii)*sqsize,((GLdouble)i)*sqsize);
				glPopMatrix();
			}
}
void myDisplay(void)//disp func
{
		//idlego=false;
		tilematrixexpandndraw();
		glutSwapBuffers();             // send all output to display 
		//idlego=true;
}
void floorincrease()//used when the number of tiles in a unit is increased. similar to tilematrixexpandndraw except it is
 {					//optimized with the consideration that it will only be called when sqs is increased. it also populates the added row and column without changinging the original unit's tiles
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glColor3f(1,1,1);
	glLoadIdentity();
	std::set<GLint>::iterator ttt;
	GLint sn=std::pow(2.0,depth);
	GLint del=sn*sqs;
	GLint prevsize=sn*(sqs-1);
	gluOrtho2D(0, XMAX*sn,0, XMAX*sn);
	if(del>gg.size())
	{
		gg.resize(del,std::vector<GLint>(del,0));
		for(GLint z=0;gg[z].size()<del;z++)
			gg[z].resize(del,0);
	}
	if(tiles.size()==0)
		return;
	GLint select=(rand())%tiles.size();
	ttt=tiles.begin();
	for(;select>0;++ttt,select--);
	gg[sqs-1][sqs-1]=(*ttt)*100+((rand())%4)*10+(rand())%2;
	for(GLint y=0;y<sqs-1;y++)
	{
		select=(rand())%tiles.size();
		ttt=tiles.begin();
		for(;select>0;++ttt,select--);
		gg[y][sqs-1]=(*ttt)*100+((rand())%4)*10+(rand())%2;
		select=(rand())%tiles.size();
		ttt=tiles.begin();
		for(;select>0;++ttt,select--);
		gg[sqs-1][y]=(*ttt)*100+((rand())%4)*10+(rand())%2;
	}
	GLint temp=0;
	GLint rot=0;
	GLint flip=0;
	GLdouble x2XMAX=2*XMAX;
	GLint tem=std::pow(2.0,depth-1);
	GLdouble xx0=0.00;
	GLdouble yy0=0.00;
	GLdouble xx1=0.00;
	GLdouble yy1=0.00;
	GLint lala=0;
	GLint rara=0;
	if(depth!=0)
		for(GLint y=0;y<sqs;y++)
		{
			for(GLint x=0;x<sqs;x++)
			{
				temp=gg[y][x];
				flip=temp%2;
				rot=(temp/10)%10%4;
				select=(temp/100);
				xx0=((GLdouble)x)*sqsize;
				yy0=((GLdouble)y)*sqsize;
				xx1=x2XMAX-(xx0+sqsize);
				yy1=x2XMAX-(yy0+sqsize);
				for(GLint m=0; m<tem;m++)
				{
					rara=2*m*sqs+y;
					for(GLint n=0;n<tem;n++)
					{
						glPushMatrix();
						lala=2*n*sqs+x;
						gg[rara][lala]=select*100+rot*10+flip;
						transform(rot,flip,select,(xx0+n*x2XMAX),(yy0+m*x2XMAX));
						glPopMatrix();
						flip=(flip+1)%2;
						glPushMatrix();
						lala=2*(n+1)*sqs-1-x;
						gg[rara][lala]=select*100+rot*10+flip;
						transform(rot,flip,select,xx1+n*x2XMAX,yy0+m*x2XMAX);
						glPopMatrix();
						flip=(flip+1)%2;
					}
					rot=(rot+2)%4;
					flip=(flip+1)%2;
					rara=2*(m+1)*sqs-1-y;
					for(GLint n=0;n<tem;n++)
					{
						glPushMatrix();
						lala=2*n*sqs+x;
						gg[rara][lala]=select*100+rot*10+flip;
						transform(rot,flip,select,xx0+n*x2XMAX,yy1+m*x2XMAX);
						glPopMatrix();
						flip=(flip+1)%2;
						glPushMatrix();
						lala=2*(n+1)*sqs-1-x;
						gg[rara][lala]=select*100+rot*10+flip;
						transform(rot,flip,select,xx1+n*x2XMAX,yy1+m*x2XMAX);
						glPopMatrix();
						flip=(flip+1)%2;
					}
					rot=(rot+2)%4;
					flip=(flip+1)%2;
				}
			}
		}
	else
		for(GLint i=0;i<sqs;i++)
			for(GLint ii=0;ii<sqs;ii++)
			{
				select=(gg[i][ii]/100);
				rot=((gg[i][ii]/10)%10)%4;
				flip=gg[i][ii]%2;
				glPushMatrix();
				transform(rot,flip,select,((GLdouble)ii)*sqsize,((GLdouble)i)*sqsize);
				glPopMatrix();
			}
	sp=true;
 }
void floordecrease()//used when the number of tiles in a unit is decreased. similar to tilematrixexpandndraw except it is
 {					//optimized with the consideration that it will  only be called when sqs is decreased. it ignores the row and column that is removed without changinging the other tiles in the original unit
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);    //it also doesn't bother to resize the vector
	glColor3f(1,1,1);
	glLoadIdentity();
	GLint sn=std::pow(2.0,depth);
	//int del=sn*sqs;
	gluOrtho2D(0, XMAX*sn,0, XMAX*sn);
	GLint select=0;
	GLint temp=0;
	GLint rot=0;
	GLint flip=0;
	GLdouble x2XMAX=2*XMAX;
	GLint tem=std::pow(2.0,depth-1);
	GLdouble xx0=0.00;
	GLdouble yy0=0.00;
	GLdouble xx1=0.00;
	GLdouble yy1=0.00;
	GLint lala=0;
	GLint rara=0;
	if(depth!=0)
		for(GLint y=0;y<sqs;y++)
		{
			for(GLint x=0;x<sqs;x++)
			{
				temp=gg[y][x];
				flip=temp%2;
				rot=(temp/10)%10%4;
				select=(temp/100);
				xx0=((GLdouble)x)*sqsize;
				yy0=((GLdouble)y)*sqsize;
				xx1=x2XMAX-(xx0+sqsize);
				yy1=x2XMAX-(yy0+sqsize);
				for(GLint m=0; m<tem;m++)
				{
					rara=2*m*sqs+y;
					for(GLint n=0;n<tem;n++)
					{
						glPushMatrix();
						lala=2*n*sqs+x;
						gg[rara][lala]=select*100+rot*10+flip;
						transform(rot,flip,select,(xx0+n*x2XMAX),(yy0+m*x2XMAX));
						glPopMatrix();
						flip=(flip+1)%2;
						glPushMatrix();
						lala=2*(n+1)*sqs-1-x;
						gg[rara][lala]=select*100+rot*10+flip;
						transform(rot,flip,select,xx1+n*x2XMAX,yy0+m*x2XMAX);
						glPopMatrix();
						flip=(flip+1)%2;
					}
					rot=(rot+2)%4;
					flip=(flip+1)%2;
					rara=2*(m+1)*sqs-1-y;
					for(GLint n=0;n<tem;n++)
					{
						glPushMatrix();
						lala=2*n*sqs+x;
						gg[rara][lala]=select*100+rot*10+flip;
						transform(rot,flip,select,xx0+n*x2XMAX,yy1+m*x2XMAX);
						glPopMatrix();
						flip=(flip+1)%2;
						glPushMatrix();
						lala=2*(n+1)*sqs-1-x;
						gg[rara][lala]=select*100+rot*10+flip;
						transform(rot,flip,select,xx1+n*x2XMAX,yy1+m*x2XMAX);
						glPopMatrix();
						flip=(flip+1)%2;
					}
					rot=(rot+2)%4;
					flip=(flip+1)%2;
				}
			}
		}
	else
		for(GLint i=0;i<sqs;i++)
			for(GLint ii=0;ii<sqs;ii++)
			{
				select=(gg[i][ii]/100);
				rot=((gg[i][ii]/10)%10)%4;
				flip=gg[i][ii]%2;
				glPushMatrix();
				transform(rot,flip,select,((GLdouble)ii)*sqsize,((GLdouble)i)*sqsize);
				glPopMatrix();
			}
}
void createnewfloor()//creates a brand new randomized floor
{
	//idlego=false;
	//gg.empty();
	//gg.resize(sqs,std::vector<int>(sqs,0));
	//for(int z=0;z<sqs;z++)
		//gg[z].resize(sqs,0);
	std::set<GLint>::iterator ttt;
	GLint select=0;
	for(GLint z=0;z<sqs;z++)
		for(GLint zz=0;zz<sqs;zz++)
		{
			select=(rand())%tiles.size();
			ttt=tiles.begin();
			for(;select>0;++ttt,select--);
			gg[z][zz]=(*ttt)*100+((rand())%4)*10+(rand())%2;
		}
	tilematrixexpandndraw();
	glutSwapBuffers();
	//idlego=true;
}
void heart()
{
	if(idlego)
	{
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glColor3f(1,0,0);
		glLoadIdentity();
		gluOrtho2D(-20,20,-20,20);
		//double R=0.5*.999;
		double theta=.01;
		double x=0;
		double y=13-5-2-1;
		glBegin(GL_POLYGON);
			glVertex2d(x,y);
			for(;theta<=2*PI;theta+=.01)
			{
				x=(12.0)*pow(sin(theta),3);
				x+=x*t;
				y=(13)*cos(theta)-5*cos(2*theta)-2*cos(3*theta)-cos(4*theta);
				y+=y*t;
				glVertex2d(x,y);
				//R=R*.999;
			}
		glEnd();
		glutSwapBuffers();
		t+=gr;
		if(t>=1.00)
			gr=-0.01;
		if(t<=0)
			gr=0.01;
	}
}
void depthincreased()//used when the number of mirror 'fold'  is increased. similar to tilematrixexpandndraw except it is
 {					//optimized with the consideration that it will only be called when depth is increased by 1. it uses current floor and increases the vectors if needed
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);   //it also only needs a double loop since it just mirrors the current floor with a single horr/vert cross 'fold'
	glColor3f(1,1,1);
	glLoadIdentity();
	GLint sn=std::pow(2.0,depth);
	GLint del=sn*sqs;
	GLint prevsize=del/2;
	gluOrtho2D(0, XMAX*sn,0, XMAX*sn);
	if(del>gg.size())
	{
		gg.resize(del,std::vector<GLint>(del,0));
		for(GLint z=0;gg[z].size()<del;z++)
			gg[z].resize(del,0);
	}
	GLint temp=0;
	GLint select=0;
	GLint rot=0;
	GLint flip=0;
	GLdouble xx0=0.00;
	GLdouble yy0=0.00;
	GLdouble xx1=0.00;
	GLdouble yy1=0.00;
	GLint xt=0,yt=0;
	for(GLint y=0;y<prevsize;y++)
	{
		for(GLint x=0;x<prevsize;x++)
		{
			temp=gg[y][x];
			flip=temp%2;
			rot=(temp/10)%10%4;
			select=(temp/100);
			xx0=((GLdouble)x)*sqsize;
			yy0=((GLdouble)y)*sqsize;
			xt=del-1-x;
			yt=del-1-y;
			xx1=((GLdouble)xt)*sqsize;
			yy1=((GLdouble)yt)*sqsize;
			glPushMatrix();
			transform(rot,flip,select,(xx0),(yy0));
			glPopMatrix();
			gg[y][xt]=select*100+rot*10+(flip+1)%2;
			glPushMatrix();
			transform(rot,(flip+1)%2,select,(xx1),(yy0));
			glPopMatrix();
			gg[yt][x]=select*100+(rot+2)%4*10+(flip+1)%2;
			glPushMatrix();
			transform((rot+2)%4,(flip+1)%2,select,(xx0),(yy1));
			glPopMatrix();
			gg[yt][xt]=select*100+(rot+2)%4*10+flip;
			glPushMatrix();
			transform((rot+2)%4,flip,select,(xx1),(yy1));
			glPopMatrix();
		}
	}
}
void depthdecrease()//used when the number of mirror 'fold'  is decreased. similar to tilematrixexpandndraw except it is
 {					//optimized with the consideration that it will only be called when depth is decreased by 1. It uses current units.
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);   //it also only needs a double loop since it just 'unfolds' once meaning it will just travers the matrix lower left 1/4th corner of the floor to make new floor
	glColor3f(1,1,1);
	glLoadIdentity();
	GLint sn=std::pow(2.0,depth);
	GLint del=sn*sqs;
	//int prevsize=del/2;
	gluOrtho2D(0, XMAX*sn,0, XMAX*sn);
	/*gg.resize(del,std::vector<int>(del,0));
	for(int z=0;z<prevsize;z++)
		gg[z].resize(del,0);*/
	GLint temp=0;
	GLint select=0;
	GLint rot=0;
	GLint flip=0;
	GLdouble xx0=0.00;
	GLdouble yy0=0.00;
	//float xx1=0.00;
	//float yy1=0.00;
	//int xt=0,yt=0;
	for(GLint y=0;y<del;y++)
	{
		for(GLint x=0;x<del;x++)
		{
			temp=gg[y][x];
			flip=temp%2;
			rot=(temp/10)%10%4;
			select=(temp/100);
			xx0=((GLdouble)x)*sqsize;
			yy0=((GLdouble)y)*sqsize;
			glPushMatrix();
			transform(rot,flip,select,(xx0),(yy0));
			glPopMatrix();
		}
	}
}

void tileselect(GLint option) //control for right click menu which adds and removes tiles from the set which randomization selects from
{
	std::string sas;
	std::string as="";
	if(option<100)
	{
		subs[option-1]=!subs[option-1];
		if(subs[option-1])
		{
			tiles.insert(option);
			as="*";
		}
		else
			tiles.erase(option);
	}
	switch (option) 
	{
		case 1:
			as="A1"+as;
			glutChangeToMenuEntry(1,as.c_str(),1);
			break;
		case 2:
			as="A2t"+as;
			glutChangeToMenuEntry(2,as.c_str(),2);
			break;
		case 3:
			as="A2p"+as;
			glutChangeToMenuEntry(3,as.c_str(),3);
			break;
		case 4:
			as="A3"+as;
			glutChangeToMenuEntry(4,as.c_str(),4);
			break;
		case 5:
			as="A4"+as;
			glutChangeToMenuEntry(5,as.c_str(),5);
			break;
		case 6:
			as="S1"+as;
			glutChangeToMenuEntry(1,as.c_str(),6);
			break;
		case 7:
			as="S2t"+as;
			glutChangeToMenuEntry(2,as.c_str(),7);
			break;
		case 8:
			as="S2p"+as;
			glutChangeToMenuEntry(3,as.c_str(),8);
			break;
		case 9:
			as="S3"+as;
			glutChangeToMenuEntry(4,as.c_str(),9);
			break;
		case 10:
			as="S4"+as;
			glutChangeToMenuEntry(5,as.c_str(),10);
			break;
		case 11:
			as="D1"+as;
			glutChangeToMenuEntry(1,as.c_str(),11);
			break;
		case 12:
			as="D2"+as;
			glutChangeToMenuEntry(2,as.c_str(),12);
			break;
		case 13:
			as="S1D1"+as;
			glutChangeToMenuEntry(6,as.c_str(),13);
			break;
		case 14:
			as="S1D2"+as;
			glutChangeToMenuEntry(7,as.c_str(),14);
			break;
		case 15:
			as="S2pD1"+as;
			glutChangeToMenuEntry(10,as.c_str(),15);
			break;
		case 16:
			as="S2tD1v"+as;
			glutChangeToMenuEntry(8,as.c_str(),16);
			break;
		case 17:
			as="S2tD1e"+as;
			glutChangeToMenuEntry(9,as.c_str(),17);
			break;
		case 18:
			as="S2pD2"+as;
			glutChangeToMenuEntry(11,as.c_str(),18);
			break;
		case 19:
			as="S2tD2"+as;
			glutChangeToMenuEntry(12,as.c_str(),19);
			break;
		case 20:
			as="S3D1"+as;
			glutChangeToMenuEntry(13,as.c_str(),20);
			break;
		case 21:
			as="S3D2"+as;
			glutChangeToMenuEntry(14,as.c_str(),21);
			break;
		case 22:
			as="S4D1"+as;
			glutChangeToMenuEntry(15,as.c_str(),22);
			break;
		case 23:
			as="S4D2"+as;
			glutChangeToMenuEntry(16,as.c_str(),23);
			break;
		case 24:
			as="A1S1t"+as;
			glutChangeToMenuEntry(1,as.c_str(),24);
			break;
		case 25:
			as="A1S1tD1v"+as;
			glutChangeToMenuEntry(2,as.c_str(),25);
			break;
		case 26:
			as="A1S1tD1e"+as;
			glutChangeToMenuEntry(3,as.c_str(),26);
			break;
		case 27:
			as="A1S1tD2"+as;
			glutChangeToMenuEntry(4,as.c_str(),27);
			break;
		case 28:
			as="A1S1p"+as;
			glutChangeToMenuEntry(5,as.c_str(),28);
			break;
		case 29:
			as="A1S1pD1"+as;
			glutChangeToMenuEntry(6,as.c_str(),29);
			break;
		case 30:
			as="A1S1pD2"+as;
			glutChangeToMenuEntry(7,as.c_str(),30);
			break;
		case 31:
			as="A1D1"+as;
			glutChangeToMenuEntry(6,as.c_str(),31);
			break;
		case 32:
			as="A1D2"+as;
			glutChangeToMenuEntry(7,as.c_str(),32);
			break;
		case 33:
			as="A1S2t"+as;
			glutChangeToMenuEntry(8,as.c_str(),33);
			break;
		case 34:
			as="A1S2tD1a"+as;
			glutChangeToMenuEntry(9,as.c_str(),34);
			break;
		case 35:
			as="A1S2tD1s"+as;
			glutChangeToMenuEntry(10,as.c_str(),35);
			break;
		case 36:
			as="A1S2tD2"+as;
			glutChangeToMenuEntry(11,as.c_str(),36);
			break;
		case 37:
			as="A1S2p"+as;
			glutChangeToMenuEntry(12,as.c_str(),37);
			break;
		case 38:
			as="A1S2pD1"+as;
			glutChangeToMenuEntry(13,as.c_str(),38);
			break;
		case 39:
			as="A1S2pD2"+as;
			glutChangeToMenuEntry(14,as.c_str(),39);
			break;
		case 40:
			as="A1S3"+as;
			glutChangeToMenuEntry(15,as.c_str(),40);
			break;
		case 41:
			as="A1S3D1"+as;
			glutChangeToMenuEntry(16,as.c_str(),41);
			break;
		case 42:
			as="A1S3D2"+as;
			glutChangeToMenuEntry(17,as.c_str(),42);
			break;
		case 43:
			as="A2tD1v"+as;
			glutChangeToMenuEntry(8,as.c_str(),43);
			break;
		case 44:
			as="A2tD1e"+as;
			glutChangeToMenuEntry(9,as.c_str(),44);
			break;
		case 45:
			as="A2tD2"+as;
			glutChangeToMenuEntry(10,as.c_str(),45);
			break;
		case 46:
			as="A2tS1"+as;
			glutChangeToMenuEntry(1,as.c_str(),46);
			break;
		case 47:
			as="A2tS1D1a"+as;
			glutChangeToMenuEntry(2,as.c_str(),47);
			break;
		case 48:
			as="A2tS1D1s"+as;
			glutChangeToMenuEntry(3,as.c_str(),48);
			break;
		case 49:
			as="A2tS1D2"+as;
			glutChangeToMenuEntry(4,as.c_str(),49);
			break;
		case 50:
			as="A2tS2"+as;
			glutChangeToMenuEntry(5,as.c_str(),50);
			break;
		case 51:
			as="A2tS2D1v"+as;
			glutChangeToMenuEntry(6,as.c_str(),51);
			break;
		case 52:
			as="A2tS2D1e"+as;
			glutChangeToMenuEntry(7,as.c_str(),52);
			break;
		case 53:
			as="A2tS2D2"+as;
			glutChangeToMenuEntry(8,as.c_str(),53);
			break;
		case 54:
			as="A2pD1"+as;
			glutChangeToMenuEntry(11,as.c_str(),54);
			break;
		case 55:
			as="A2pD2"+as;
			glutChangeToMenuEntry(12,as.c_str(),55);
			break;
		case 56:
			as="A2pS1"+as;
			glutChangeToMenuEntry(9,as.c_str(),56);
			break;
		case 57:
			as="A2pS1D1"+as;
			glutChangeToMenuEntry(10,as.c_str(),57);
			break;
		case 58:
			as="A2pS1D2"+as;
			glutChangeToMenuEntry(11,as.c_str(),58);
			break;
		case 59:
			as="A2pS2"+as;
			glutChangeToMenuEntry(12,as.c_str(),59);
			break;
		case 60:
			as="A2pS2D1"+as;
			glutChangeToMenuEntry(13,as.c_str(),60);
			break;
		case 61:
			as="A2pS2D2"+as;
			glutChangeToMenuEntry(14,as.c_str(),61);
			break;
		case 62:
			as="A3D1"+as;
			glutChangeToMenuEntry(13,as.c_str(),62);
			break;
		case 63:
			as="A3D2"+as;
			glutChangeToMenuEntry(14,as.c_str(),63);
			break;
		case 64:
			as="A3S1"+as;
			glutChangeToMenuEntry(15,as.c_str(),64);
			break;
		case 65:
			as="A3S1D1"+as;
			glutChangeToMenuEntry(16,as.c_str(),65);
			break;
		case 66:
			as="A3S1D2"+as;
			glutChangeToMenuEntry(17,as.c_str(),66);
			break;
		case 67:
			as="A4D1"+as;
			glutChangeToMenuEntry(15,as.c_str(),67);
			break;
		case 68:
			as="A4D2"+as;
			glutChangeToMenuEntry(16,as.c_str(),68);
			break;
		case 100:
		case 101:
			if(option==100)
			{
				subs[5]=false;
				subs[6]=false;
				subs[7]=false;
				subs[8]=false;
				subs[9]=false;
				subs[12]=false;
				subs[13]=false;
				subs[15]=false;
				subs[16]=false;
				subs[14]=false;
				subs[17]=false;
				subs[18]=false;
				subs[19]=false;
				subs[20]=false;
				subs[21]=false;
				subs[22]=false;
				tiles.erase(6);
				tiles.erase(7);
				tiles.erase(8);
				tiles.erase(9);
				tiles.erase(10);
				tiles.erase(13);
				tiles.erase(14);
				tiles.erase(16);
				tiles.erase(17);
				tiles.erase(15);
				tiles.erase(18);
				tiles.erase(19);
				tiles.erase(20);
				tiles.erase(21);
				tiles.erase(22);
				tiles.erase(23);
			}
			else
			{
				subs[5]=true;
				subs[6]=true;
				subs[7]=true;
				subs[8]=true;
				subs[9]=true;
				subs[12]=true;
				subs[13]=true;
				subs[15]=true;
				subs[16]=true;
				subs[14]=true;
				subs[17]=true;
				subs[18]=true;
				subs[19]=true;
				subs[20]=true;
				subs[21]=true;
				subs[22]=true;
				as="*";
				tiles.insert(6);
				tiles.insert(7);
				tiles.insert(8);
				tiles.insert(9);
				tiles.insert(10);
				tiles.insert(13);
				tiles.insert(14);
				tiles.insert(16);
				tiles.insert(17);
				tiles.insert(15);
				tiles.insert(18);
				tiles.insert(19);
				tiles.insert(20);
				tiles.insert(21);
				tiles.insert(22);
				tiles.insert(23);
			}
			sas="S1"+as;
			glutChangeToMenuEntry(1,sas.c_str(),6);
			sas="S2t"+as;
			glutChangeToMenuEntry(2,sas.c_str(),7);
			sas="S2p"+as;
			glutChangeToMenuEntry(3,sas.c_str(),8);
			sas="S3"+as;
			glutChangeToMenuEntry(4,sas.c_str(),9);
			sas="S4"+as;
			glutChangeToMenuEntry(5,sas.c_str(),10);
			sas="S1D1"+as;
			glutChangeToMenuEntry(6,sas.c_str(),13);
			sas="S1D2"+as;
			glutChangeToMenuEntry(7,sas.c_str(),14);
			sas="S2tD1v"+as;
			glutChangeToMenuEntry(8,sas.c_str(),16);
			sas="S2tD1e"+as;
			glutChangeToMenuEntry(9,sas.c_str(),17);
			sas="S2pD1"+as;
			glutChangeToMenuEntry(10,sas.c_str(),15);
			sas="S2pD2"+as;
			glutChangeToMenuEntry(11,sas.c_str(),18);
			sas="S2tD2"+as;
			glutChangeToMenuEntry(12,sas.c_str(),19);
			sas="S3D1"+as;
			glutChangeToMenuEntry(13,sas.c_str(),20);
			sas="S3D2"+as;
			glutChangeToMenuEntry(14,sas.c_str(),21);
			sas="S4D1"+as;
			glutChangeToMenuEntry(15,sas.c_str(),22);
			sas="S4D2"+as;
			glutChangeToMenuEntry(16,sas.c_str(),23);
			break;
		case 110:
		case 111:
			if(option==110)
			{
				subs[0]=false;
				subs[1]=false;
				subs[2]=false;
				subs[3]=false;
				subs[4]=false;
				subs[30]=false;
				subs[31]=false;
				subs[42]=false;
				subs[43]=false;
				subs[44]=false;
				subs[53]=false;
				subs[54]=false;
				subs[61]=false;
				subs[62]=false;
				subs[66]=false;
				subs[67]=false;
				tiles.erase(1);
				tiles.erase(2);
				tiles.erase(3);
				tiles.erase(4);
				tiles.erase(5);
				tiles.erase(31);
				tiles.erase(32);
				tiles.erase(43);
				tiles.erase(44);
				tiles.erase(45);
				tiles.erase(54);
				tiles.erase(55);
				tiles.erase(62);
				tiles.erase(63);
				tiles.erase(67);
				tiles.erase(68);
			}
			else
			{
				subs[0]=true;
				subs[1]=true;
				subs[2]=true;
				subs[3]=true;
				subs[4]=true;
				subs[30]=true;
				subs[31]=true;
				subs[42]=true;
				subs[43]=true;
				subs[44]=true;
				subs[53]=true;
				subs[54]=true;
				subs[61]=true;
				subs[62]=true;
				subs[66]=true;
				subs[67]=true;
				as="*";
				tiles.insert(1);
				tiles.insert(2);
				tiles.insert(3);
				tiles.insert(4);
				tiles.insert(5);
				tiles.insert(31);
				tiles.insert(32);
				tiles.insert(43);
				tiles.insert(44);
				tiles.insert(45);
				tiles.insert(54);
				tiles.insert(55);
				tiles.insert(62);
				tiles.insert(63);
				tiles.insert(67);
				tiles.insert(68);
			}
			sas="A1"+as;
			glutChangeToMenuEntry(1,sas.c_str(), 1);
			sas="A2t"+as;
			glutChangeToMenuEntry(2,sas.c_str(), 2);
			sas="A2p"+as;
			glutChangeToMenuEntry(3,sas.c_str(), 3);
			sas="A3"+as;
			glutChangeToMenuEntry(4,sas.c_str(), 4);
			sas="A4"+as;
			glutChangeToMenuEntry(5,sas.c_str(), 5);
			sas="A1D1"+as;
			glutChangeToMenuEntry(6,sas.c_str(), 31);
			sas="A1D2"+as;
			glutChangeToMenuEntry(7,sas.c_str(), 32);
			sas="A2tD1v"+as;
			glutChangeToMenuEntry(8,sas.c_str(), 43);
			sas="A2tD1e"+as;
			glutChangeToMenuEntry(9,sas.c_str(), 44);
			sas="A2tD2"+as;
			glutChangeToMenuEntry(10,sas.c_str(), 45);
			sas="A2pD1"+as;
			glutChangeToMenuEntry(11,sas.c_str(), 54);
			sas="A2pD2"+as;
			glutChangeToMenuEntry(12,sas.c_str(), 55);
			sas="A3D1"+as;
			glutChangeToMenuEntry(13,sas.c_str(), 62);
			sas="A3D2"+as;
			glutChangeToMenuEntry(14,sas.c_str(), 63);
			sas="A4D1"+as;
			glutChangeToMenuEntry(15,sas.c_str(), 67);
			sas="A4D2"+as;
			glutChangeToMenuEntry(16,sas.c_str(), 68);
			break;
			case 120:
			case 121:
				if(option==120)
				{
					subs[24]=false;
					subs[25]=false;
					subs[26]=false;
					subs[27]=false;
					subs[28]=false;
					subs[29]=false;
					subs[30]=false;
					subs[33]=false;
					subs[34]=false;
					subs[35]=false;
					subs[36]=false;
					subs[37]=false;
					subs[38]=false;
					subs[39]=false;
					subs[40]=false;
					subs[41]=false;
					subs[42]=false;
					tiles.erase(24);
					tiles.erase(25);
					tiles.erase(26);
					tiles.erase(27);
					tiles.erase(28);
					tiles.erase(29);
					tiles.erase(30);
					tiles.erase(33);
					tiles.erase(34);
					tiles.erase(35);
					tiles.erase(36);
					tiles.erase(37);
					tiles.erase(38);
					tiles.erase(39);
					tiles.erase(40);
					tiles.erase(41);
					tiles.erase(42);
				}
				else
				{
					subs[24]=true;
					subs[25]=true;
					subs[26]=true;
					subs[27]=true;
					subs[28]=true;
					subs[29]=true;
					subs[30]=true;
					subs[33]=true;
					subs[34]=true;
					subs[35]=true;
					subs[36]=true;
					subs[37]=true;
					subs[38]=true;
					subs[39]=true;
					subs[40]=true;
					subs[41]=true;
					subs[42]=true;
					as="*";
					tiles.insert(24);
					tiles.insert(25);
					tiles.insert(26);
					tiles.insert(27);
					tiles.insert(28);
					tiles.insert(29);
					tiles.insert(30);
					tiles.insert(33);
					tiles.insert(34);
					tiles.insert(35);
					tiles.insert(36);
					tiles.insert(37);
					tiles.insert(38);
					tiles.insert(39);
					tiles.insert(40);
					tiles.insert(41);
					tiles.insert(42);
				}
				sas="A1S1t"+as;
				glutChangeToMenuEntry(1,sas.c_str(), 24);
				sas="A1S1tD1v"+as;
				glutChangeToMenuEntry(2,sas.c_str(), 25);
				sas="A1S1tD1e"+as;
				glutChangeToMenuEntry(3,sas.c_str(), 26);
				sas="A1S1tD2"+as;
				glutChangeToMenuEntry(4,sas.c_str(), 27);
				sas="A1S1p"+as;
				glutChangeToMenuEntry(5,sas.c_str(), 28);
				sas="A1S1pD1"+as;
				glutChangeToMenuEntry(6,sas.c_str(), 29);
				sas="A1S1pD2"+as;
				glutChangeToMenuEntry(7,sas.c_str(), 30);
				sas="A1S2t"+as;
				glutChangeToMenuEntry(8,sas.c_str(), 33);
				sas="A1S2tD1a"+as;
				glutChangeToMenuEntry(9,sas.c_str(), 34);
				sas="A1S2tD1s"+as;
				glutChangeToMenuEntry(10,sas.c_str(), 35);
				sas="A1S2tD2"+as;
				glutChangeToMenuEntry(11,sas.c_str(), 36);
				sas="A1S2p"+as;
				glutChangeToMenuEntry(12,sas.c_str(), 37);
				sas="A1S2pD1"+as;
				glutChangeToMenuEntry(13,sas.c_str(), 38);
				sas="A1S2pD2"+as;
				glutChangeToMenuEntry(14,sas.c_str(), 39);
				sas="A1S3"+as;
				glutChangeToMenuEntry(15,sas.c_str(), 40);
				sas="A1S3D1"+as;
				glutChangeToMenuEntry(16,sas.c_str(), 41);
				sas="A1S3D2"+as;
				glutChangeToMenuEntry(17,sas.c_str(), 42);
				break;
			case 130:
			case 131:
				if(option==130)
				{
					subs[45]=false;
					subs[46]=false;
					subs[47]=false;
					subs[48]=false;
					subs[49]=false;
					subs[50]=false;
					subs[51]=false;
					subs[52]=false;
					subs[55]=false;
					subs[56]=false;
					subs[57]=false;
					subs[58]=false;
					subs[59]=false;
					subs[60]=false;
					subs[63]=false;
					subs[64]=false;
					subs[65]=false;
					tiles.erase(46);
					tiles.erase(47);
					tiles.erase(48);
					tiles.erase(49);
					tiles.erase(50);
					tiles.erase(51);
					tiles.erase(52);
					tiles.erase(53);
					tiles.erase(56);
					tiles.erase(57);
					tiles.erase(58);
					tiles.erase(59);
					tiles.erase(60);
					tiles.erase(61);
					tiles.erase(64);
					tiles.erase(65);
					tiles.erase(66);
				}
				else
				{
					subs[45]=true;
					subs[46]=true;
					subs[47]=true;
					subs[48]=true;
					subs[49]=true;
					subs[50]=true;
					subs[51]=true;
					subs[52]=true;
					subs[55]=true;
					subs[56]=true;
					subs[57]=true;
					subs[58]=true;
					subs[59]=true;
					subs[60]=true;
					subs[63]=true;
					subs[64]=true;
					subs[65]=true;
					as="*";
					tiles.insert(46);
					tiles.insert(47);
					tiles.insert(48);
					tiles.insert(49);
					tiles.insert(50);
					tiles.insert(51);
					tiles.insert(52);
					tiles.insert(53);
					tiles.insert(56);
					tiles.insert(57);
					tiles.insert(58);
					tiles.insert(59);
					tiles.insert(60);
					tiles.insert(61);
					tiles.insert(64);
					tiles.insert(65);
					tiles.insert(66);
				}
				sas="A2tS1"+as;
				glutChangeToMenuEntry(1,sas.c_str(), 46);
				sas="A2tS1D1a"+as;
				glutChangeToMenuEntry(2,sas.c_str(), 47);
				sas="A2tS1D1s"+as;
				glutChangeToMenuEntry(3,sas.c_str(), 48);
				sas="A2tS1D2"+as;
				glutChangeToMenuEntry(4,sas.c_str(), 49);
				sas="A2tS2"+as;
				glutChangeToMenuEntry(5,sas.c_str(), 50);
				sas="A2tS2D1v"+as;
				glutChangeToMenuEntry(6,sas.c_str(), 51);
				sas="A2tS2D1e"+as;
				glutChangeToMenuEntry(7,sas.c_str(), 52);
				sas="A2tS2D2"+as;
				glutChangeToMenuEntry(8,sas.c_str(), 53);
				sas="A2pS1"+as;
				glutChangeToMenuEntry(9,sas.c_str(), 56);
				sas="A2pS1D1"+as;
				glutChangeToMenuEntry(10,sas.c_str(), 57);
				sas="A2pS1D2"+as;
				glutChangeToMenuEntry(11,sas.c_str(), 58);
				sas="A2pS2"+as;
				glutChangeToMenuEntry(12,sas.c_str(), 59);
				sas="A2pS2D1"+as;
				glutChangeToMenuEntry(13,sas.c_str(), 60);
				sas="A2pS2D2"+as;
				glutChangeToMenuEntry(14,sas.c_str(), 61);
				sas="A3S1"+as;
				glutChangeToMenuEntry(15,sas.c_str(), 64);
				sas="A3S1D1"+as;
				glutChangeToMenuEntry(16,sas.c_str(), 65);
				sas="A3S1D2"+as;
				glutChangeToMenuEntry(17,sas.c_str(), 66);
				break;
			case 140:
			case 141:
				if(option==140)
				{
					subs[10]=false;
					subs[11]=false;
					tiles.erase(11);
					tiles.erase(12);
				}
				else
				{
					subs[10]=true;
					subs[11]=true;
					as="*";
					tiles.insert(11);
					tiles.insert(12);
				}
				sas="D1"+as;
				glutChangeToMenuEntry(1,sas.c_str(), 11);
				sas="D2"+as;
				glutChangeToMenuEntry(2,sas.c_str(), 12);
				break;


	}


}
void keys(unsigned char s, GLint x,GLint y)//handles keyboard input for chars
{
	switch(s)
	{
		case 'n'://creats new floor using current sqs and depth
			createnewfloor();
			break;
		case 'f'://increases animation speed by decreaseing refreshnum to half
			if(refreshnum>100)
				refreshnum/=2;
			break;
		case 's'://decreases animation speed by increaseing refreshnum by 2 times
			if(refreshnum<10000000)
				refreshnum*=2;
			break;
		case 'F'://increases animation speed by decreaseing refreshnum by 10-fold
			if(refreshnum>100)
				refreshnum/=10;
			break;
		case 'S'://decreases animation speed by increaseing refreshnum by 10-fold
			if(refreshnum<10000000)
				refreshnum*=10;
			break;
	}
}
void skeys(GLint key,GLint x,GLint y)//handles arrow keys input
{
	if(sp)//if not currently doing somthing frome the arrows input
	{
		sp=false;
		GLboolean temp=false;
		switch(key)
		{
			case GLUT_KEY_UP://up arrow increases the number of tiles in a unit by one. makes the needed adjustments and calculations and draws floor
				if(idlego)
				{
					idlego=false;
					temp=true;
				}
				sqs++;
				anind=0;
				sqsize=XMAX/sqs;
				divi=1500.0;
				if((sqs*std::pow(2.0,depth))>15)
					divi=divi-1400;
				else if((sqs*std::pow(2.0,depth))>7)
					divi=divi-1300;
				else if((sqs*std::pow(2.0,depth))>3)
					divi=divi-1000;
				else if(sqs*std::pow(2.0,depth)>1)
					divi=divi-700;
				hsqsize=sqsize/2.0;
				delta=sqsize/divi;
				R=sqrt(2.0*pow(hsqsize,2));
				Arcbot();
				//Arctop();
				//Arcleft();
				//Arcright();
				floorincrease();
				glutSwapBuffers();
				if(temp)
					idlego=true;
				break;
			case GLUT_KEY_DOWN://up arrow decreases the number of tiles in a unit by one. makes the needed adjustments and calculations and draws floor
				if(sqs>1)
				{
					if(idlego)
					{
						idlego=false;
						temp=true;
					}
					sqs--;
					anind=0;
					sqsize=XMAX/sqs;
					divi=1500.0;
					if((sqs*std::pow(2.0,depth))>15)
						divi=divi-1400;
					else if((sqs*std::pow(2.0,depth))>7)
						divi=divi-1300;
					else if((sqs*std::pow(2.0,depth))>3)
						divi=divi-1000;
					else if(sqs*std::pow(2.0,depth)>1)
						divi=divi-700;
					hsqsize=sqsize/2.0;
					delta=sqsize/divi;
					R=sqrt(2.0*pow(hsqsize,2));
					Arcbot();
					//Arctop();
					//Arcleft();
					//Arcright();
					floordecrease();
					glutSwapBuffers();
					if(temp)
						idlego=true;
				}
				sp=true;
				break;
			case GLUT_KEY_LEFT://left arrow decreases the number of 'folds' in floor by one. makes the needed adjustments and calculations and draws floor
				if(depth>0)
				{
					if(idlego)
					{
						idlego=false;
						temp=true;
					}
					depth--;
					divi=1500.0;
					if((sqs*std::pow(2.0,depth))>15)
						divi=divi-1400;
					else if((sqs*std::pow(2.0,depth))>7)
						divi=divi-1300;
					else if((sqs*std::pow(2.0,depth))>3)
						divi=divi-1000;
					else if(sqs*std::pow(2.0,depth)>1)
					divi=divi-700;
					delta=sqsize/divi;
					R=sqrt(2.0*pow(hsqsize,2));
					Arcbot();
					//Arctop();
					//Arcleft();
					//Arcright();
					depthdecrease();
					glutSwapBuffers();
					if(temp)
						idlego=true;
				}
				sp=true;
				break;
			case GLUT_KEY_RIGHT://right arrow increases the number of 'folds' in floor by one. makes the needed adjustments and calculations and draws floor
				if(idlego)
				{
					idlego=false;
					temp=true;
				}
				depth++;		
				divi=1500.0;
				if((sqs*std::pow(2.0,depth))>15)
					divi=divi-1400;
				else if((sqs*std::pow(2.0,depth))>7)
					divi=divi-1300;
				else if((sqs*std::pow(2.0,depth))>3)
					divi=divi-1000;
				else if(sqs*std::pow(2.0,depth)>1)
					divi=divi-700;
				delta=sqsize/divi;
				R=sqrt(2.0*pow(hsqsize,2));
				Arcbot();
				//Arctop();
				//Arcleft();
				//Arcright();
				depthincreased();
				glutSwapBuffers();
				if(temp)
					idlego=true;
				sp=true;
				break;
		}
		
	}
}
void processRightMenuEvents(GLint option) {
	
}
void idle()
{
	if(idlego)
	{
		if(on%(refreshnum)==0)
		{
			GLint ind=sqs-1-anind%(GLint)sqs;
			anind++;
			if(tiles.size()==0)
				return;
			GLint select=(rand())%tiles.size();
			std::set<GLint>::iterator ttt;
			ttt=tiles.begin();
			for(;select>0;++ttt,select--);
			gg[ind][ind]=(*ttt)*100+((rand())%4)*10+(rand())%2;
			for(GLint z=ind+1;z<sqs;z++)
			{
				select=(rand())%tiles.size();
				ttt=tiles.begin();
				for(;select>0;++ttt,select--);
				gg[z][ind]=(*ttt)*100+((rand())%4)*10+(rand())%2;
				select=(rand())%tiles.size();
				ttt=tiles.begin();
				for(;select>0;++ttt,select--);
				gg[ind][z]=(*ttt)*100+((rand())%4)*10+(rand())%2;
			}
			glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
			//float cc=1.000000000/sqs;
			glColor4d((GLdouble)(rand()%1001)/1000.0,(GLdouble)(rand()%1001)/1000.0,(GLdouble)(rand()%1001)/1000.0,1.0);
			//glColor4f(1,1,1,1);
			glLoadIdentity();
			GLint sn=std::pow(2.0,depth);
			gluOrtho2D(0, XMAX*sn,0, XMAX*sn);
			GLint temp=0;
			//int select=0;
			GLint rot=0;
			GLint flip=0;
			GLdouble x2XMAX=2*XMAX;
			GLint tem=std::pow(2.0,depth-1);
			GLdouble xx0=0.00;
			GLdouble yy0=0.00;
			GLdouble xx1=0.00;
			GLdouble yy1=0.00;
			GLint lala=0;
			GLint rara=0;
			if(depth!=0)
				for(GLint y=0;y<sqs;y++)
				{
					for(GLint x=0;x<sqs;x++)
					{
						temp=gg[y][x];
						flip=temp%2;
						rot=(temp/10)%10%4;
						select=(temp/100);
						xx0=((GLdouble)x)*sqsize;
						yy0=((GLdouble)y)*sqsize;
						xx1=x2XMAX-(xx0+sqsize);
						yy1=x2XMAX-(yy0+sqsize);
						for(GLint m=0; m<tem;m++)
						{
							rara=2*m*sqs+y;
							for(GLint n=0;n<tem;n++)
							{
								glPushMatrix();
								lala=2*n*sqs+x;
								gg[rara][lala]=select*100+rot*10+flip;
								transform(rot,flip,select,(xx0+n*x2XMAX),(yy0+m*x2XMAX));
								glPopMatrix();
								flip=(flip+1)%2;
								glPushMatrix();
								lala=2*(n+1)*sqs-1-x;
								gg[rara][lala]=select*100+rot*10+flip;
								transform(rot,flip,select,xx1+n*x2XMAX,yy0+m*x2XMAX);
								glPopMatrix();
								flip=(flip+1)%2;
							}
							rot=(rot+2)%4;
							flip=(flip+1)%2;
							rara=2*(m+1)*sqs-1-y;
							for(GLint n=0;n<tem;n++)
							{
								glPushMatrix();
								lala=2*n*sqs+x;
								gg[rara][lala]=select*100+rot*10+flip;
								transform(rot,flip,select,xx0+n*x2XMAX,yy1+m*x2XMAX);
								glPopMatrix();
								flip=(flip+1)%2;
								glPushMatrix();
								lala=2*(n+1)*sqs-1-x;
								gg[rara][lala]=select*100+rot*10+flip;
								transform(rot,flip,select,xx1+n*x2XMAX,yy1+m*x2XMAX);
								glPopMatrix();
								flip=(flip+1)%2;
							}
							rot=(rot+2)%4;
							flip=(flip+1)%2;
						}
					}
				}
			else
				for(GLint i=0;i<sqs;i++)
					for(GLint ii=0;ii<sqs;ii++)
					{
						select=(gg[i][ii]/100);
						rot=((gg[i][ii]/10)%10)%4;
						flip=gg[i][ii]%2;
						glPushMatrix();
						transform(rot,flip,select,((GLdouble)ii)*sqsize,((GLdouble)i)*sqsize);
						glPopMatrix();
					}
			glutSwapBuffers();
		}
		on++;
	}
}
void processLeftMenuEvents(GLint option)// left mouse click menu
{
	switch(option)
	{
		case 0://when remake is selected it creates a new randomized floor of unit size of sqs tiles and mirrored with depth mirrored 'folds'
			glutDisplayFunc(myDisplay);     // register the redraw function
			glutIdleFunc(idle);
			createnewfloor();
			break;
		case 1://when animation is selected it changes the bool starting/stoping the idle function
			idlego=!idlego;
			break;
		case 2:
			idlego=true;
			heart();
			idlego=false;
			glutDisplayFunc(heart);     // register the redraw function
			glutIdleFunc(heart);
			break;
	}
}
//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
void main(int argc, char **argv)
{
	gg.resize(sqs,std::vector<GLint>(sqs,0));
	std::srand(std::time(0));
	std::set<GLint>::iterator ttt;
	GLint select=0;
	for(GLint z=1;z<69;z++)
	{
		subs[z]=true;
		tiles.insert(z);
	}
	for(GLint z=0;z<sqs;z++)
		for(GLint zz=0;zz<sqs;zz++)
		{
			select=(rand())%tiles.size();
			ttt=tiles.begin();
			for(;select>0;++ttt,select--);
			gg[z][zz]=(*ttt)*100+((rand())%4)*10+(rand())%2;
		}		
	sqsize=XMAX/sqs;
	hsqsize=sqsize/2.0;
	//toparc.resize(1501,std::vector<float>(2,0));
	botarc.resize(1501,std::vector<GLdouble>(2,0));
	//leftarc.resize(1501,std::vector<float>(2,0));
	//rightarc.resize(1501,std::vector<float>(2,0));
	delta=sqsize/divi;
	R=sqrt(2.0*pow(hsqsize,2));
	Arcbot();
	//Arctop();
	//Arcleft();
	//Arcright();
	glutInit(&argc, argv);          // initialize the toolkit
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // set the display mode
	glutInitWindowSize(2*680,2*480);     // set the window size
	glutInitWindowPosition(100, 150); // set the window position on the screen
	glutCreateWindow("tiles"); // open the screen window(with its exciting title)
	glutDisplayFunc(myDisplay);     // register the redraw function
	glutIdleFunc(idle);
	myInit();        
	glutCreateMenu(processLeftMenuEvents);
	glutAddMenuEntry("tiles",0);
	glutAddMenuEntry("animation",1);
	glutAddMenuEntry("heart",2);
	glutAttachMenu(GLUT_LEFT_BUTTON);

	GLint lines, arcs,combo1,combo2,diag;

	lines = glutCreateMenu(tileselect);
	glutAddMenuEntry("S1*",6);
	glutAddMenuEntry("S2t*",7);
	glutAddMenuEntry("S2p*",8);
	glutAddMenuEntry("S3*",9);
	glutAddMenuEntry("S4*",10);
	glutAddMenuEntry("S1D1*",13);
	glutAddMenuEntry("S1D2*",14);
	glutAddMenuEntry("S2tD1v*",16);
	glutAddMenuEntry("S2tD1e*",17);
	glutAddMenuEntry("S2pD1*",15);
	glutAddMenuEntry("S2pD2*",18);
	glutAddMenuEntry("S2tD2*",19);
	glutAddMenuEntry("S3D1*",20);
	glutAddMenuEntry("S3D2*",21);
	glutAddMenuEntry("S4D1*",22);
	glutAddMenuEntry("S4D2*",23);
	glutAddMenuEntry("remove all these",100);
	glutAddMenuEntry("add all these",101);


	arcs = glutCreateMenu(tileselect);
	glutAddMenuEntry("A1*", 1);
	glutAddMenuEntry("A2t*", 2);
	glutAddMenuEntry("A2p*", 3);
	glutAddMenuEntry("A3*", 4);
	glutAddMenuEntry("A4*", 5);
	glutAddMenuEntry("A1D1*", 31);
	glutAddMenuEntry("A1D2*", 32);
	glutAddMenuEntry("A2tD1v*", 43);
	glutAddMenuEntry("A2tD1e*", 44);
	glutAddMenuEntry("A2tD2*", 45);
	glutAddMenuEntry("A2pD1*", 54);
	glutAddMenuEntry("A2pD2*", 55);
	glutAddMenuEntry("A3D1*", 62);
	glutAddMenuEntry("A3D2*", 63);
	glutAddMenuEntry("A4D1*", 67);
	glutAddMenuEntry("A4D2*", 68);
	glutAddMenuEntry("remove all these",110);
	glutAddMenuEntry("add all these",111);

	combo1= glutCreateMenu(tileselect);
	glutAddMenuEntry("A1S1t*", 24);
	glutAddMenuEntry("A1S1tD1v*", 25);
	glutAddMenuEntry("A1S1tD1e*", 26);
	glutAddMenuEntry("A1S1tD2*", 27);
	glutAddMenuEntry("A1S1p*", 28);
	glutAddMenuEntry("A1S1pD1*", 29);
	glutAddMenuEntry("A1S1pD2*", 30);
	glutAddMenuEntry("A1S2t*", 33);
	glutAddMenuEntry("A1S2tD1a*", 34);
	glutAddMenuEntry("A1S2tD1s*", 35);
	glutAddMenuEntry("A1S2tD2*", 36);
	glutAddMenuEntry("A1S2p*", 37);
	glutAddMenuEntry("A1S2pD1*", 38);
	glutAddMenuEntry("A1S2pD2*", 39);
	glutAddMenuEntry("A1S3*", 40);
	glutAddMenuEntry("A1S3D1*", 41);
	glutAddMenuEntry("A1S3D2*", 42);
	glutAddMenuEntry("remove all these",120);
	glutAddMenuEntry("add all these",121);

	combo2= glutCreateMenu(tileselect);
	glutAddMenuEntry("A2tS1*", 46);
	glutAddMenuEntry("A2tS1D1a*", 47);
	glutAddMenuEntry("A2tS1D1s*", 48);
	glutAddMenuEntry("A2tS1D2*", 49);
	glutAddMenuEntry("A2tS2*", 50);
	glutAddMenuEntry("A2tS2D1v*", 51);
	glutAddMenuEntry("A2tS2D1e*", 52);
	glutAddMenuEntry("A2tS2D2*", 53);
	glutAddMenuEntry("A2pS1*", 56);
	glutAddMenuEntry("A2pS1D1*", 57);
	glutAddMenuEntry("A2pS1D2*", 58);
	glutAddMenuEntry("A2pS2*", 59);
	glutAddMenuEntry("A2pS2D1*", 60);
	glutAddMenuEntry("A2pS2D2*", 61);
	glutAddMenuEntry("A3S1*", 64);
	glutAddMenuEntry("A3S1D1*", 65);
	glutAddMenuEntry("A3S1D2*", 66);
	glutAddMenuEntry("remove all these",130);
	glutAddMenuEntry("add all these",131);

	diag= glutCreateMenu(tileselect);
	glutAddMenuEntry("D1*", 11);
	glutAddMenuEntry("D2*", 12);
	glutAddMenuEntry("remove all these",140);
	glutAddMenuEntry("add all these",141);

	glutKeyboardFunc(keys);
	glutSpecialFunc(skeys);
	glutCreateMenu(processRightMenuEvents);
	glutAddSubMenu("D",diag);
	glutAddSubMenu("S",lines);
	glutAddSubMenu("A",arcs);
	glutAddSubMenu("combo1",combo1);
	glutAddSubMenu("combo2",combo2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);      
	glutMainLoop(); 		     // go into a perpetual loop
}
//1:A1//
//2:A2t//
//3:A2p//
//4:A3//
//5:A4//
//6:S1//
//7:S2t//
//8:S2p//
//9:S3//
//10:S4//
//11:D1//
//12:D2//
//13:S1D1//
//14:S1D2//
//15:S2pD1//
//16:S2tD1v//
//17:S2tD1e//
//18:S2pD2//
//19:S2tD2//
//20:S3D1//
//21:S3D2//
//22:S4D1//
//23:S4D2//
//24:A1S1t//
//25:A1S1tD1v//
//26:A1S1tD1e//
//27:A1S1tD2//
//28:A1S1p//
//29:A1S1pD1//
//30:A1S1pD2//
//31:A1D1//
//32:A1D2//
//33:A1S2t//
//34:A1S2tD1a//
//35:A1S2tD1s//
//36:A1S2tD2//
//37:A1S2p//
//38:A1S2pD1//
//39:A1S2pD2//
//40:A1S3//
//41:A1S3D1//
//42:A1S3D2//
//43:A2tD1v//
//44:A2tD1e//
//45:A2tD2//
//46:A2tS1//
//47:A2tS1D1a//
//48:A2tS1D1s//
//49:A2tS1D2//
//50:A2tS2//
//51:A2tS2D1v//
//52:A2tS2D1e//
//53:A2tS2D2//
//54:A2pD1//
//55:A2pD2//
//56:A2pS1//
//57:A2pS1D1//
//58:A2pS1D2//
//59:A2pS2//
//60:A2pS2D1//
//61:A2pS2D2//
//62:A3D1//
//63:A3D2//
//64:A3S1//
//65:A3S1D1//
//66:A3S1D2//
//67:A4D1//
//68:A4D2//