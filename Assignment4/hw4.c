/*
 *  Homework #4
 *
 *  Demonstrates how to draw objects in 3D.
 *  orthogonal & prespective projections & first person view
 *
 *  Key bindings:
 *  1/2/3        Toggle between orthogonal perspective, and first person view
 *  r          Toggle axes
 *  arrows     Change view angle
 *  0          Reset view angle
 *  First Person View
 *  w          Forward
 *  a          Left
 *  s          Backward
 *  d          Right
 *  q          Rotate Left
 *  e          Rotate Right
 *  ESC        Exit
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int axes=0;       //  Display axes
int mode=1;       //  Projection mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int zh=0;
double dim=2.5;
double asp=1;     //  Aspect ratio
double cx=0;
double cy=1;
double cz=4;
double view=1000;
double viewlr=-90;

// angle of rotation for the camera direction
float angle=0.0;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float x=0.0f,z=5.0f;


//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
    char    buf[LEN];
    char*   ch=buf;
    va_list args;
    //  Turn the parameters into a character string
    va_start(args,format);
    vsnprintf(buf,LEN,format,args);
    va_end(args);
    //  Display the characters one at a time at the current raster position
    while (*ch)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}
// glColor3f(((double) rand()/(RAND_MAX)),((double) rand()/(RAND_MAX)),((double) rand()/(RAND_MAX)));

/*
 *  Draw a house
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void house(double x,double y,double z,
                  double dx,double dy,double dz,
                  double th)
{
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    //  House
    glBegin(GL_QUADS);
    //  Front
    glColor3f(1,0,0);
    glVertex3f(-1,-1, 1);
    glVertex3f(+1,-1, 1);
    glVertex3f(+1,+1, 1);
    glVertex3f(-1,+1, 1);
    // Door
    glColor3f(1,1,0);
    glVertex3f(-0.8,-1, 1.01);
    glVertex3f(-0.6,-1, 1.01);
    glVertex3f(-0.6,-0.2, 1.01);
    glVertex3f(-0.8,-0.2, 1.01);
    //  Back
    glColor3f(0,1,0);
    glVertex3f(+1,-1,-1);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,+1,-1);
    glVertex3f(+1,+1,-1);
    //  Right
    glColor3f(0,0,1);
    glVertex3f(+1,-1,+1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,+1,-1);
    glVertex3f(+1,+1,+1);
    //  Left
    glColor3f(1,1,0);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,-1,+1);
    glVertex3f(-1,+1,+1);
    glVertex3f(-1,+1,-1);
    //  Top
    glColor3f(1,0,1);
    glVertex3f(-1,+1,+1);
    glVertex3f(+1,+1,+1);
    glVertex3f(+1,+1,-1);
    glVertex3f(-1,+1,-1);
    //  Bottom
    glColor3f(1,0,1);
    glVertex3f(-1,-1,-1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,-1,+1);
    glVertex3f(-1,-1,+1);
    // Chimney Bottom
    glColor3f(1,0,1);
    glVertex3f(-0.4,-1,-0.4);
    glVertex3f(-0.4,-1,-0.6);
    glVertex3f(-0.6,-1,-0.6);
    glVertex3f(-0.6,-1,-0.4);
    // Chimney Top
    glColor3f(1,0,1);
    glVertex3f(-0.4,2.2,-0.4);
    glVertex3f(-0.4,2.2,-0.6);
    glVertex3f(-0.6,2.2,-0.6);
    glVertex3f(-0.6,2.2,-0.4);
    // Chimney Left
    glColor3f(1,0,1);
    glVertex3f(-0.4,2.2,-0.4);
    glVertex3f(-0.4,-1,-0.4);
    glVertex3f(-0.4,-1,-0.6);
    glVertex3f(-0.4,2.2,-0.6);
    // Chimney Front
    glColor3f(1,0,1);
    glVertex3f(-0.4,2.2,-0.6);
    glVertex3f(-0.4,-1,-0.6);
    glVertex3f(-0.6,-1,-0.6);
    glVertex3f(-0.6,2.2,-0.6);
    // Chimney Right
    glColor3f(1,0,1);
    glVertex3f(-0.6,2.2,-0.6);
    glVertex3f(-0.6,-1,-0.6);
    glVertex3f(-0.6,-1,-0.4);
    glVertex3f(-0.6,2.2,-0.4);
    // Chimney Back
    glColor3f(1,0,1);
    glVertex3f(-0.6,2.2,-0.4);
    glVertex3f(-0.6,-1,-0.4);
    glVertex3f(-0.4,-1,-0.4);
    glVertex3f(-0.4,2.2,-0.4);
    // Roof Side 1
    glColor3f(0,1,0);
    glVertex3f(-1,+2,0);
    glVertex3f(-1,+1,+1);
    glVertex3f(+1,+1,+1);
    glVertex3f(+1,+2,0);
    // Roof Side 2
    glColor3f(0,0,1);
    glVertex3f(-1,+2,0);
    glVertex3f(-1,+1,-1);
    glVertex3f(+1,+1,-1);
    glVertex3f(+1,+2,0);
    glEnd();
    //  Roof
    glBegin(GL_TRIANGLES);
    //  Roof Left
    glColor3f(1,1,0);
    glVertex3f(+1,+1,+1);
    glVertex3f(+1,+1,-1);
    glVertex3f(+1,+2,0);
    //  Roof Right
    glColor3f(1,0,1);
    glVertex3f(-1,+1,+1);
    glVertex3f(-1,+1,-1);
    glVertex3f(-1,+2,0);
    //  End
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

static void car(double x,double y,double z,
                double dx,double dy,double dz,
                double th)
{
    double x_1;
    double y_1;
    double r = 0.5;
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    //  Body
    glBegin(GL_QUADS);
    //  Front
    glColor3f(1,0,0);
    glVertex3f(-1,-1, 1);
    glVertex3f(+1,-1, 1);
    glVertex3f(+1,+1, 1);
    glVertex3f(-1,+1, 1);
    //  Back
    glColor3f(0,1,0);
    glVertex3f(+1,-1,-1);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,+1,-1);
    glVertex3f(+1,+1,-1);
    //  Right
    glColor3f(0,0,1);
    glVertex3f(+1,-1,+1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,+1,-1);
    glVertex3f(+1,+1,+1);
    //  Left
    glColor3f(1,1,0);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,-1,+1);
    glVertex3f(-1,+1,+1);
    glVertex3f(-1,+1,-1);
    //  Top
    glColor3f(1,0,1);
    glVertex3f(-1,+1,+1);
    glVertex3f(+1,+1,+1);
    glVertex3f(+1,+1,-1);
    glVertex3f(-1,+1,-1);
    //  Bottom
    glColor3f(1,0,1);
    glVertex3f(-1,-1,-1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,-1,+1);
    glVertex3f(-1,-1,+1);
    //  End
    glEnd();
    
    //Wheels
    glBegin(GL_POLYGON);
    glColor3f(1,0,1);
    for(double i=0;i<=3.1416*2;i+=0.05)
    {
        x_1=r*cos(i);
        y_1=r*sin(i);
        glVertex3f(x_1+1,y_1-1,1.01);
    }
    glEnd();
    
    glBegin(GL_POLYGON);
    glColor3f(1,0,1);
    for(double i=0;i<=3.1416*2;i+=0.05)
    {
        x_1=r*cos(i);
        y_1=r*sin(i);
        glVertex3f(x_1-1,y_1-1,1.01);
    }
    glEnd();
    
    glBegin(GL_POLYGON);
    glColor3f(1,0,1);
    for(double i=3.1416*2;i>=0;i-=0.05)
    {
        x_1=r*cos(i);
        y_1=r*sin(i);
        glVertex3f(x_1+1,y_1-1,-1.01);
    }
    glEnd();
    
    glBegin(GL_POLYGON);
    glColor3f(1,0,1);
    for(double i=3.1416*2;i>=0;i-=0.05)
    {
        x_1=r*cos(i);
        y_1=r*sin(i);
        glVertex3f(x_1-1,y_1-1,-1.01);
    }
    glEnd();
    
    //  Undo transformations
    glPopMatrix();
}

/*
 *  Set projection
 */
