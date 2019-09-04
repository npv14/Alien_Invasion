//  ========================================================================
//  COSC363: Computer Graphics (2018);  University of Canterbury.
//
//  FILE NAME: Cannon.cpp
//  See Lab02.pdf for details
//
//  Program displays the model of a cannon (mesh data from Cannon.off)
//  Use left/right arrow keys to rotate the scene
//  Use up/down arrow keys to move camera up/down
//  ========================================================================

#include <iostream>
#include <fstream>
#include <climits>
#include <math.h>
#include <GL/freeglut.h>
#include "loadTGA.h"
#include "loadBMP.h"
using namespace std;
GLuint txId[2];
GLUquadricObj*	q;
//--Globals ---------------------------------------------------------------
int count = 0;
float theta = 0;
float alpha = 0;
float teaPort = 0;
float robotMovement = 20.0;
float *x, *y, *z;  //vertex coordinate arrays
int *t1, *t2, *t3; //triangles
int nvrt, ntri;    //total number of vertices and triangles
float angle = -90;  //Rotation angle for viewing
float upAngle = 20;
float cam_hgt = 400;
int flashing = 0;
float cam_up = 0;
float cam_down = 0;
int boo = 0;
int gateAnimation = 0;
int floorbase = 2;
float robotAngleMove = 1;
int shipLiftOff = 0;
float spaceShipHeight = 0;
float look_y = 200;
float  look_x = 0;
float  look_z = -1;
float  eye_x = 500;
float  eye_z = 0;
 float toR = 3.141592654/180;
 int camChange = 0;
//-- Loads mesh data in OFF format    -------------------------------------

 // Particle System
#define MAX_PAR 10000
int NumCurrentPar = 0;
//--------------------------------------------------------------------------------

double drand10()
{
    int ranVa = rand();
//    cout<< ranVa%20 << endl;
    return ranVa%20;
}

void idle(void)
{
    glutPostRedisplay();
}
double drand10000()
{
    int ranVa = rand();
//    cout<< ranVa%10000 << endl;
    return ranVa%10000;
}

//Partical System
typedef struct{
    int alive;
    float x;
    float y;
    float z;
    float xdir;
    float ydir;
    float zdir;
    float xinit;
    float yinit;
    float zinit;

} Particle;

Particle fire[MAX_PAR];

void draw_fire(void){
    glColor3f(0.121, 0.611, 1);
        glBegin(GL_POINTS);
        for (int i=0 ; i<NumCurrentPar ; i++ ){
            if (fire[i].alive) {
                glScaled(3,3,3);
                glVertex3f(fire[i].x, fire[i].y, fire[i].z);
            }
        }
        glEnd();
        glFlush();
}

void Update(void){
    for(int i = 0; i < NumCurrentPar; i++){
         if (fire[i].alive == 1) {
             fire[i].x += fire[i].xdir;
             fire[i].y += fire[i].ydir;
             fire[i].z += fire[i].zdir;
         }

         if (fire[i].y > fire[i].yinit + 100 || fire[i].x > fire[i].xinit + 60 || fire[i].x < fire[i].xinit -60 || fire[i].z > fire[i].zinit + 60 || fire[i].z < fire[i].zinit + -60){
                     fire[i].x = fire[i].xinit;
                     fire[i].z = fire[i].zinit;
                     fire[i].y = fire[i].yinit;

         }


         if (fire[i].y < fire[i].yinit + 20 && (fire[i].x > fire[i].xinit + 10 || fire[i].x < fire[i].xinit - 10 || fire[i].z > fire[i].zinit + 10 || fire[i].z < fire[i].zinit + -10) ){
             fire[i].x = fire[i].xinit;
             fire[i].z = fire[i].zinit;
             fire[i].y = fire[i].yinit;
         }

         if (fire[i].y < fire[i].yinit + 30 && (fire[i].x > fire[i].xinit + 15 || fire[i].x < fire[i].xinit - 15 || fire[i].z > fire[i].zinit + 15 || fire[i].z < fire[i].zinit + -15) ){
             fire[i].x = fire[i].xinit;
             fire[i].z = fire[i].zinit;
             fire[i].y = fire[i].yinit;
         }

         if (fire[i].y < fire[i].yinit + 10 && (fire[i].x > fire[i].xinit + 5 || fire[i].x < fire[i].xinit + -5 || fire[i].z > fire[i].zinit + 5 || fire[i].z < fire[i].zinit + -5) ){
             fire[i].x = fire[i].xinit;
             fire[i].z = fire[i].zinit;
             fire[i].y = fire[i].yinit;
         }

    }
}

void fire_generator(void){
    float newdrops = drand10000();
//    cout<< newdrops << endl;
    if (NumCurrentPar + newdrops > MAX_PAR){
            newdrops = MAX_PAR - NumCurrentPar;
        }

    for ( int i=NumCurrentPar ; i<NumCurrentPar+newdrops ; i++ ) {
            fire[i].alive = 1;
            fire[i].x = -130;
            fire[i].y = 140;
            fire[i].z = 0;
            fire[i].xinit = -130;
            fire[i].yinit = 140;
            fire[i].zinit = 0;

            int getRan =  10- drand10();
            cout <<10- drand10()<< endl;
            fire[i].xdir = 10- drand10();
            fire[i].ydir = drand10();
            fire[i].zdir = 10- drand10();
        }
     NumCurrentPar += newdrops;
}


 const int N = 50;
 float vx[N] = {0, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
               1, 1, 5, 5, 4, 4, 3, 3, 2, 2};


 float vy[N] = {0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
                39, 40, 41, 42, 43, 43, 42, 41, 40, 39};
 float vz[N] = {0};

 void normal(float x1, float y1, float z1,
             float x2, float y2, float z2,
               float x3, float y3, float z3 )
 {
       float nx, ny, nz;
       nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
       ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
       nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

       glNormal3f(-nx, -ny, -nz);
 }


 void drawVast(void){
     float wx[N], wy[N], wz[N];
     float angStep = 10.0*3.1415926/180.0;  //Rotate in 10 deg steps (converted to radians)
     glColor4f (1.0, 0.75, 0.5, 1.0);    //Temporary
     //  Include code for drawing the surface of revolution here.
     for(int j = 0; j < 36; j++) //36 slices in 10 deg steps
         {
             for(int i = 0; i < N; i++)
             {

                 wx[i] =(vx[i] * cos(angStep)) + (vz[i] * sin(angStep));
                 wy[i] = vy[i];
                 wz[i] = (-vx[i] * sin(angStep)) + ( vz[i] * cos(angStep));

                 glBegin(GL_TRIANGLE_STRIP);

                  if(i > 0) normal( wx[i-1], wy[i-1], wz[i-1],
                  vx[i-1], vy[i-1], vz[i-1],
                 vx[i], vy[i], vz[i] );

                  glTexCoord2f(j/36.0, i/(float)N);
                  glVertex3f(vx[i], vy[i], vz[i]);

                  if(i > 0) normal( wx[i-1], wy[i-1], wz[i-1],
                  vx[i], vy[i], vz[i],
                  wx[i], wy[i], wz[i] );

                  glTexCoord2f((j+1)/36.0, i/(float)N);
                  glVertex3f(wx[i], wy[i], wz[i]);
             }

             glEnd();
             for (int k = 0; k < N; k++)
                {
                    vx[k] = wx[k];
                    vy[k] = wy[k];
                    vz[k] = wz[k];
                }
         }
 }


