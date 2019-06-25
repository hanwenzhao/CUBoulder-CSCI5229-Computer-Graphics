/*
 *  Lorenz Attractor
 *
 *  Display dimensional coordinates in 3D with Lorenz Attractor.
 *
 *  Key bindings:
 *  s      Randomly change Lorenz Parameter s
 *  b      Randomly change Lorenz Parameter b
 *  r      Randomly change Lorenz Parameter r
 *  =/-    Increase/decrease w
 *  arrows Change view angle
 *  0      Reset view angle
 *  q      Reset default Lorenz parameters
 *  ESC    Exit
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>       /* time */
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

struct dataPoint
{
    double X;
    double Y;
    double Z;
};


// Global Parameters
int th=0;       // Azimuth of view angle
int ph=0;       // Elevation of view angle
double z=0;     // Z variable
double w=35;     // W variable
double dim=2;   // Dimension of orthogonal box
int mode=3;
const char* text[] = {"","2D","3D constant Z","3D","4D"};  // Dimension display text
/*  Lorenz Parameters  */
double s  = 10;
double b  = 2.6666;
double r  = 28;
dataPoint lorenzPoints[50001];

void calculateDataPoints(dataPoint * lorenzPoints)
{
    int i;
    /*  Coordinates  */
    double x = 1;
    double y = 1;
    double z = 1;
    /*  Time step  */
    double dt = 0.001;
    lorenzPoints[0].X = 1;
    lorenzPoints[0].Y = 1;
    lorenzPoints[0].Z = 1;
    
    //printf("%5d %8.3f %8.3f %8.3f\n",0,x,y,z);
    /*
     *  Integrate 50,000 steps (50 time units with dt = 0.001)
     *  Explicit Euler integration
     */
    for (i=0;i<50000;i++)
    {
        double dx = s*(y-x);
        double dy = x*(r-z)-y;
        double dz = x*y - b*z;
        x += dt*dx;
        y += dt*dy;
        z += dt*dz;
        lorenzPoints[i+1].X = x;
        lorenzPoints[i+1].Y = y;
        lorenzPoints[i+1].Z = z;
        //printf("%5d %8.3f %8.3f %8.3f\n",i+1,x,y,z);
    }
}



#define LEN 8192  // Maximum length of text string
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

/*
 *  Display the scene
 */
void display()
{
    //  Clear the image
    glClear(GL_COLOR_BUFFER_BIT);
    //  Reset previous transforms
    glLoadIdentity();
    //  Set view angle
    glRotated(ph,1,0,0);
    glRotated(th,0,1,0);
    //  Draw 10 pixel yellow points
    glColor3f(1,1,0);
    glPointSize(1);
    glBegin(GL_POINTS);
    
    for (int i=0; i<=50000; i++)
    {
        //srand (time(NULL));
        //printf ("Random number: %f\n", ((double) rand() / (RAND_MAX)));
        //glColor3f(((double) rand()/(RAND_MAX)),((double) rand()/(RAND_MAX)),((double) rand()/(RAND_MAX)));
        if (i % 100 == 0)
        {
            glColor3f(((double) rand()/(RAND_MAX)),((double) rand()/(RAND_MAX)),((double) rand()/(RAND_MAX)));
        }
        glVertex4d(lorenzPoints[i].X,lorenzPoints[i].Y,lorenzPoints[i].Z,w);
    }
    
    glEnd();
    //  Draw axes in white
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    glVertex3d(0,0,0);
    glVertex3d(1,0,0);
    glVertex3d(0,0,0);
    glVertex3d(0,1,0);
    glVertex3d(0,0,0);
    glVertex3d(0,0,1);
    glEnd();
    //  Label axes
    glRasterPos3d(1,0,0);
    Print("X");
    glRasterPos3d(0,1,0);
    Print("Y");
    glRasterPos3d(0,0,1);
    Print("Z");
    //  Display parameters
    glWindowPos2i(5,5);
    Print("View Angle=%d,%d ",th,ph);
    Print("  w=%.1f",w);
    Print("  s=%.1f",s);
    Print("  b=%.1f",b);
    Print("  r=%.1f",r);
    
    //  Flush and swap
    glFlush();
    glutSwapBuffers();
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
    //  Increase w by 0.1
    else if (ch == '=')
    {
        w += 0.1;
    }
    //  Decrease w by 0.1
    else if (ch == '-')
    {
        w -= 0.1;
    }
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
    //  Right arrow key - increase azimuth by 5 degrees
    if (key == GLUT_KEY_RIGHT)
        th += 5;
    //  Left arrow key - decrease azimuth by 5 degrees
    else if (key == GLUT_KEY_LEFT)
        th -= 5;
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
        ph += 5;
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
        ph -= 5;
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
    //  Ratio of the width to the height of the window
    double w2h = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, width,height);
    //  Tell OpenGL we want to manipulate the projection matrix
    glMatrixMode(GL_PROJECTION);
    //  Undo previous transformations
    glLoadIdentity();
    //  Orthogonal projection box adjusted for the
    //  aspect ratio of the window
    glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
    //  Switch to manipulating the model matrix
    glMatrixMode(GL_MODELVIEW);
    //  Undo previous transformations
    glLoadIdentity();
}


/*
 *  Main
 */
int main(int argc, char *argv[])
{
    //  Calculate Lorenz data points
    calculateDataPoints(lorenzPoints);
    //  Initialize GLUT and process user parameters
    glutInit(&argc,argv);
    //  Request double buffered, true color window
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    //  Request 500 x 500 pixel window
    glutInitWindowSize(500,500);
    //  Create the window
    glutCreateWindow("Coordinates");
    //  Tell GLUT to call "display" when the scene should be drawn
    glutDisplayFunc(display);
    //  Tell GLUT to call "reshape" when the window is resized
    glutReshapeFunc(reshape);
    //  Tell GLUT to call "special" when an arrow key is pressed
    glutSpecialFunc(special);
    //  Tell GLUT to call "key" when a key is pressed
    glutKeyboardFunc(key);
    //  Pass control to GLUT so it can interact with the user
    glutMainLoop();
    //  Return code
    return 0;
}










