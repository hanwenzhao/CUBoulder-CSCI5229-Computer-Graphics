/*  Homework 5
 *  Hanwen Zhao: Lighting
 *
 *  Demonstrates basic lighting using a basic scene.
 *
 *  Key bindings:
 *  l          Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  F2         Toggle local viewer mode
 *  F3         Toggle light distance (1/5)
 *  F8         Change ball increment
 *  F9         Invert bottom normal
 *  m          Toggles light movement
 *  []         Lower/rise light
 *  p          Toggles ortogonal/perspective projection
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
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

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))


int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light


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

void ErrCheck(const char* where)
{
    int err = glGetError();
    if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

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
    // Set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
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
    glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1);
    glVertex3f(+1,-1, 1);
    glVertex3f(+1,+1, 1);
    glVertex3f(-1,+1, 1);
    //  Back
    glColor3f(0,1,0);
    glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,+1,-1);
    glVertex3f(+1,+1,-1);
    //  Right
    glColor3f(0,0,1);
    glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,+1,-1);
    glVertex3f(+1,+1,+1);
    //  Left
    glColor3f(1,1,0);
    glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,-1,+1);
    glVertex3f(-1,+1,+1);
    glVertex3f(-1,+1,-1);
    //  Top
    glColor3f(1,0,1);
    glNormal3f( 0,+1, 0);
    glVertex3f(-1,+1,+1);
    glVertex3f(+1,+1,+1);
    glVertex3f(+1,+1,-1);
    glVertex3f(-1,+1,-1);
    //  Bottom
    glColor3f(1,0,1);
    glNormal3f( 0,-1, 0);
    glVertex3f(-1,-1,-1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,-1,+1);
    glVertex3f(-1,-1,+1);
    // Door
    glColor3f(1,1,0);
    glNormal3f(   0, 0,    1);
    glVertex3f(-0.8,-1, 1.01);
    glVertex3f(-0.6,-1, 1.01);
    glVertex3f(-0.6,-0.2, 1.01);
    glVertex3f(-0.8,-0.2, 1.01);
    // Chimney Bottom
    glColor3f(1,0,1);
    glNormal3f( 0,-1, 0);
    glVertex3f(-0.4,-1,-0.4);
    glVertex3f(-0.4,-1,-0.6);
    glVertex3f(-0.6,-1,-0.6);
    glVertex3f(-0.6,-1,-0.4);
    // Chimney Top
    glColor3f(1,0,1);
    glNormal3f( 0,+1, 0);
    glVertex3f(-0.4,2.2,-0.4);
    glVertex3f(-0.4,2.2,-0.6);
    glVertex3f(-0.6,2.2,-0.6);
    glVertex3f(-0.6,2.2,-0.4);
    // Chimney Right
    glColor3f(1,0,1);
    glNormal3f(+1, 0, 0);
    glVertex3f(-0.4,2.2,-0.4);
    glVertex3f(-0.4,-1,-0.4);
    glVertex3f(-0.4,-1,-0.6);
    glVertex3f(-0.4,2.2,-0.6);
    // Chimney Back
    glColor3f(1,0,1);
    glNormal3f( 0, 0,-1);
    glVertex3f(-0.4,2.2,-0.6);
    glVertex3f(-0.4,-1,-0.6);
    glVertex3f(-0.6,-1,-0.6);
    glVertex3f(-0.6,2.2,-0.6);
    // Chimney Left
    glColor3f(1,0,1);
    glNormal3f(-1, 0, 0);
    glVertex3f(-0.6,2.2,-0.6);
    glVertex3f(-0.6,-1,-0.6);
    glVertex3f(-0.6,-1,-0.4);
    glVertex3f(-0.6,2.2,-0.4);
    // Chimney Front
    glColor3f(1,0,1);
    glNormal3f( 0, 0, 1);
    glVertex3f(-0.6,2.2,-0.4);
    glVertex3f(-0.6,-1,-0.4);
    glVertex3f(-0.4,-1,-0.4);
    glVertex3f(-0.4,2.2,-0.4);
    // Roof Side 1
    glColor3f(0,1,0);
    glNormal3f( 0,+1,+1);
    glVertex3f(-1,+2,0);
    glVertex3f(-1,+1,+1);
    glVertex3f(+1,+1,+1);
    glVertex3f(+1,+2,0);
    // Roof Side 2
    glColor3f(0,0,1);
    glNormal3f( 0,+1,-1);
    glVertex3f(-1,+2,0);
    glVertex3f(-1,+1,-1);
    glVertex3f(+1,+1,-1);
    glVertex3f(+1,+2,0);
    glEnd();
    //  Roof
    glBegin(GL_TRIANGLES);
    //  Roof Right
    glColor3f(1,1,0);
    glNormal3f(+1, 0, 0);
    glVertex3f(+1,+1,+1);
    glVertex3f(+1,+1,-1);
    glVertex3f(+1,+2,0);
    //  Roof Left
    glColor3f(1,0,1);
    glNormal3f(-1, 0, 0);
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
    // Set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
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
    glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1);
    glVertex3f(+1,-1, 1);
    glVertex3f(+1,+1, 1);
    glVertex3f(-1,+1, 1);
    //  Back
    glColor3f(0,1,0);
    glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,+1,-1);
    glVertex3f(+1,+1,-1);
    //  Right
    glColor3f(0,0,1);
    glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,+1,-1);
    glVertex3f(+1,+1,+1);
    //  Left
    glColor3f(1,1,0);
    glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,-1,+1);
    glVertex3f(-1,+1,+1);
    glVertex3f(-1,+1,-1);
    //  Top
    glColor3f(1,0,1);
    glNormal3f( 0,+1, 0);
    glVertex3f(-1,+1,+1);
    glVertex3f(+1,+1,+1);
    glVertex3f(+1,+1,-1);
    glVertex3f(-1,+1,-1);
    //  Bottom
    glColor3f(1,0,1);
    glNormal3f( 0,-1, 0);
    glVertex3f(-1,-1,-1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,-1,+1);
    glVertex3f(-1,-1,+1);
    //  End
    glEnd();
    
    //Wheels
    glBegin(GL_POLYGON);
    glColor3f(1,0,1);
    glNormal3f( 0, 0, 1);
    for(double i=0;i<=3.1416*2;i+=0.05)
    {
        x_1=r*cos(i);
        y_1=r*sin(i);
        glVertex3f(x_1+1,y_1-1,1.01);
    }
    glEnd();
    
    glBegin(GL_POLYGON);
    glColor3f(1,0,1);
    glNormal3f( 0, 0, 1);
    for(double i=0;i<=3.1416*2;i+=0.05)
    {
        x_1=r*cos(i);
        y_1=r*sin(i);
        glVertex3f(x_1-1,y_1-1,1.01);
    }
    glEnd();
    
    glBegin(GL_POLYGON);
    glColor3f(1,0,1);
    glNormal3f( 0, 0, -1);
    for(double i=3.1416*2;i>=0;i-=0.05)
    {
        x_1=r*cos(i);
        y_1=r*sin(i);
        glVertex3f(x_1+1,y_1-1,-1.01);
    }
    glEnd();
    
    glBegin(GL_POLYGON);
    glColor3f(1,0,1);
    glNormal3f( 0, 0, -1);
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
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
    double x = Sin(th)*Cos(ph);
    double y = Cos(th)*Cos(ph);
    double z =         Sin(ph);
    //  For a sphere at the origin, the position
    //  and normal vectors are the same
    glNormal3d(x,y,z);
    glVertex3d(x,y,z);
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
    int th,ph;
    float yellow[] = {1.0,1.0,0.0,1.0};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glScaled(r,r,r);
    //  White ball
    glColor3f(1,1,1);
    glMaterialf(GL_FRONT,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
    glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
    //  Bands of latitude
    for (ph=-90;ph<90;ph+=inc)
    {
        glBegin(GL_QUAD_STRIP);
        for (th=0;th<=360;th+=2*inc)
        {
            Vertex(th,ph);
            Vertex(th,ph+inc);
        }
        glEnd();
    }
    //  Undo transofrmations
    glPopMatrix();
}

void Project(double fov,double asp,double dim)
{
    //  Tell OpenGL we want to manipulate the projection matrix
    glMatrixMode(GL_PROJECTION);
    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective transformation
    if (fov)
        gluPerspective(fov,asp,dim/16,16*dim);
    //  Orthogonal transformation
    else
        glOrtho(-asp*dim,asp*dim,-dim,+dim,-dim,+dim);
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
    const double len=2.0;  //  Length of axes
    //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //  Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);
    
    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective - set eye position
    if (mode)
    {
        double Ex = -2*dim*Sin(th)*Cos(ph);
        double Ey = +2*dim        *Sin(ph);
        double Ez = +2*dim*Cos(th)*Cos(ph);
        gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
    }
    //  Orthogonal - set world orientation
    else
    {
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);
    }
    
    //  Flat or smooth shading
    glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);
    
    //  Light switch
    if (light)
    {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
    }
    else
        glDisable(GL_LIGHTING);
    
    //  Draw scene
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
    
    //  Draw axes - no lighting from here on
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
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
    
    //  Display parameters
    glWindowPos2i(5,5);
    Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
          th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
    if (light)
    {
        glWindowPos2i(5,45);
        Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
        glWindowPos2i(5,25);
        Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
    }
    
    //  Render the scene and make it visible
    ErrCheck("display");
    glFlush();
    glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
    //  Elapsed time in seconds
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    zh = fmod(90*t,360.0);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
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
    else if (key == GLUT_KEY_PAGE_DOWN)
        dim += 0.1;
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_UP && dim>1)
        dim -= 0.1;
    //  Smooth color model
    else if (key == GLUT_KEY_F1)
        smooth = 1-smooth;
    //  Local Viewer
    else if (key == GLUT_KEY_F2)
        local = 1-local;
    else if (key == GLUT_KEY_F3)
        distance = (distance==1) ? 5 : 1;
    //  Toggle ball increment
    else if (key == GLUT_KEY_F8)
        inc = (inc==10)?3:10;
    //  Flip sign
    else if (key == GLUT_KEY_F9)
        one = -one;
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    //  Update projection
    Project(mode?fov:0,asp,dim);
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
    else if (ch == 'x' || ch == 'X')
        axes = 1-axes;
    //  Toggle lighting
    else if (ch == 'l' || ch == 'L')
        light = 1-light;
    //  Switch projection mode
    else if (ch == 'p' || ch == 'P')
        mode = 1-mode;
    //  Toggle light movement
    else if (ch == 'm' || ch == 'M')
        move = 1-move;
    //  Move light
    else if (ch == '<')
        zh += 1;
    else if (ch == '>')
        zh -= 1;
    //  Change field of view angle
    else if (ch == '-' && ch>1)
        fov--;
    else if (ch == '+' && ch<179)
        fov++;
    //  Light elevation
    else if (ch=='[')
        ylight -= 0.1;
    else if (ch==']')
        ylight += 0.1;
    //  Ambient level
    else if (ch=='a' && ambient>0)
        ambient -= 5;
    else if (ch=='A' && ambient<100)
        ambient += 5;
    //  Diffuse level
    else if (ch=='d' && diffuse>0)
        diffuse -= 5;
    else if (ch=='D' && diffuse<100)
        diffuse += 5;
    //  Specular level
    else if (ch=='s' && specular>0)
        specular -= 5;
    else if (ch=='S' && specular<100)
        specular += 5;
    //  Emission level
    else if (ch=='e' && emission>0)
        emission -= 5;
    else if (ch=='E' && emission<100)
        emission += 5;
    //  Shininess level
    else if (ch=='n' && shininess>-1)
        shininess -= 1;
    else if (ch=='N' && shininess<7)
        shininess += 1;
    //  Translate shininess power to value (-1 => 0)
    shiny = shininess<0 ? 0 : pow(2.0,shininess);
    //  Reproject
    Project(mode?fov:0,asp,dim);
    //  Animate if requested
    glutIdleFunc(move?idle:NULL);
    //  Tell GLUT it is necessary to redisplay the scene
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
    Project(mode?fov:0,asp,dim);
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
    glutInitWindowSize(400,400);
    glutCreateWindow("Hanwen Zhao: Homework5");
    //  Set callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    //  Pass control to GLUT so it can interact with the user
    ErrCheck("init");
    glutMainLoop();
    return 0;
}