void loadMeshFile(const char* fname)
{
    ifstream fp_in;
    int num, ne;

    fp_in.open(fname, ios::in);
    if(!fp_in.is_open())
    {
        cout << "Error opening mesh file" << endl;
        exit(1);
    }

    fp_in.ignore(INT_MAX, '\n');				//ignore first line
    fp_in >> nvrt >> ntri >> ne;			    // read number of vertices, polygons, edges

    x = new float[nvrt];                        //create arrays
    y = new float[nvrt];
    z = new float[nvrt];

    t1 = new int[ntri];
    t2 = new int[ntri];
    t3 = new int[ntri];

    for(int i=0; i < nvrt; i++)                         //read vertex list
        fp_in >> x[i] >> y[i] >> z[i];

    for(int i=0; i < ntri; i++)                         //read polygon list
    {
        fp_in >> num >> t1[i] >> t2[i] >> t3[i];
        if(num != 3)
        {
            cout << "ERROR: Polygon with index " << i  << " is not a triangle." << endl;  //not a triangle!!
            exit(1);
        }
    }

    fp_in.close();
    cout << " File successfully read." << endl;
}

//--Function to compute the normal vector of a triangle with index tindx ----------
void normal(int tindx)
{
    float x1 = x[t1[tindx]], x2 = x[t2[tindx]], x3 = x[t3[tindx]];
    float y1 = y[t1[tindx]], y2 = y[t2[tindx]], y3 = y[t3[tindx]];
    float z1 = z[t1[tindx]], z2 = z[t2[tindx]], z3 = z[t3[tindx]];
    float nx, ny, nz;
    nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
    ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
    nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
    glNormal3f(nx, ny, nz);
}

//--------draws the mesh model of the cannon----------------------------
void drawCannon()
{


    //Construct the object model here using triangles read from OFF file
    glBegin(GL_TRIANGLES);
        for(int tindx = 0; tindx < ntri; tindx++)
        {
           normal(tindx);
           glVertex3d(x[t1[tindx]], y[t1[tindx]], z[t1[tindx]]);
           glVertex3d(x[t2[tindx]], y[t2[tindx]], z[t2[tindx]]);
           glVertex3d(x[t3[tindx]], y[t3[tindx]], z[t3[tindx]]);
        }
    glEnd();
}