static void Project()
{
    //  Tell OpenGL we want to manipulate the projection matrix
    glMatrixMode(GL_PROJECTION);
    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective transformation
    if (mode == 2 || mode == 3)
        gluPerspective(fov,asp,dim/4,4*dim);
    //  Orthogonal projection
    else if (mode == 1)
        glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
    //  Switch to manipulating the model matrix
    glMatrixMode(GL_MODELVIEW);
    //  Undo previous transformations
    glLoadIdentity();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
    const double len=1.5;  //  Length of axes
    //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //  Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);
    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective - set eye position
    if (mode == 2)
    {
        double Ex = -2*dim*Sin(th)*Cos(ph);
        double Ey = +2*dim        *Sin(ph);
        double Ez = +2*dim*Cos(th)*Cos(ph);
        gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
        Print("Angle=%d,%d FOV=%d Projection=%s",th,ph,fov,"Perpective");
    }
    //  Orthogonal - set world orientation
    else if (mode == 1)
    {
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);
        Print("Angle=%d,%d FOV=%d Projection=%s",th,ph,fov,"Orthogonal");
    }
    //  Orthogonal
    else if (mode == 3)
    {
        gluLookAt(cx,cy,cz,cx+view*Cos(viewlr),cy,cz+view*Sin(viewlr),0,1,0);
        Print("Angle=%d,%d FOV=%d Projection=%s",th,ph,fov,"First Person View");
    }
    /*
    // Draw ground
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f,  100.0f);
    glVertex3f( 100.0f, 0.0f,  100.0f);
    glVertex3f( 100.0f, 0.0f, -100.0f);
    glEnd();
    */
    house(-2,0,1 , 0.3,0.35,0.35, 90);
    house(-1,0,1 , 0.3,0.35,0.35, 90);
    house(-2,0,0 , 0.3,0.3,0.4, 0);
    house(-1,0,0 , 0.3,0.3,0.4, 0);
    house(-2,0,-1 , 0.3,0.25,0.45, -90);
    house(-1,0,-1 , 0.3,0.25,0.45, -90);
    
    house(2,0,1 , 0.3,0.35,0.35, 90);
    house(1,0,1 , 0.3,0.35,0.35, 90);
    house(2,0,0 , 0.3,0.3,0.4, 0);
    house(1,0,0 , 0.3,0.3,0.4, 0);
    house(2,0,-1 , 0.3,0.25,0.45, -90);
    house(1,0,-1 , 0.3,0.25,0.45, -90);
    
    car(0.2,0,-2+zh/100 , 0.1,0.1,0.1, 90);
    car(-0.2,0,2-zh/100 , 0.2,0.1,0.1, 90);
    
    //  White
    glColor3f(1,1,1);
    //  Draw axes
    if (axes)
    {
        glBegin(GL_LINES);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(len,0.0,0.0);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(0.0,len,0.0);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(0.0,0.0,len);
        glEnd();
        //  Label axes
        glRasterPos3d(len,0.0,0.0);
        Print("X");
        glRasterPos3d(0.0,len,0.0);
        Print("Y");
        glRasterPos3d(0.0,0.0,len);
        Print("Z");
    }
    //  Five pixels from the lower left corner of the window
    glWindowPos2i(5,5);
    //Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
    //  Render the scene and make it visible
    glFlush();
    glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
        th += 5;
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
        th -= 5;
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
        ph += 5;
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
        ph -= 5;
    //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_UP)
        dim += 0.1;
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
        dim -= 0.1;
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    //  Update projection
    Project();
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
    //  Exit on ESC
    if (ch == 27)
        exit(0);
    //  Reset view angle
    else if (ch == '0')
        th = ph = 0;
    //  Toggle axes
    else if (ch == 'r' || ch == 'R')
        axes = 1-axes;
    
    else if (ch == '1')
        mode=1;
    else if (ch == '2')
        mode=2;
    else if (ch == '3')
        mode=3;
    
    else if (ch == 'w' || ch == 'W'){
        if(mode==3){
            cx += 0.3*Cos(viewlr);
            cz += 0.3*Sin(viewlr);
        }
    }
    else if (ch == 'a' || ch == 'A'){
        if(mode==3){
            cx += 0.3*Sin(viewlr);
            cz -= 0.3*Cos(viewlr);
        }
    }
    else if (ch == 's' || ch == 'S'){
        if(mode==3){
            cx -= 0.3*Cos(viewlr);
            cz -= 0.3*Sin(viewlr);
        }
    }
    else if (ch == 'd' || ch == 'D'){
        if(mode==3){
            cx -= 0.3*Sin(viewlr);
            cz += 0.3*Cos(viewlr);
        }
    }
    else if (ch == 'q' || ch=='Q'){
        if(mode==3){
            viewlr-=15;
        }
    }
    else if (ch == 'e' || ch=='E'){
        if(mode==3){
            viewlr+=15;
        }
    }
    //  Change field of view angle
    else if (ch == '-' && ch>1)
        fov--;
    else if (ch == '+' && ch<179)
        fov++;
    
    Project();
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, width,height);
    //  Set projection
    Project();
}

/*
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    zh = fmod(90*t,360);
    glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
    //  Initialize GLUT
    glutInit(&argc,argv);
    //  Request double buffered, true color window with Z buffering at 600x600
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600,600);
    glutCreateWindow("HW4: Hanwen Zhao");
    //  Set callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(key);
    //  Pass control to GLUT so it can interact with the user
    glutMainLoop();
    return 0;
}