//----------draw a floor plane-------------------
void drawFloor()
{
    bool flag = false;

    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    for(int x = -1000; x <= 1000; x += 20)
    {
        for(int z = -1000; z <= 1000; z += 20)
        {

            glColor3f(0.184, 0.329, 0.352);
            glVertex3f(x,0, z);
            glVertex3f(x, 0, z+20);
            glVertex3f(x+20, 0, z+20);
            glVertex3f(x+20, 0, z);
            flag = !flag;
        }
    }
    glEnd();
}
// load TGA tecture
GLuint texId[9];
void loadGLTextures()				// Load bitmaps And Convert To Textures
{
    glGenTextures(9, texId); 		// Create texture ids
    // *** left ***
    glBindTexture(GL_TEXTURE_2D, texId[0]);
    loadTGA("totality_bk.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** front ***
    glBindTexture(GL_TEXTURE_2D, texId[1]);
    loadTGA("totality_rt.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** right ***
    glBindTexture(GL_TEXTURE_2D, texId[2]);
    loadTGA("totality_ft.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** back***
    glBindTexture(GL_TEXTURE_2D, texId[3]);
    loadTGA("totality_lf.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** top ***
    glBindTexture(GL_TEXTURE_2D, texId[4]);
    loadTGA("totality_up.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** down ***
    glBindTexture(GL_TEXTURE_2D, texId[5]);
    loadTGA("totality_dn.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, texId[6]);  //Use this texture name for the following OpenGL texture
    loadTGA("123.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texId[7]);  //Use this texture name for the following OpenGL texture
    loadTGA("wall1.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texId[8]);  //Use this texture name for the following OpenGL texture
    loadTGA("Sun.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

}

//void loadTexture()
//{
//    glGenTextures(2, txId);   //Get 2 texture IDs
//    glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture name for the following OpenGL texture
//    loadBMP("images (1).bmp");
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//    glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture name for the following OpenGL texture
//    loadBMP("Sun.bmp");
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//}

void createCannon(int x, float scale){
    glPushMatrix();
        glTranslatef(x, 0, 0);
        glScalef(scale, scale, scale);

        glPushMatrix();
        glTranslatef(-20, 30, 0);
        glRotatef(30.0, 0.0, 0.0, 1.0);
        glTranslatef(20, -30, 0);
        drawCannon();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-10, 5, 17);
        glScalef(80, 10, 6);
        glutSolidCube(1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-20, 25, 17);
        glScalef(40, 30, 6);
        glutSolidCube(1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-10, 5, -17);
        glScalef(80, 10, 6);
        glutSolidCube(1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-20, 25, -17);
        glScalef(40, 30, 6);
        glutSolidCube(1);
        glPopMatrix();

        glPushMatrix();
        float yEquation = (-0.01* alpha * alpha + 2 * alpha ) ;
        //collision detection
        if(yEquation < -35){
            alpha = 0;
            count += 1;
        }
        if (count == 0){
            glTranslatef(38.88+theta, 64+(-0.01* alpha * alpha + 2 * alpha ), 0);
        }
        else{
            glTranslatef(38.88+theta, (-0.01* alpha * alpha + 2 * alpha)/ (2*(count * count)), 0);
        }

        glutSolidSphere(5, 36, 18);
        glPopMatrix();
    glPopMatrix();
}

void createPillars(){


    //Create front wall
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    glBindTexture(GL_TEXTURE_2D, texId[7]);


    glPushMatrix();
      glTranslatef(200, floorbase, 150-65);
      glScalef(10, 170, 80);
      glutSolidCube(1.0);
    glPopMatrix();


    glPushMatrix();
      glTranslatef(200, floorbase, -150+65);
      glScalef(10, 170, 80);
      glutSolidCube(1.0);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    glPushMatrix();
        glTranslatef(0, floorbase-1.9+gateAnimation, 0);

        glPushMatrix();
            glTranslatef(200+10+10, floorbase, 0);
             glRotatef(-90, 1.0, 0.0, 0.0);
            GLUquadric *q;
            q = gluNewQuadric();
            gluCylinder(q, 0, 3, 80, 10,
            5);
            gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(200+10+10, floorbase, 0+10);
            glRotatef(-90, 1.0, 0.0, 0.0);
            gluCylinder(q, 0, 3, 80, 10,
            5);
            gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(200+10+10, floorbase, 0+20);
            glRotatef(-90, 1.0, 0.0, 0.0);
            gluCylinder(q, 0, 3, 80, 10,
            5);
            gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(200+10+10, floorbase, 0+30);
            glRotatef(-90, 1.0, 0.0, 0.0);
            gluCylinder(q, 0, 3, 80, 10,
            5);
            gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(200+10+10, floorbase, 0+40);
            glRotatef(-90, 1.0, 0.0, 0.0);
            gluCylinder(q, 0, 3, 80, 10,
            5);
            gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(200+10+10, floorbase, 0-10);
            glRotatef(-90, 1.0, 0.0, 0.0);
            gluCylinder(q, 0, 3, 80, 10,
            5);
            gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(200+10+10, floorbase, 0-20);
            glRotatef(-90, 1.0, 0.0, 0.0);
            gluCylinder(q, 0, 3, 80, 10,
            5);
            gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(200+10+10, floorbase, 0-30);
            glRotatef(-90, 1.0, 0.0, 0.0);
            gluCylinder(q, 0, 3, 80, 10,
            5);
            gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(200+10+10, floorbase, 0-40);
            glRotatef(-90, 1.0, 0.0, 0.0);
            gluCylinder(q, 0, 3, 80, 10,
            5);
            gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(200+10, floorbase, 0-40);
            glRotatef(-90, 1.0, 0.0, 0.0);
            gluCylinder(q, 0, 3, 80, 10,
            5);
            gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(200+10, floorbase, 40);
            glRotatef(-90, 1.0, 0.0, 0.0);
            gluCylinder(q, 0, 3, 80, 10,
            5);
            gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();
        glPushMatrix();
          glTranslatef(200+10, floorbase+80, 0);
          glScalef(30, 10, 90);
          glutSolidCube(1.0);
        glPopMatrix();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);


    //Create left wall
    glPushMatrix();
      glTranslatef(0, floorbase, -150);
      glScalef(400, 170, 10);
      glutSolidCube(1.0);
    glPopMatrix();

    //Create right wall
    glPushMatrix();
      glTranslatef(0, floorbase, 150);
      glScalef(400, 170, 10);
      glutSolidCube(1.0);
    glPopMatrix();

    //Create bottom wall
    glPushMatrix();
      glTranslatef(-200, floorbase,0);
      glScalef(10, 170, 300);
      glutSolidCube(1.0);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    glColor3f(0.074, 0.219, 0.945);
    //Create pillar 1
    int XlocationPillar1 = 200;
    int YlocationPillar1 = 150;

    glPushMatrix();
      glTranslatef(XlocationPillar1, floorbase, YlocationPillar1);
      glScalef(50.0, 200.0, 50.0);
      glutSolidCube(1.0);
    glPopMatrix();
    //base
    glPushMatrix();
      glTranslatef(XlocationPillar1, floorbase+100, YlocationPillar1);
      glScalef(100, 10, 100);
      glutSolidCube(1.0);
    glPopMatrix();
    glColor3f(0.007, 0.019, 0.352);
    glPushMatrix();
      glTranslatef(XlocationPillar1, floorbase+90, YlocationPillar1);
      glScalef(85, 10, 85);
      glutSolidCube(1.0);
    glPopMatrix();
    glColor3f(0.074, 0.219, 0.945);
    //wall pillars
    glPushMatrix();
      glTranslatef(XlocationPillar1-40, floorbase+115, YlocationPillar1);
      glScalef(20, 40, 100);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(XlocationPillar1+40, floorbase+115, YlocationPillar1);
      glScalef(20, 40, 100);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(XlocationPillar1, floorbase+115, YlocationPillar1-40);
      glScalef(80, 40, 20);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(XlocationPillar1, floorbase+115, YlocationPillar1+40);
      glScalef(80, 40, 20);
      glutSolidCube(1.0);
    glPopMatrix();

    //hedron
    glPushMatrix();
        glTranslatef(XlocationPillar1, floorbase+140, YlocationPillar1+40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar1, floorbase+140, YlocationPillar1-40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar1-40, floorbase+140, YlocationPillar1);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar1-40, floorbase+140, YlocationPillar1-40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar1-40, floorbase+140, YlocationPillar1+40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar1+40, floorbase+140, YlocationPillar1+40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar1+40, floorbase+140, YlocationPillar1);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar1+40, floorbase+140, YlocationPillar1-40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    //
    //Create pillar 2
    int XlocationPillar2 = 200;
    int YlocationPillar2 = -150;
    glPushMatrix();
      glTranslatef(XlocationPillar2, floorbase, YlocationPillar2);
      glScalef(50.0, 200.0, 50.0);
      glutSolidCube(1.0);
    glPopMatrix();
    //base
    glPushMatrix();
      glTranslatef(XlocationPillar2, floorbase+100, YlocationPillar2);
      glScalef(100, 10, 100);
      glutSolidCube(1.0);
    glPopMatrix();

    glColor3f(0.007, 0.019, 0.352);
    glPushMatrix();
      glTranslatef(XlocationPillar2, floorbase+90, YlocationPillar2);
      glScalef(85, 10, 85);
      glutSolidCube(1.0);
    glPopMatrix();

     glColor3f(0.074, 0.219, 0.945);
    //wall pillars
    glPushMatrix();
      glTranslatef(XlocationPillar2-40, floorbase+115, YlocationPillar2);
      glScalef(20, 40, 100);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(XlocationPillar2+40, floorbase+115, YlocationPillar2);
      glScalef(20, 40, 100);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(XlocationPillar2, floorbase+115, YlocationPillar2-40);
      glScalef(80, 40, 20);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(XlocationPillar2, floorbase+115, YlocationPillar2+40);
      glScalef(80, 40, 20);
      glutSolidCube(1.0);
    glPopMatrix();

    //hedron
    glPushMatrix();
        glTranslatef(XlocationPillar2, floorbase+140, YlocationPillar2+40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar2, floorbase+140, YlocationPillar2-40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar2-40, floorbase+140, YlocationPillar2);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar2-40, floorbase+140, YlocationPillar2-40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar2-40, floorbase+140, YlocationPillar2+40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar2+40, floorbase+140, YlocationPillar2+40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar2+40, floorbase+140, YlocationPillar2);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar2+40, floorbase+140, YlocationPillar2-40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    //Create pillar 3
    int XlocationPillar3 = -200;
    int YlocationPillar3 = 150;
    glPushMatrix();
      glTranslatef(XlocationPillar3, floorbase, YlocationPillar3);
      glScalef(50.0, 200.0, 50.0);
      glutSolidCube(1.0);
    glPopMatrix();

    //base
    glPushMatrix();
      glTranslatef(XlocationPillar3, floorbase+100, YlocationPillar3);
      glScalef(100, 10, 100);
      glutSolidCube(1.0);
    glPopMatrix();
    glColor3f(0.007, 0.019, 0.352);
    glPushMatrix();
      glTranslatef(XlocationPillar3, floorbase+90, YlocationPillar3);
      glScalef(85, 10, 85);
      glutSolidCube(1.0);
    glPopMatrix();

     glColor3f(0.074, 0.219, 0.945);
    //wall pillars
    glPushMatrix();
      glTranslatef(XlocationPillar3-40, floorbase+115, YlocationPillar3);
      glScalef(20, 40, 100);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(XlocationPillar3+40, floorbase+115, YlocationPillar3);
      glScalef(20, 40, 100);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(XlocationPillar3, floorbase+115, YlocationPillar3-40);
      glScalef(80, 40, 20);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(XlocationPillar3, floorbase+115, YlocationPillar3+40);
      glScalef(80, 40, 20);
      glutSolidCube(1.0);
    glPopMatrix();

    //hedron
    glPushMatrix();
        glTranslatef(XlocationPillar3, floorbase+140, YlocationPillar3+40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar3, floorbase+140, YlocationPillar3-40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar3-40, floorbase+140, YlocationPillar3);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar3-40, floorbase+140, YlocationPillar3-40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar3-40, floorbase+140, YlocationPillar3+40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar3+40, floorbase+140, YlocationPillar3+40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar3+40, floorbase+140, YlocationPillar3);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar3+40, floorbase+140, YlocationPillar3-40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    //Create pillar 4
    int XlocationPillar4 = -200;
    int YlocationPillar4 = -150;
    glPushMatrix();
      glTranslatef(XlocationPillar4, floorbase, YlocationPillar4);
      glScalef(50.0, 200.0, 50.0);
      glutSolidCube(1.0);
    glPopMatrix();

    //base
    glPushMatrix();
      glTranslatef(XlocationPillar4, floorbase+100, YlocationPillar4);
      glScalef(100, 10, 100);
      glutSolidCube(1.0);
    glPopMatrix();

    glColor3f(0.007, 0.019, 0.352);
    glPushMatrix();
      glTranslatef(XlocationPillar4, floorbase+90, YlocationPillar4);
      glScalef(85, 10, 85);
      glutSolidCube(1.0);
    glPopMatrix();

     glColor3f(0.074, 0.219, 0.945);
    //wall pillars
    glPushMatrix();
      glTranslatef(XlocationPillar4-40, floorbase+115, YlocationPillar4);
      glScalef(20, 40, 100);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(XlocationPillar4+40, floorbase+115, YlocationPillar4);
      glScalef(20, 40, 100);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(XlocationPillar4, floorbase+115, YlocationPillar4-40);
      glScalef(80, 40, 20);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(XlocationPillar4, floorbase+115, YlocationPillar4+40);
      glScalef(80, 40, 20);
      glutSolidCube(1.0);
    glPopMatrix();

    //hedron
    glPushMatrix();
        glTranslatef(XlocationPillar4, floorbase+140, YlocationPillar4+40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar4, floorbase+140, YlocationPillar4-40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar4-40, floorbase+140, YlocationPillar4);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar4-40, floorbase+140, YlocationPillar4-40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar4-40, floorbase+140, YlocationPillar4+40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar4+40, floorbase+140, YlocationPillar4+40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar4+40, floorbase+140, YlocationPillar4);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(XlocationPillar4+40, floorbase+140, YlocationPillar4-40);
        glScalef(20, 20,20);
        glutSolidCube(1.0);
    glPopMatrix();

}


void createRobot(int flashing, float RArmMovement, float LArmMovement, float legMovement, float headMovement, float color1a, float color1b, float color1c, float color2a, float color2b, float color2c)
{
    //head

    glScaled(7,7,7);
    glPushMatrix();
         glTranslated(0,3,0);
         glRotatef(headMovement, 0, 1, 0);
        // angten
        glPushMatrix();
            glTranslated(0, 6.9, 0);
            glRotated(robotMovement, 0,0,1);
//            glRotated(25, 0,0,1);
            int result = flashing/2;
            if(result*2 == flashing ){
            glColor3f(0,0,0);
            }
            else{
            glColor3f(0.858, 0, 0.192);
             }

            glPushMatrix();
                glTranslated(0,0.7,0);
                glScalef(0.05, 0.05,0.05);
                glutSolidSphere(7, 36, 18);
            glPopMatrix();

            glColor3f(color1a, color1b, color1c);
            glPushMatrix();
                glScaled(1,4,1);
                glutSolidCube(0.25);
            glPopMatrix();
 // eyes
        glPopMatrix();
            // left_eye
            glPushMatrix();
                glColor3f(1, 1, 1);
                glPushMatrix();
                    glTranslated(-0.8, 5.8 ,1.2);
                    glScalef(0.08, 0.08,0.08);
                    glutSolidSphere(7, 36, 18);
                glPopMatrix();
                glColor3f(0, 0, 0);
                glPushMatrix();
                    glTranslated(-0.8, 5.8 ,1.53);
                    glScalef(0.04, 0.04,0.04);
                    glutSolidSphere(7, 36, 18);
                glPopMatrix();
            glPopMatrix();
            // right eye
            glPushMatrix();
                glTranslated(1, 2.4 ,0.6);
                glScaled(0.6,0.6,0.6);
                glColor3f(1, 1, 1);
                glPushMatrix();
                    glTranslated(-0.8, 5.8 ,1.2);
                    glScalef(0.08, 0.08,0.08);
                    glutSolidSphere(7, 36, 18);
                glPopMatrix();
                glColor3f(0, 0, 0);
                glPushMatrix();
                    glTranslated(-0.8, 5.8 ,1.53);
                    glScalef(0.04, 0.04,0.04);
                    glutSolidSphere(7, 36, 18);
                glPopMatrix();
            glPopMatrix();

        glColor3f(color1a, color1b, color1c);
        glPushMatrix();
            glTranslated(0,5,0);
            glutSolidCube(3);
        glPopMatrix();
    glPopMatrix();



    //body
    glColor3f(color2a, color2b, color2c);
    glPushMatrix();
        glTranslated(0,6.3,0);
        glScalef(0.13, 0.04,0.13);
        glutSolidSphere(7, 36, 18);
    glPopMatrix();

    glColor3f(color1a, color1b, color1c);
    glPushMatrix();
        glTranslated(0,5,0);
        glScalef(0.9, 0.7,0.7);
        glutSolidCube(3);
    glPopMatrix();

    // left Arm
    glPushMatrix();
        glTranslated(-2,5,0);
        glRotatef(-LArmMovement, 1, 0, 0);
        glRotated(-15, 0, 0, 1);

        glColor3f(color2a, color2b, color2c);
        glPushMatrix();
            glTranslated(0,0.7,0);
            glScalef(0.09, 0.09,0.09);
            glutSolidSphere(7, 36, 18);
        glPopMatrix();

        glColor3f(color1a, color1b, color1c);
        glPushMatrix();
            glTranslated(0,-0.1,0);
            glutSolidCube(1);
        glPopMatrix();

        glColor3f(color2a, color2b, color2c);
        glPushMatrix();
            glTranslated(0,-0.7,0);
            glScalef(0.06, 0.02,0.06);
            glutSolidSphere(7, 36, 18);
        glPopMatrix();

        glColor3f(color1a, color1b, color1c);
        glPushMatrix();
            glTranslated(0,-1.3,0);
            glutSolidCube(1);
        glPopMatrix();

        glColor3f(color2a, color2b, color2c);
        glPushMatrix();
            glTranslated(0,-2.3,0);
            glScalef(0.08, 0.08,0.08);
            glutSolidSphere(7, 36, 18);
        glPopMatrix();

    glPopMatrix();

    // right Arm
    glPushMatrix();
        glTranslated(2,5,0);
        glRotatef(RArmMovement, 1, 0, 0);
        glRotated(15, 0, 0, 1);


        glColor3f(color2a, color2b, color2c);
        glPushMatrix();
            glTranslated(0,0.7,0);
            glScalef(0.09, 0.09,0.09);
            glutSolidSphere(7, 36, 18);
        glPopMatrix();

        glColor3f(color1a, color1b, color1c);
        glPushMatrix();
            glTranslated(0,-0.1,0);
            glutSolidCube(1);
        glPopMatrix();

        glColor3f(color2a, color2b, color2c);
        glPushMatrix();
            glTranslated(0,-0.7,0);
            glScalef(0.06, 0.02,0.06);
            glutSolidSphere(7, 36, 18);
        glPopMatrix();

        glColor3f(color1a, color1b, color1c);
        glPushMatrix();
            glTranslated(0,-1.3,0);
            glutSolidCube(1);
        glPopMatrix();

        glColor3f(color2a, color2b, color2c);
        glPushMatrix();
            glTranslated(0,-2.3,0);
            glScalef(0.08, 0.08,0.08);
            glutSolidSphere(7, 36, 18);
        glPopMatrix();

    glPopMatrix();

    // left leg

    glPushMatrix();
        glTranslated(-0.6, 3.8, 0);
        glRotatef(-legMovement, 1, 0, 0);
        glColor3f(color2a, color2b, color2c);
        glPushMatrix();
            glTranslated(0,0,0);
            glScalef(0.08, 0.02,0.08);
            glutSolidSphere(7, 36, 18);
        glPopMatrix();

        glColor3f(color1a, color1b, color1c);
        glPushMatrix();
            glTranslated(0,-1.6,0);
            glScaled(1,3,1);
            glutSolidCube(1);
        glPopMatrix();

    glPopMatrix();

    // right leg

    glPushMatrix();
        glTranslated(0.8, 3.8, 0);
        glRotatef(legMovement, 1, 0, 0);
        glColor3f(color2a, color2b, color2c);
        glPushMatrix();
            glTranslated(0,0,0);
            glScalef(0.08, 0.02,0.08);
            glutSolidSphere(7, 36, 18);
        glPopMatrix();

        glColor3f(color1a, color1b, color1c);
        glPushMatrix();
            glTranslated(0,-1.6,0);
            glScaled(1,3,1);
            glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
}
void createRobot2(float theta)
{


    glScalef(7, 7, 7);

    glPushMatrix();
//        glRotatef(theta, 1, 0, 0);
        glColor3f(0.925, 0.501, 0.074);//Head
        glPushMatrix();
            glRotatef(0, 0, 1, 0);
            glPushMatrix();
              glTranslatef(0, 8.7, 0);
              glutSolidSphere(1, 36, 18);
            glPopMatrix();

        glPopMatrix();

        glColor3f(1., 1., 1.);			//Torso
        glPushMatrix();
            glPushMatrix();
              glTranslatef(0, 5.5, 0);
              glScalef(3, 3, 1.4);
              glutSolidCube(1);
            glPopMatrix();

            glPushMatrix();
               glColor3f(1., 0, 0.);
               glRotated(20, 0, 0, 1);
               glTranslatef(1.8, 5, 1);
               glutSolidTorus(0.5, 1.25, 5, 5);
            glPopMatrix();
        glPopMatrix();

                    //Right arm

        glPushMatrix();
          glTranslatef(-2, 6.5, 0);
          glRotatef(-145, 1, 0, 0);
          glTranslatef(2, -6.5, 0);
          glPushMatrix();
              glTranslatef(-2, 4.5, 0);
              glScalef(0.5, 2.5, 0.5);
              glColor3f(0.529, 0.074, 0.925);
              glutSolidSphere(1, 18, 18);
          glPopMatrix();

          glPushMatrix();
            glColor3f(0.5, 1, 0.5);
            glTranslatef(-2, 1.2, 0);
            glRotated(theta, 0, 1, 0 );
            glTranslatef(2, -1.2, 0);
            glRotated(theta, 0, 1, 0 );

            glRotated(180, 1, 0, 0 );
            glutSolidTeapot(1);
          glPopMatrix();

        glPopMatrix();




        glColor3f(0.529, 0.074, 0.925);			//Left arm
        glPushMatrix();
          glTranslatef(2, 6.5, 0);
          glRotatef(0, 1, 0, 0);
          glTranslatef(-2, -6.5, 0);
          glTranslatef(2, 4.5, 0);
          glScalef(0.5, 2.5, 0.5);
          glutSolidSphere(1, 18, 18);
        glPopMatrix();

    glPopMatrix();


    glColor3f(0.074, 0.925, 0.866);			//Right leg
    glPushMatrix();

      glTranslatef(-0.8, 4, 0);
      glRotatef(-0, 1, 0, 0);
      glTranslatef(0.8, -4, 0);

      glPushMatrix();
          glTranslatef(-0.8, 3.2, 0);
          glScalef(1, 2.4, 1);
          glutSolidCube(1);
      glPopMatrix();

      glPushMatrix();
        glTranslatef(-0.8, 1.2, 0);
        glRotated(-90, 0, 1,0);
        glutSolidTeapot(1);
      glPopMatrix();

    glPopMatrix();

    glColor3f(0.074, 0.925, 0.866);
    glPushMatrix();                 //left leg
        glTranslatef(0.8, 4, 0);
        glRotatef(0, 1, 0, 0);
        glTranslatef(-0.8, -4, 0);

        glPushMatrix();
            glTranslatef(0.8, 3.2, 0);
            glScalef(1, 2.4, 1);
            glutSolidCube(1);
        glPopMatrix();

        glPushMatrix();
          glTranslatef(0.8, 1.2, 0);
          glRotated(-90, 0, 1,0);
          glutSolidTeapot(1);
        glPopMatrix();

    glPopMatrix();


}
void createCannonBullet(){
    glPushMatrix();
    glTranslatef(250, floorbase, -100);
    glutSolidSphere(5, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(250, floorbase, -110);
    glutSolidSphere(5, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(250, floorbase, -120);
    glutSolidSphere(5, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(250, floorbase, -90);
    glutSolidSphere(5, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(250, floorbase, -80);
    glutSolidSphere(5, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(260, floorbase, -100);
    glutSolidSphere(5, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(260, floorbase, -110);
    glutSolidSphere(5, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(260, floorbase, -120);
    glutSolidSphere(5, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(260, floorbase, -90);
    glutSolidSphere(5, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(260, floorbase, -80);
    glutSolidSphere(5, 36, 18);
    glPopMatrix();

}
void createSpaceShip(int flashing, float angle1, int lightOff)
{

    glRotatef(robotAngleMove, 0.0, 1.0, 0.0);
    if(lightOff == 0){
        glTranslatef( 0, 10+spaceShipHeight, 0);
        glColor3f(1, 1, 1);
    }

    glTranslatef( 0, -5+spaceShipHeight, 0);
//    glRotatef(robotAngleMove, 0.0, 1.0, 0.0);


    glPushMatrix(); // head
    glTranslated(1,5,0);
        if(lightOff == 0){

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texId[6]);

        glPushMatrix();
            glScalef(0.7, 0.7,0.7);
            gluSphere (q, 5, 36, 18);
//            glutSolidSphere(5, 36, 18);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        glColor4f(1, 1, 1, 1);

        }
        else{
            glPushMatrix();
                glScalef(0.7, 0.7,0.7);
                glutSolidSphere(5, 36, 18);
            glPopMatrix();

        }

        glDisable(GL_TEXTURE_2D);
        if(lightOff == 0){
        glColor3f(1, 1, 1);
        }
        glPushMatrix();
            glTranslated(0,3.5,0);
            glScalef(0.1, 0.02,0.1);
            glutSolidSphere(7, 36, 18);
        glPopMatrix();

        glPushMatrix();
            glTranslated(0,3.7,0);
            glScalef(0.05, 0.02,0.05);
            glutSolidSphere(7, 36, 18);
        glPopMatrix();

        glPushMatrix();
        glTranslated(0,3.7,0);
        glRotatef(-80, 1.0, 0.0, 0.0);

        glPushMatrix();
            glScalef(0.05, 0.05,0.05);
            GLUquadric *q;
            q = gluNewQuadric();
            gluCylinder(q, 3, 0, 40, 10,
            5);
            gluQuadricDrawStyle(q, GLU_FILL);
            int result = flashing/2;
        if(lightOff == 0){



        if(result*2 == flashing ){
        glColor3f(1,1,1);
        }
        else{
        glColor3f(0.901, 0.058, 0.203);
         }
        }
        glPopMatrix();
            glPushMatrix();
            glTranslated(0,0,2);
            glScalef(0.05, 0.05,0.05);
            glutSolidSphere(5, 36, 18);
            glPopMatrix();

         glPopMatrix();

    glPopMatrix();
    if(lightOff == 0){
    glColor3f(0.278, 0.631, 0.882);
    }

    glPushMatrix();

    glTranslated(1,2,0);

    glPushMatrix();
        glScalef(1, 0.15,1);
        glutSolidSphere(5, 36, 18);
    glPopMatrix();
    if(lightOff == 0){
    if(result*2 == flashing ){
           glColor3f(1,1,1);
    }else{
    glColor3f(0.023, 0.435, 0.105);
     }
}
    glPushMatrix();
        glRotatef(angle1, 0.0, 1.0, 0.0);
        glTranslated(0,0.7,4);
        glScalef(0.05, 0.05,0.05);
        glutSolidSphere(5, 36, 18);
    glPopMatrix();
    if(lightOff == 0){

    if(result*2 == flashing ){
           glColor3f(1,1,1);
    }else{
    glColor3f(0.682, 0, 0.819);
     }
}
    glPushMatrix();
        glRotatef(angle1 + 90, 0.0, 1.0, 0.0);
        glTranslated(0,0.7,4);
        glScalef(0.05, 0.05,0.05);
        glutSolidSphere(5, 36, 18);
    glPopMatrix();
if(lightOff == 0){
    if(result*2 == flashing ){
           glColor3f(1,1,1);
    }else{
    glColor3f(0.007, 0.164, 0.611);
     }
}
    glPushMatrix();
        glRotatef(angle1 + 90*2, 0.0, 1.0, 0.0);
        glTranslated(0,0.7,4);
        glScalef(0.05, 0.05,0.05);
        glutSolidSphere(5, 36, 18);
    glPopMatrix();

if(lightOff == 0){
    if(result*2 == flashing ){
           glColor3f(1,1,1);
    }else{
    glColor3f(0.945, 0.521, 0.133);
     }
}

    glPushMatrix();
        glRotatef(angle1 + 90*3, 0.0, 1.0, 0.0);
        glTranslated(0,0.7,4);
        glScalef(0.05, 0.05,0.05);
        glutSolidSphere(5, 36, 18);
    glPopMatrix();



    glPopMatrix();

    // legs
    if(lightOff == 0){
    glColor3f(1, 1, 1);
    }
    glPushMatrix();
        glTranslated(1,2,0);
        glRotated(40,1,0,0);

        glPushMatrix();
        glScalef(0.05, 0.05,0.05);
        glRotatef(90, 1.0, 0.0, 0.0);
        q = gluNewQuadric();
        gluCylinder(q, 3, 0, 80, 10,
        5);
        gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();
        if(lightOff == 0){
        glColor3f(0.278, 0.631, 0.882);
        }
        glPushMatrix();
        glTranslated(0,-3.7,0);
        glScalef(0.05, 0.05,0.05);
        glutSolidSphere(5, 36, 18);
        glPopMatrix();

    glPopMatrix();
    if(lightOff == 0){
    glColor3f(1, 1, 1);
    }
    glPushMatrix();
        glTranslated(1,2,0);
        glRotated(-40,1,0,0);

        glPushMatrix();
        glScalef(0.05, 0.05,0.05);
        glRotatef(90, 1.0, 0.0, 0.0);
        q = gluNewQuadric();
        gluCylinder(q, 3, 0, 80, 10,
        5);
        gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();
        if(lightOff == 0){
        glColor3f(0.278, 0.631, 0.882);
        }
        glPushMatrix();
        glTranslated(0,-3.7,0);
        glScalef(0.05, 0.05,0.05);
        glutSolidSphere(5, 36, 18);
        glPopMatrix();

    glPopMatrix();
    if(lightOff == 0){
    glColor3f(1, 1, 1);
    }
    glPushMatrix();
        glTranslated(1,2,0);
        glRotated(-40,0,0,1);

        glPushMatrix();
        glScalef(0.05, 0.05,0.05);
        glRotatef(90, 1.0, 0.0, 0.0);
        q = gluNewQuadric();
        gluCylinder(q, 3, 0, 80, 10,
        5);
        gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();
        if(lightOff == 0){
        glColor3f(0.278, 0.631, 0.882);
        }
        glPushMatrix();
        glTranslated(0,-3.7,0);
        glScalef(0.05, 0.05,0.05);
        glutSolidSphere(5, 36, 18);
        glPopMatrix();

    glPopMatrix();
    if(lightOff == 0){
    glColor3f(1, 1, 1);
    }
    glPushMatrix();
        glTranslated(1,2,0);
        glRotated(40,0,0,1);

        glPushMatrix();
        glScalef(0.05, 0.05,0.05);
        glRotatef(90, 1.0, 0.0, 0.0);
        q = gluNewQuadric();
        gluCylinder(q, 3, 0, 80, 10,
        5);
        gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();
        if(lightOff == 0){
        glColor3f(0.278, 0.631, 0.882);
        }
        glPushMatrix();
        glTranslated(0,-3.7,0);
        glScalef(0.05, 0.05,0.05);
        glutSolidSphere(5, 36, 18);
        glPopMatrix();

    glPopMatrix();

}


void skybox(){
    glEnable(GL_TEXTURE_2D);

  ////////////////////// LEFT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[0]);
 // glColor3f(1, 0, 0);    //<<<<<<<<<<<< Remove the statements that assign color to the sides of the cube
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);     glVertex3f(-1000,  0, 1000);
  glTexCoord2f(1, 0.);     glVertex3f(-1000, 0., -1000);
  glTexCoord2f(1, 1);     glVertex3f(-1000, 1000., -1000);
  glTexCoord2f(0, 1);     glVertex3f(-1000, 1000, 1000);
  glEnd();

  ////////////////////// FRONT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[1]);
  glColor3f(0, 1, 0);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);     glVertex3f(-1000,  0, -1000);
  glTexCoord2f(1, 0.);     glVertex3f(1000, 0., -1000);
  glTexCoord2f(1, 1);     glVertex3f(1000, 1000, -1000);
  glTexCoord2f(0., 1);     glVertex3f(-1000,  1000, -1000);
  glEnd();

 ////////////////////// RIGHT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[2]);
  glColor3f(0, 0, 1);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);     glVertex3f(1000,  0, -1000);
  glTexCoord2f(1, 0.);     glVertex3f(1000, 0, 1000);
  glTexCoord2f(1, 1);     glVertex3f(1000, 1000,  1000);
  glTexCoord2f(0., 1);     glVertex3f(1000,  1000,  -1000);
  glEnd();


  ////////////////////// REAR WALL ////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[3]);
  glColor3f(1, 1, 0);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);     glVertex3f( 1000, 0, 1000);
  glTexCoord2f(1, 0.);     glVertex3f(-1000, 0,  1000);
  glTexCoord2f(1, 1);     glVertex3f(-1000, 1000,  1000);
  glTexCoord2f(0., 1);     glVertex3f( 1000, 1000, 1000);
  glEnd();

  /////////////////////// TOP //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[4]);
  glColor3f(1, 0, 1);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);     glVertex3f(-1000, 1000, -1000);
  glTexCoord2f(1, 0.);     glVertex3f(1000, 1000,  -1000);
  glTexCoord2f(1, 1);     glVertex3f(1000, 1000,  1000);
  glTexCoord2f(0., 1);     glVertex3f(-1000, 1000, 1000);
  glEnd();

//  /////////////////////// FLOOR //////////////////////////
//  glBindTexture(GL_TEXTURE_2D, texId[5]);
//  glColor3f(0, 1, 1);
//  glBegin(GL_QUADS);
//  glTexCoord2f(0., 0.);     glVertex3f(-1000, 0., 1000);
//  glTexCoord2f(1, 0.);     glVertex3f(1000, 0.,  1000);
//  glTexCoord2f(1, 1);     glVertex3f(1000, 0., -1000);
//  glTexCoord2f(0., 1);     glVertex3f(-1000, 0., -1000);
//  glEnd();

}

//--Display: ----------------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.

void display()
{


//    glScaled(0.1, 0.1,0.1);

    float lx = 500.0;
    float ly = 400.0;
    float lz = -200.0;
    float lpos[4] = {lx, ly, lz, 1.0};  //light's position

    float lgt_pos1[] = {50, 100, 200, 1};
    float lgt_pos2[] = {20+spaceShipHeight*10 , 20+spaceShipHeight*10, 0, 1};

    float spot_dir[] = {-1, -1, 0};

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    float shadowMat[16] = { ly,0,0,0, -lx,0,-lz,-1,
    0,0,ly,0, 0,0,0,ly };

    look_x = eye_x + 100*sin(angle*toR);
    look_z = eye_z - 100*cos(angle*toR);
    look_y = cam_hgt - 100*tan(upAngle*toR);

    if(camChange == 1){
        cout << spaceShipHeight<<endl;
        if(spaceShipHeight > 60){
            camChange = 0;
        }
        if(shipLiftOff == 0 ){
            glRotatef(-robotAngleMove, 0.0, 1.0, 0.0);
            gluLookAt(7, 100+spaceShipHeight, 0,  200, 100+spaceShipHeight, 0,   0, 1, 0);
        }
        else{
            gluLookAt(7, 120+spaceShipHeight*10, 0,  0, 0, 0,   0, 1, 0);
        }

//        gluLookAt(7, 50+spaceShipHeight, 0,  0, 0, 0,   0, 1, 0);
    }else{
        gluLookAt(eye_x, cam_hgt, eye_z,  look_x, look_y, look_z,   0, 1, 0);
    }


    glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position


    glLightfv(GL_LIGHT2, GL_POSITION, lgt_pos2);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_dir);


//    glRotatef(angle, 0.0, 1.0, 0.0);
//    glRotatef(cam_up, 0.0, 0, 1.0);	//rotate the whole scene



    //create the castle

    //****************************************************************
    //--start here
    //create cannon with shadow
    glDisable(GL_LIGHTING);
    glPushMatrix();
        glTranslatef( 0, 1, 0);
        glMultMatrixf(shadowMat);
        glColor4f(0.2, 0.2, 0.2, 1.0);
        createCannon(270, 0.5);
    glPopMatrix();
        glEnable(GL_LIGHTING);

        glPushMatrix();
            glColor3f(0.929, 0.027, 0.137);
            createCannon(270, 0.5);
        glPopMatrix();

    createCannonBullet();
    createPillars();

     // create SpaceShip with shadow


    glDisable(GL_LIGHTING);
    glPushMatrix();
        glTranslatef( -100, 1, 30);
        glMultMatrixf(shadowMat);
        glColor4f(0.2, 0.2, 0.2, 1.0);
        glScaled(10,10,10);
        createSpaceShip(flashing, 0, 1);
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glPushMatrix();
    glScaled(10,10,10);
    createSpaceShip(flashing, 0, 0);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);



    // create robot
    glPushMatrix();
        glRotatef(robotAngleMove, 0.0, 1.0, 0.0);
        glTranslatef(320, floorbase, 0);
        glPushMatrix();
        glRotatef(robotMovement, 0, 1, 0);

        glLightfv(GL_LIGHT1, GL_POSITION, lgt_pos1);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);

        glPopMatrix();
        createRobot(flashing, robotMovement, robotMovement, robotMovement, robotMovement, 0.964, 0.686, 0.137, 0.427, 0.290, 0.011);
    glPopMatrix();


    glPushMatrix();
        glTranslatef(0 + robotMovement , floorbase, 250);
        glPushMatrix();
        createCannon(45, 0.7);
        glPopMatrix();
        glPushMatrix();
        glRotatef(90, 0.0, 1.0, 0.0);
        createRobot(flashing, -50, 50, robotMovement, 0, 0.372, 0.964, 0.274, 0.086, 0.435, 0.023);
        glPopMatrix();
    glPopMatrix();


    glPushMatrix();
        glTranslatef(0 + robotMovement , floorbase, -250);
        glPushMatrix();
        createCannon(45, 0.7);
        glPopMatrix();
        glPushMatrix();
        glRotatef(90, 0.0, 1.0, 0.0);
        createRobot(flashing, -50, 50, robotMovement, 0, 0.286, 0.505, 0.952, 0.058, 0.211, 0.521);
        glPopMatrix();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(200 , floorbase + 120 , 150);
    glRotatef(90, 0.0, 1.0, 0.0);
    createRobot2(teaPort);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(200 , floorbase + 120 , -150);
    glRotatef(90, 0.0, 1.0, 0.0);
    createRobot2(teaPort);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-200 , floorbase + 120 , 150);
    glRotatef(-90, 0.0, 1.0, 0.0);
    createRobot2(teaPort);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-200 , floorbase + 120 , -150);
    glRotatef(-90, 0.0, 1.0, 0.0);
    createRobot2(teaPort);
    glPopMatrix();

//    glColor4f(0, 0, 0, 1);

    skybox();
    glDisable(GL_TEXTURE_2D);

    drawFloor();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[8]);
    glPushMatrix();
    glTranslated(-130, floorbase, 0);
    glScaled(3,3,3);
    drawVast();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    fire_generator();
    draw_fire();
    Update();

    glutSwapBuffers();
}
int countUp = 1;
int direction = 1;

void myTimer(int value)
{
    if( boo == 1){
        theta += 4;
        alpha += 4;
    }

    if( gateAnimation >= 80 ){
        direction = -1 ;
    }
    if( gateAnimation <= 0 ){
        direction = 1;
    }
    gateAnimation += direction;

    if(robotMovement > 20){
        countUp = -1;
    }

    if (robotMovement < -20){
        countUp = 1;
    }
    if(shipLiftOff == 1){
        spaceShipHeight += 0.5;
    }
    if(spaceShipHeight > 50){
        glDisable(GL_LIGHT2);
    }

    robotMovement += countUp;
    robotAngleMove -= 0.5;
    teaPort += 2;
    flashing += 1;
    glutPostRedisplay();
    glutTimerFunc(50, myTimer, 0);

}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1, 1, 1,1};
    q =  gluNewQuadric ( );

    gluQuadricDrawStyle (q, GLU_FILL );
    gluQuadricNormals	(q, GLU_SMOOTH );
    gluQuadricTexture (q, GL_TRUE);

//    loadTexture();
    loadGLTextures();
//    glEnable(GL_TEXTURE_2D);

    loadMeshFile("Cannon.off");				//Specify mesh file name here
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour

    glEnable(GL_LIGHTING);					//Enable OpenGL states
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);


    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,70);

    glLightfv(GL_LIGHT2, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT2, GL_SPECULAR, white);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT,0);


    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    gluPerspective(88, 1, 100, 5000);  //The camera view volume
}

//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene
void special(int key, int x, int y)
{

    if(key == GLUT_KEY_LEFT) angle--;
    else if(key == GLUT_KEY_RIGHT) angle++;

    else if(key == GLUT_KEY_DOWN)
    {  //Move backward
        eye_x -= 5*sin(angle*toR);
        eye_z += 5*cos(angle*toR);
    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        eye_x += 5*sin(angle*toR);
        eye_z -= 5*cos(angle*toR);
    }
    else if(key == GLUT_KEY_HOME)
    { //Move forward
       if(camChange == 0)
           camChange = 1;
       else{
           camChange = 0;
       }
    }

    look_x = eye_x + 100*sin(angle*toR);
    look_z = eye_z - 100*cos(angle*toR);
    glutPostRedisplay();
    cout << eye_x << endl;
    //collision detection
    if(eye_x < -830){
        eye_x = -830;
    }
    if(eye_x > 830){
        eye_x = 830;
    }
    if(eye_z < -830){
        eye_z = -830;
    }
    if(eye_z > 830){
        eye_z = 830;
    }
}

void keyboard(unsigned char key, int, int){
    if(key == 'c'){
        boo = 1;
    }
    if(key == 'w'){
        upAngle += 2;
    }
    if(key == 'q'){
        upAngle -= 2;
    }
    if(key == 's'){
        shipLiftOff = 1;
    }




}

//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE| GLUT_DEPTH);
   glutInitWindowSize (840, 840);
   glutInitWindowPosition (10, 10);
   glutTimerFunc(50, myTimer, 0);

   glutCreateWindow ("Alien Invasion!");
   initialize();

   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutKeyboardFunc(keyboard);
   glutIdleFunc(idle);
   glutMainLoop();
   return 0;
}
