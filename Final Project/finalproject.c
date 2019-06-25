#include "CSCIx229.h"

/*
 * Global Variables
 */
int mode = 1;           // mode 1 for town viewing
                        // mode 2 for driving
double dim = 50.0;      // size of the workd
double asp = 1;         // aspect ratio
int th=0;               //  Azimuth of view angle
int ph=5;               //  Elevation of view angle
int axes=1;             //  Display axes
int fov=45;       //  Field of view (for perspective)
double cx=0;            //  Camera Location
double cy=5;
double cz=4;
double view=1000;
double viewlr=-90;
double skyBoxScale = 20;
int light = 1;
int emission  = 60;  // Emission intensity (%)
int ambient   = 60;  // Ambient intensity (%)
int diffuse   = 60;  // Diffuse intensity (%)
int specular  = 60;  // Specular intensity (%)
int shininess = 64;  // Shininess (power of two)
float shiny   =   1;    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   2;  // Elevation of light
double repWall=4;
double repWall2 = 1;
unsigned int skyBoxTexture[10]; // Texture for Sky Box
unsigned int houseTexture[10]; // Texture for house
unsigned int storeTexture[10]; // Texture for store
unsigned int storeSignTexture[10]; // Texture for logos
unsigned int otherTexture[10];
int f1Car = 0;
double carPosition[] = {-3,0,3,90};
bool keyState[256] = {false};
double carSpeed = 0;
bool collisionDetection = false;


/*
 * Draw highway
 */
static void highway(double dx, double dy, double dz, double th,
                    double x, double y, double z)
{
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
    // Enable Texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,otherTexture[1]);

    //  Front
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,otherTexture[1]);
    glBegin(GL_QUADS);
    glNormal3f( 0, 1, 0);
    glTexCoord2f(0.0,0.0);  glVertex3f(+0,+0.01,+0);
    glTexCoord2f(0.0,1.0);  glVertex3f(+0,+0.01,+5*1.3);
    glTexCoord2f(1.0,1.0);  glVertex3f(+8*1.3,+0.01,+5*1.3);
    glTexCoord2f(1.0,0.0);  glVertex3f(+8*1.3,+0.01,+0);
    glEnd();
    //  Undo transformations
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/*
 * Draw the F1 Car
 */
static void drawF1Car(double dx,double dy, double dz, double th)
{
    glPushMatrix();
    //glScaled(0.15,0.15,0.15);
    glTranslated(dx,dy,dz);
    glRotated(th,0,1,0);
    glCallList(f1Car);
    glPopMatrix();
}

/*
 * Draw the Sky Box
 */

static void skyBox(double scale)
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
    glScaled(scale,scale,scale);
    // Enable Texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,skyBoxTexture[0]);
    // Front
    glBindTexture(GL_TEXTURE_2D,skyBoxTexture[0]);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,-1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f( +5,-3,+5);    // Lower Left
    glTexCoord2f(1.0f,0.0f);    glVertex3f( -5,-3,+5);    // Lower Right
    glTexCoord2f(1.0f,1.0f);    glVertex3f( -5,+7,+5);    // Upper Right
    glTexCoord2f(0.0f,1.0f);    glVertex3f( +5,+7,+5);    // Upper Left
    glEnd();
    // Back
    glBindTexture(GL_TEXTURE_2D,skyBoxTexture[1]);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,+1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f( -5,-3,-5);    // Lower Left
    glTexCoord2f(1.0f,0.0f);    glVertex3f( +5,-3,-5);    // Lower Right
    glTexCoord2f(1.0f,1.0f);    glVertex3f( +5,+7,-5);    // Upper Right
    glTexCoord2f(0.0f,1.0f);    glVertex3f( -5,+7,-5);    // Upper Left
    glEnd();
    // Left
    glBindTexture(GL_TEXTURE_2D,skyBoxTexture[2]);
    glBegin(GL_QUADS);
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0.0f,0.0f);    glVertex3f( -5, -3, +5);    // Lower Left
    glTexCoord2f(1.0f,0.0f);    glVertex3f( -5, -3, -5);    // Lower Right
    glTexCoord2f(1.0f,1.0f);    glVertex3f( -5, +7,-5);    // Upper Right
    glTexCoord2f(0.0f,1.0f);    glVertex3f( -5, +7,+5);    // Upper Left
    glEnd();
    // Right
    glBindTexture(GL_TEXTURE_2D,skyBoxTexture[3]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0.0f,0.0f);    glVertex3f( +5, -3, -5);    // Lower Left
    glTexCoord2f(1.0f,0.0f);    glVertex3f( +5, -3, +5);    // Lower Right
    glTexCoord2f(1.0f,1.0f);    glVertex3f( +5, +7,+5);    // Upper Right
    glTexCoord2f(0.0f,1.0f);    glVertex3f( +5, +7,-5);    // Upper Left
    glEnd();
    // Top
    glBindTexture(GL_TEXTURE_2D,skyBoxTexture[4]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0.0f,0.0f);    glVertex3f( +5,+7,-5);
    glTexCoord2f(1.0f,0.0f);    glVertex3f( +5,+7,+5);
    glTexCoord2f(1.0f,1.0f);    glVertex3f( -5,+7,+5);
    glTexCoord2f(0.0f,1.0f);    glVertex3f( -5,+7,-5);
    glEnd();
    // Bottom
    glBindTexture(GL_TEXTURE_2D,skyBoxTexture[5]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0.0f,0.0f);    glVertex3f( +5,+0,-5);
    glTexCoord2f(1.0f,0.0f);    glVertex3f( +5,+0,+5);
    glTexCoord2f(1.0f,1.0f);    glVertex3f( -5,+0,+5);
    glTexCoord2f(0.0f,1.0f);    glVertex3f( -5,+0,-5);
    glEnd();
    //  Undo transformations
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/*
 * Draw a store
 */

static void store(double x,double y,double z,
                  double dx,double dy,double dz,
                  double th, int n, int n2)
{
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
    // Enable Texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,storeTexture[0]);

    //  Front
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,storeTexture[n2]);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0, 1);
    glTexCoord2f(0.0f,0.0f);      glVertex3f(-1,+0, 1);
    glTexCoord2f(repWall2,0.0f);   glVertex3f(+1,+0, 1);
    glTexCoord2f(repWall2,repWall2);glVertex3f(+1,+2, 1);
    glTexCoord2f(0.0f,repWall2);   glVertex3f(-1,+2, 1);
    glEnd();
    //  Back
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,storeTexture[n2]);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,-1);
    glTexCoord2f(repWall2,0.0f);   glVertex3f(+1,-0,-1);
    glTexCoord2f(0.0f,0.0f);      glVertex3f(-1,-0,-1);
    glTexCoord2f(0.0f,repWall2);   glVertex3f(-1,+2,-1);
    glTexCoord2f(repWall2,repWall2);glVertex3f(+1,+2,-1);
    glEnd();
    //  Right
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,storeTexture[n2]);
    glBegin(GL_QUADS);
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0.0f,0.0f);         glVertex3f(+1,-0,+1);
    glTexCoord2f(repWall2,0.0f);      glVertex3f(+1,-0,-1);
    glTexCoord2f(repWall2,repWall2);   glVertex3f(+1,+2,-1);
    glTexCoord2f(0.0f,repWall2);      glVertex3f(+1,+2,+1);
    glEnd();
    //  Left
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,storeTexture[n2]);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0.0f,repWall2);         glVertex3f(-1,-0,-1);
    glTexCoord2f(repWall2,repWall2);      glVertex3f(-1,-0,+1);
    glTexCoord2f(repWall2,0.0f);         glVertex3f(-1,+2,+1);
    glTexCoord2f(0.0f,0.0f);            glVertex3f(-1,+2,-1);
    glEnd();
    //  Top
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,storeTexture[n2]);
    glBegin(GL_QUADS);
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0.0f,repWall2);         glVertex3f(-1,+2,+1);
    glTexCoord2f(repWall2,repWall2);      glVertex3f(+1,+2,+1);
    glTexCoord2f(repWall2,0.0f);         glVertex3f(+1,+2,-1);
    glTexCoord2f(0.0f,0.0f);            glVertex3f(-1,+2,-1);
    glEnd();
    //  Bottom
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,storeTexture[n2]);
    glBegin(GL_QUADS);
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0.0f,0.0f);            glVertex3f(-1,-0,-1);
    glTexCoord2f(repWall2,0.0f);         glVertex3f(+1,-0,-1);
    glTexCoord2f(repWall2,repWall2);      glVertex3f(+1,-0,+1);
    glTexCoord2f(0.0f,repWall2);         glVertex3f(-1,-0,+1);
    glEnd();
    // Sign Front
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,storeSignTexture[n]);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0, 1);
    glTexCoord2f(0.0f,0.0f);      glVertex3f(-1,+1.1, 1.01);
    glTexCoord2f(repWall2,0.0f);   glVertex3f(+1,+1.1, 1.01);
    glTexCoord2f(repWall2,repWall2);glVertex3f(+1,+2, 1.01);
    glTexCoord2f(0.0f,repWall2);   glVertex3f(-1,+2, 1.01);
    glEnd();
    // Door
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,storeTexture[2]);
    glBegin(GL_QUADS);
    glNormal3f(   0, 0,    1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(-0.8,-0, 1.01);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(-0.4,-0, 1.01);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(-0.4,+0.8, 1.01);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(-0.8,+0.8, 1.01);
    glEnd();
    // Windows
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,storeTexture[3]);
    glBegin(GL_QUADS);
    glNormal3f(   0, 0,    1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(0,-0, 1.01);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(0.8,-0, 1.01);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(0.8,+0.8, 1.01);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(0,+0.8, 1.01);
    glEnd();
    //  grass
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[6]);
    glBegin(GL_QUADS);
    glNormal3f( 0,1, 0);
    glTexCoord2f(0.0f,0.0f);            glVertex3f(-1.5,0.01,-1.5);
    glTexCoord2f(1,0.0f);               glVertex3f(+1.5,0.01,-1.5);
    glTexCoord2f(1,1);                  glVertex3f(+1.5,0.01,+1.5);
    glTexCoord2f(0.0f,1);               glVertex3f(-1.5,0.01,+1.5);
    glEnd();
    // fence front
    for (int i=0;i<4;i++){
        // body
        glColor3f(0,0,0);
        glBegin(GL_QUADS);
        glNormal3f( 0, 0,+1);
        glVertex3f(-1.5+i*0.2,0,1.5);
        glVertex3f(-1.48+i*0.2,0,1.5);
        glVertex3f(-1.48+i*0.2,0.5,1.5);
        glVertex3f(-1.5+i*0.2,0.5,1.5);
        glEnd();
        // top
        glColor3f(0,0,0);
        glBegin(GL_TRIANGLES);
        glNormal3f( 0, 0,+1);
        glVertex3f(-1.48+i*0.2,0.5,1.5);
        glVertex3f(-1.5+i*0.2,0.5,1.5);
        glVertex3f(-1.49+i*0.2,0.55,1.5);
        glEnd();
    }
    for (int i=0;i<10;i++){
        // body
        glColor3f(0,0,0);
        glBegin(GL_QUADS);
        glNormal3f( 0, 0,+1);
        glVertex3f(-1.5+1.1+i*0.2,0,1.5);
        glVertex3f(-1.48+1.1+i*0.2,0,1.5);
        glVertex3f(-1.48+1.1+i*0.2,0.5,1.5);
        glVertex3f(-1.5+1.1+i*0.2,0.5,1.5);
        glEnd();
        // top
        glColor3f(0,0,0);
        glBegin(GL_TRIANGLES);
        glNormal3f( 0, 0,+1);
        glVertex3f(-1.48+1.1+i*0.2,0.5,1.5);
        glVertex3f(-1.5+1.1+i*0.2,0.5,1.5);
        glVertex3f(-1.49+1.1+i*0.2,0.55,1.5);
        glEnd();
    }
    // bar
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,+1);
    glVertex3f(-1.5,0.25,1.5);
    glVertex3f(-0.9,0.25,1.5);
    glVertex3f(-0.9,0.27,1.5);
    glVertex3f(-1.5,0.27,1.5);
    glEnd();
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,+1);
    glVertex3f(-0.4,0.25,1.5);
    glVertex3f(1.5,0.25,1.5);
    glVertex3f(1.5,0.27,1.5);
    glVertex3f(-0.4,0.27,1.5);
    glEnd();
    // fence back
    for (int i=0;i<16;i++){
        // body
        glColor3f(0,0,0);
        glBegin(GL_QUADS);
        glNormal3f( 0, 0,-1);
        glVertex3f(-1.5+i*0.2,0,-1.5);
        glVertex3f(-1.48+i*0.2,0,-1.5);
        glVertex3f(-1.48+i*0.2,0.5,-1.5);
        glVertex3f(-1.5+i*0.2,0.5,-1.5);
        glEnd();
        // top
        glColor3f(0,0,0);
        glBegin(GL_TRIANGLES);
        glNormal3f( 0, 0,-1);
        glVertex3f(-1.48+i*0.2,0.5,-1.5);
        glVertex3f(-1.5+i*0.2,0.5,-1.5);
        glVertex3f(-1.49+i*0.2,0.55,-1.5);
        glEnd();
    }
    // bar
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,-1);
    glVertex3f(-1.5,0.25,-1.5);
    glVertex3f(1.5,0.25,-1.5);
    glVertex3f(1.5,0.27,-1.5);
    glVertex3f(-1.5,0.27,-1.5);
    glEnd();

    // fence left
    for (int i=0;i<10;i++){
        // body
        glColor3f(0,0,0);
        glBegin(GL_QUADS);
        glNormal3f( +1, 0, 0);
        glVertex3f(1.5,0,-1.5+0.3*i);
        glVertex3f(1.5,0.5,-1.5+0.3*i);
        glVertex3f(1.5,0.5,-1.48+0.3*i);
        glVertex3f(1.5,0,-1.48+0.3*i);
        glEnd();
        // top
        glColor3f(0,0,0);
        glBegin(GL_TRIANGLES);
        glNormal3f( +1, 0, 0);
        glVertex3f(1.5,0.5,-1.5+0.3*i);
        glVertex3f(1.5,0.5,-1.48+0.3*i);
        glVertex3f(1.5,0.55,-1.49+i*0.3);
        glEnd();
    }
    // bar
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glNormal3f(+1, 0, 0);
    glVertex3f(1.5,0.25,-1.5);
    glVertex3f(1.5,0.25,1.5);
    glVertex3f(1.5,0.27,1.5);
    glVertex3f(1.5,0.27,-1.5);
    glEnd();

    // fence right
    for (int i=0;i<10;i++){
        // body
        glColor3f(0,0,0);
        glBegin(GL_QUADS);
        glNormal3f(-1, 0, 0);
        glVertex3f(-1.5,0,-1.5+0.3*i);
        glVertex3f(-1.5,0.5,-1.5+0.3*i);
        glVertex3f(-1.5,0.5,-1.48+0.3*i);
        glVertex3f(-1.5,0,-1.48+0.3*i);
        glEnd();
        // top
        glColor3f(0,0,0);
        glBegin(GL_TRIANGLES);
        glNormal3f(-1, 0, 0);
        glVertex3f(-1.5,0.5,-1.5+0.3*i);
        glVertex3f(-1.5,0.5,-1.48+0.3*i);
        glVertex3f(-1.5,0.55,-1.49+i*0.3);
        glEnd();
    }
    // bar
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glVertex3f(-1.5,0.25,-1.5);
    glVertex3f(-1.5,0.25,1.5);
    glVertex3f(-1.5,0.27,1.5);
    glVertex3f(-1.5,0.27,-1.5);
    glEnd();


    //  Undo transformations
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw a house
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void house(double x,double y,double z,
                  double dx,double dy,double dz,
                  double th, int n)
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
    // Enable Texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[0]);
    //  House

    //  Front
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[n]);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0, 1);
    glTexCoord2f(0.0f,0.0f);      glVertex3f(-1,+0, 1);
    glTexCoord2f(repWall,0.0f);   glVertex3f(+1,+0, 1);
    glTexCoord2f(repWall,repWall);glVertex3f(+1,+2, 1);
    glTexCoord2f(0.0f,repWall);   glVertex3f(-1,+2, 1);
    glEnd();
    //  Back
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[n]);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,-1);
    glTexCoord2f(repWall,0.0f);   glVertex3f(+1,-0,-1);
    glTexCoord2f(0.0f,0.0f);      glVertex3f(-1,-0,-1);
    glTexCoord2f(0.0f,repWall);   glVertex3f(-1,+2,-1);
    glTexCoord2f(repWall,repWall);glVertex3f(+1,+2,-1);
    glEnd();
    //  Right
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[n]);
    glBegin(GL_QUADS);
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0.0f,0.0f);         glVertex3f(+1,-0,+1);
    glTexCoord2f(repWall,0.0f);      glVertex3f(+1,-0,-1);
    glTexCoord2f(repWall,repWall);   glVertex3f(+1,+2,-1);
    glTexCoord2f(0.0f,repWall);      glVertex3f(+1,+2,+1);
    glEnd();
    //  Left
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[n]);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0.0f,repWall);         glVertex3f(-1,-0,-1);
    glTexCoord2f(repWall,repWall);      glVertex3f(-1,-0,+1);
    glTexCoord2f(repWall,0.0f);         glVertex3f(-1,+2,+1);
    glTexCoord2f(0.0f,0.0f);            glVertex3f(-1,+2,-1);
    glEnd();
    //  Top
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[n]);
    glBegin(GL_QUADS);
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0.0f,repWall);         glVertex3f(-1,+2,+1);
    glTexCoord2f(repWall,repWall);      glVertex3f(+1,+2,+1);
    glTexCoord2f(repWall,0.0f);         glVertex3f(+1,+2,-1);
    glTexCoord2f(0.0f,0.0f);            glVertex3f(-1,+2,-1);
    glEnd();
    //  Bottom
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[n]);
    glBegin(GL_QUADS);
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0.0f,0.0f);            glVertex3f(-1,-0,-1);
    glTexCoord2f(repWall,0.0f);         glVertex3f(+1,-0,-1);
    glTexCoord2f(repWall,repWall);      glVertex3f(+1,-0,+1);
    glTexCoord2f(0.0f,repWall);         glVertex3f(-1,-0,+1);
    glEnd();
    //  grass
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[6]);
    glBegin(GL_QUADS);
    glNormal3f( 0,1, 0);
    glTexCoord2f(0.0f,0.0f);            glVertex3f(-1.5,0.01,-1.5);
    glTexCoord2f(1,0.0f);               glVertex3f(+1.5,0.01,-1.5);
    glTexCoord2f(1,1);                  glVertex3f(+1.5,0.01,+1.5);
    glTexCoord2f(0.0f,1);               glVertex3f(-1.5,0.01,+1.5);
    glEnd();
    // Door
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[2]);
    glBegin(GL_QUADS);
    glNormal3f(   0, 0,    1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(-0.8,-0, 1.01);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(-0.4,-0, 1.01);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(-0.4,+0.8, 1.01);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(-0.8,+0.8, 1.01);
    glEnd();
    // Window 1
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[5]);
    glBegin(GL_QUADS);
    glNormal3f(   0, 0,    1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(0.4,0.4, 1.01);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(0.8,0.4, 1.01);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(0.8,+0.8, 1.01);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(0.4,+0.8, 1.01);
    glEnd();
    // Window 2
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[5]);
    glBegin(GL_QUADS);
    glNormal3f(   0, 0,    1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(0.4,1.4, 1.01);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(0.8,1.4, 1.01);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(0.8,1.8, 1.01);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(0.4,1.8, 1.01);
    glEnd();
    // Window 3
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[5]);
    glBegin(GL_QUADS);
    glNormal3f(   0, 0,    1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(-0.8,1.4, 1.01);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(-0.4,1.4, 1.01);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(-0.4,1.8, 1.01);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(-0.8,1.8, 1.01);
    glEnd();
    // Window 4
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[5]);
    glBegin(GL_QUADS);
    glNormal3f(   0, 0,   -1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(-0.8,1.4,-1.01);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(-0.4,1.4,-1.01);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(-0.4,1.8,-1.01);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(-0.8,1.8,-1.01);
    glEnd();
    // Window 5
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[5]);
    glBegin(GL_QUADS);
    glNormal3f(   0, 0,    -1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(0.4,1.4,-1.01);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(0.8,1.4,-1.01);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(0.8,1.8,-1.01);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(0.4,1.8,-1.01);
    glEnd();
    // Window 6
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[5]);
    glBegin(GL_QUADS);
    glNormal3f(   0, 0,    -1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(-0.8,0.4,-1.01);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(-0.4,0.4,-1.01);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(-0.4,0.8,-1.01);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(-0.8,0.8,-1.01);
    glEnd();
    // Window 7
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[5]);
    glBegin(GL_QUADS);
    glNormal3f(   0, 0,    -1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(0.4,0.4,-1.01);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(0.8,0.4,-1.01);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(0.8,0.8,-1.01);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(0.4,0.8,-1.01);
    glEnd();

    // Level Seperater
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
    glNormal3f(0,0,1);
    glVertex3f(-1.0,0.9,1.01);
    glVertex3f(-1.0,1.1,1.01);
    glVertex3f(+1.0,1.1,1.01);
    glVertex3f(+1.0,0.9,1.01);
    glEnd();

    // Chimney Bottom
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[1]);
    glBegin(GL_QUADS);
    glNormal3f( 0,-1, 0);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(-0.4,-0,-0.4+1);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(-0.4,-0,-0.6+1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(-0.6,-0,-0.6+1);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(-0.6,-0,-0.4+1);
    glEnd();
    // Chimney Top
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[1]);
    glBegin(GL_QUADS);
    glNormal3f( 0,+1, 0);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(-0.4,3.2,-0.4+1);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(-0.4,3.2,-0.6+1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(-0.6,3.2,-0.6+1);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(-0.6,3.2,-0.4+1);
    glEnd();
    // Chimney Right
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[1]);
    glBegin(GL_QUADS);
    glNormal3f(+1, 0, 0);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(-0.4,3.2,-0.4+1);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(-0.4,-0,-0.4+1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(-0.4,-0,-0.6+1);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(-0.4,3.2,-0.6+1);
    glEnd();
    // Chimney Back
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[1]);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,-1);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(-0.4,3.2,-0.6+1);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(-0.4,-0,-0.6+1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(-0.6,-0,-0.6+1);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(-0.6,3.2,-0.6+1);
    glEnd();
    // Chimney Left
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[1]);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(-0.6,3.2,-0.6+1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(-0.6,-0,-0.6+1);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(-0.6,-0,-0.4+1);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(-0.6,3.2,-0.4+1);
    glEnd();
    // Chimney Front
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[1]);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0, 1);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(-0.6,3.2,-0.4+1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(-0.6,-0,-0.4+1);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(-0.4,-0,-0.4+1);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(-0.4,3.2,-0.4+1);
    glEnd();
    //  Roof
    // Roof Side 1
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[3]);
    glBegin(GL_QUADS);
    glNormal3f( 0,+1,+1);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(-1,+3,0);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(-1,+2,+1);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(+1,+2,+1);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(+1,+3,0);
    glEnd();
    // Roof Side 2
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[3]);
    glBegin(GL_QUADS);
    glNormal3f( 0,+1,-1);
    glTexCoord2f(0.0f,1.0f);    glVertex3f(-1,+3,0);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(-1,+2,-1);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(+1,+2,-1);
    glTexCoord2f(1.0f,1.0f);    glVertex3f(+1,+3,0);
    glEnd();
    //  Roof Right
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[4]);
    glBegin(GL_TRIANGLES);
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(+1,+2,+1);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(+1,+2,-1);
    glTexCoord2f(0.5f,1.0f);    glVertex3f(+1,+3,0);
    glEnd();
    //  Roof Left
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,houseTexture[4]);
    glBegin(GL_TRIANGLES);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(1.0f,0.0f);    glVertex3f(-1,+2,+1);
    glTexCoord2f(0.0f,0.0f);    glVertex3f(-1,+2,-1);
    glTexCoord2f(0.5f,1.0f);    glVertex3f(-1,+3,0);
    glEnd();
    // fence front
    for (int i=0;i<4;i++){
        // body
        glColor3f(0,0,0);
        glBegin(GL_QUADS);
        glNormal3f( 0, 0,+1);
        glVertex3f(-1.5+i*0.2,0,1.5);
        glVertex3f(-1.48+i*0.2,0,1.5);
        glVertex3f(-1.48+i*0.2,0.5,1.5);
        glVertex3f(-1.5+i*0.2,0.5,1.5);
        glEnd();
        // top
        glColor3f(0,0,0);
        glBegin(GL_TRIANGLES);
        glNormal3f( 0, 0,+1);
        glVertex3f(-1.48+i*0.2,0.5,1.5);
        glVertex3f(-1.5+i*0.2,0.5,1.5);
        glVertex3f(-1.49+i*0.2,0.55,1.5);
        glEnd();
    }
    for (int i=0;i<10;i++){
        // body
        glColor3f(0,0,0);
        glBegin(GL_QUADS);
        glNormal3f( 0, 0,+1);
        glVertex3f(-1.5+1.1+i*0.2,0,1.5);
        glVertex3f(-1.48+1.1+i*0.2,0,1.5);
        glVertex3f(-1.48+1.1+i*0.2,0.5,1.5);
        glVertex3f(-1.5+1.1+i*0.2,0.5,1.5);
        glEnd();
        // top
        glColor3f(0,0,0);
        glBegin(GL_TRIANGLES);
        glNormal3f( 0, 0,+1);
        glVertex3f(-1.48+1.1+i*0.2,0.5,1.5);
        glVertex3f(-1.5+1.1+i*0.2,0.5,1.5);
        glVertex3f(-1.49+1.1+i*0.2,0.55,1.5);
        glEnd();
    }
    // bar
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,+1);
    glVertex3f(-1.5,0.25,1.5);
    glVertex3f(-0.9,0.25,1.5);
    glVertex3f(-0.9,0.27,1.5);
    glVertex3f(-1.5,0.27,1.5);
    glEnd();
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,+1);
    glVertex3f(-0.4,0.25,1.5);
    glVertex3f(1.5,0.25,1.5);
    glVertex3f(1.5,0.27,1.5);
    glVertex3f(-0.4,0.27,1.5);
    glEnd();
    // fence back
    for (int i=0;i<16;i++){
        // body
        glColor3f(0,0,0);
        glBegin(GL_QUADS);
        glNormal3f( 0, 0,-1);
        glVertex3f(-1.5+i*0.2,0,-1.5);
        glVertex3f(-1.48+i*0.2,0,-1.5);
        glVertex3f(-1.48+i*0.2,0.5,-1.5);
        glVertex3f(-1.5+i*0.2,0.5,-1.5);
        glEnd();
        // top
        glColor3f(0,0,0);
        glBegin(GL_TRIANGLES);
        glNormal3f( 0, 0,-1);
        glVertex3f(-1.48+i*0.2,0.5,-1.5);
        glVertex3f(-1.5+i*0.2,0.5,-1.5);
        glVertex3f(-1.49+i*0.2,0.55,-1.5);
        glEnd();
    }
    // bar
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,-1);
    glVertex3f(-1.5,0.25,-1.5);
    glVertex3f(1.5,0.25,-1.5);
    glVertex3f(1.5,0.27,-1.5);
    glVertex3f(-1.5,0.27,-1.5);
    glEnd();

    // fence left
    for (int i=0;i<10;i++){
        // body
        glColor3f(0,0,0);
        glBegin(GL_QUADS);
        glNormal3f( +1, 0, 0);
        glVertex3f(1.5,0,-1.5+0.3*i);
        glVertex3f(1.5,0.5,-1.5+0.3*i);
        glVertex3f(1.5,0.5,-1.48+0.3*i);
        glVertex3f(1.5,0,-1.48+0.3*i);
        glEnd();
        // top
        glColor3f(0,0,0);
        glBegin(GL_TRIANGLES);
        glNormal3f( +1, 0, 0);
        glVertex3f(1.5,0.5,-1.5+0.3*i);
        glVertex3f(1.5,0.5,-1.48+0.3*i);
        glVertex3f(1.5,0.55,-1.49+i*0.3);
        glEnd();
    }
    // bar
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glNormal3f(+1, 0, 0);
    glVertex3f(1.5,0.25,-1.5);
    glVertex3f(1.5,0.25,1.5);
    glVertex3f(1.5,0.27,1.5);
    glVertex3f(1.5,0.27,-1.5);
    glEnd();

    // fence right
    for (int i=0;i<10;i++){
        // body
        glColor3f(0,0,0);
        glBegin(GL_QUADS);
        glNormal3f(-1, 0, 0);
        glVertex3f(-1.5,0,-1.5+0.3*i);
        glVertex3f(-1.5,0.5,-1.5+0.3*i);
        glVertex3f(-1.5,0.5,-1.48+0.3*i);
        glVertex3f(-1.5,0,-1.48+0.3*i);
        glEnd();
        // top
        glColor3f(0,0,0);
        glBegin(GL_TRIANGLES);
        glNormal3f(-1, 0, 0);
        glVertex3f(-1.5,0.5,-1.5+0.3*i);
        glVertex3f(-1.5,0.5,-1.48+0.3*i);
        glVertex3f(-1.5,0.55,-1.49+i*0.3);
        glEnd();
    }
    // bar
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glVertex3f(-1.5,0.25,-1.5);
    glVertex3f(-1.5,0.25,1.5);
    glVertex3f(-1.5,0.27,1.5);
    glVertex3f(-1.5,0.27,-1.5);
    glEnd();

    //  Undo transformations
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius r
 */
static void ball(double x,double y,double z,double r)
{
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glScaled(r,r,r);
    //  White ball
    glColor3f(1,1,1);
    glutSolidSphere(1.0,16,16);
    //  Undo transofrmations
    glPopMatrix();
}

void keyOperations(){
    if (keyState['w']){
        if (carSpeed < 5){
            carSpeed += 0.025;
        }
    }
}

/*
 * Draw SpeedNeedle
 */
void speedNeedle()
{
    //  Save transform attributes (Matrix Mode and Enabled Modes)
    glPushAttrib(GL_TRANSFORM_BIT|GL_ENABLE_BIT);
    //  Save projection matrix and set unit transform
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    //  Save model view matrix and set to indentity
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-2,+2,-1,1,-1,1);
    // Draw line
    glLineWidth(0.5);
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    glVertex2f(1.6-0.25*Sin(69+carSpeed*10),-0.85-0.25*Cos(69+carSpeed*10));
    glVertex2f(1.6,-0.85);
    glEnd();
    glRotated(0,0,0,1);
    //  Reset model view matrix
    glPopMatrix();
    //  Reset projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    //  Pop transform attributes (Matrix Mode and Enabled Modes)
    glPopAttrib();
}

/*
 * Draw Speedometer
 */
void speedometer()
{
    //  Save transform attributes (Matrix Mode and Enabled Modes)
    glPushAttrib(GL_TRANSFORM_BIT|GL_ENABLE_BIT);
    //  Save projection matrix and set unit transform
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    //  Save model view matrix and set to indentity
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-2,+2,-1,1,-1,1);
    //  Draw instrument panel with texture
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,otherTexture[0]);
    glBegin(GL_QUADS);
    glTexCoord2d(0,0);glVertex2f(1.2,-1);
    glTexCoord2d(1,0);glVertex2f(2,-1);
    glTexCoord2d(1,1);glVertex2f(2,-0.7);
    glTexCoord2d(0,1);glVertex2f(1.2,-0.7);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //  Reset model view matrix
    glPopMatrix();
    //  Reset projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    //  Pop transform attributes (Matrix Mode and Enabled Modes)
    glPopAttrib();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
    keyOperations();
    if (keyState['w'] == false)
    {
        carSpeed = 0;
    }
    //  Length of axes
    const double len=2.0;
    //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //  Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    if (mode == 1)
    {
        //  Eye position
        double Ex = -2*dim*Sin(th)*Cos(ph);
        double Ey = +2*dim        *Sin(ph);
        double Ez = +2*dim*Cos(th)*Cos(ph);
        gluLookAt(Ex,Ey,Ez, 0,0,0, 0,Cos(ph),0);
    }
    if (mode == 2)
    {
        gluLookAt(carPosition[0]-6*Sin(carPosition[3]),carPosition[1]+2,carPosition[2]-6*Cos(carPosition[3]),
                  carPosition[0]+5*Sin(carPosition[3]),0,carPosition[2]+5*Cos(carPosition[3]),
                  0,1,0);
    }
    if (mode == 3)
    {
        gluLookAt(cx,cy,cz,cx+view*Cos(viewlr),cy,cz+view*Sin(viewlr),0,1,0);
    }
    if (collisionDetection == true)
    {
        Print("Collision Detected");
    }
    //Lighting
    if (light)
    {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,2.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,2.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,2.0};
        //  Light direction
        //float Position[]  = {0,10,0};
        //float Position[]  = {5*Cos(zh),ylight,5*Sin(zh),1};
        float Position[]  = {95,90,60,1};
        //  Draw light position as ball (still no lighting here)
        glColor3f(16,16,16);
        ball(Position[0],Position[1],Position[2], 5);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
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
    // Draw Scene
    skyBox(skyBoxScale);
    drawF1Car(carPosition[0],carPosition[1],carPosition[2], carPosition[3]);
    for (int i = 0; i <= 9; i++)
    {
        highway(1,1,1,0, 10*i,0.01,0);
        highway(1,1,1,0, -10*i,0.01,0);
        highway(1,1,1,0, 10*i,0.01,50);
        highway(1,1,1,0, -10*i,0.01,50);
        highway(1,1,1,0, 10*i,0.01,-50);
        highway(1,1,1,0, -10*i,0.01,-50);
        highway(1,1,1,90, 0,0,17+10*i);
        highway(1,1,1,90, 0,0,0-10*i);
        highway(1,1,1,90, 50,0,17+10*i);
        highway(1,1,1,90, 50,0,0-10*i);
        highway(1,1,1,90, -50,0,17+10*i);
        highway(1,1,1,90, -50,0,0-10*i);
    }

    //drawF1Car(5*Cos(zh),0,5*Sin(zh),0);
    house(+11,0,-3, 3,2,2, 0, 0);
    store(-4.5,0,-3, 3,3,2, 0, 1, 0);
    store(+11+11,0,-3, 3,2,2, 0, 0, 1);
    house(-4.5-11,0,-3, 3,4,2, 0, 7);
    house(+11+22,0,-3, 3,2,2, 0, 1);
    house(-4.5-22,0,-3, 3,3,2, 0, 1);
    store(-4.5-33,0,-3, 3,2,2, 0, 1, 1);
    store(+11+33,0,-3, 3,4,2, 0, 2, 0);

    house(+11, 0,10, 3,2,2, 180, 7);
    store(-4.5,0,10, 3,2,2, 180, 0, 0);
    house(+11+11, 0,10, 3,3,2, 180, 0);
    store(-4.5-11,0,10, 3,2,2, 180, 2, 0);
    house(+11+22, 0,10, 3,4,2, 180, 7);
    store(-4.5-22,0,10, 3,2,2, 180, 3, 0);
    house(+11+33, 0,10, 3,3,2, 180, 1);
    store(-4.5-33,0,10, 3,2,2, 180, 3, 1);

    house(+11,0,-3+50, 3,2,2, 0, 7);
    store(-4.5,0,-3+50, 3,2,2, 0, 0, 1);
    store(+11+11,0,-3+50, 3,2,2, 0, 1, 0);
    house(-4.5-11,0,-3+50, 3,2,2, 0, 1);
    house(+11+22,0,-3+50, 3,2,2, 0, 1);
    house(-4.5-22,0,-3+50, 3,2,2, 0, 0);
    store(-4.5-33,0,-3+50, 3,2,2, 0, 3, 0);
    store(+11+33,0,-3+50, 3,2,2, 0, 0, 1);

    house(+11, 0,   10+50, 3,4,2, 180, 0);
    store(-4.5,0,   10+50, 3,2,2, 180, 3, 1);
    house(+11+11, 0,10+50, 3,3,2, 180, 0);
    store(-4.5-11,0,10+50, 3,2,2, 180, 2, 1);
    house(+11+22, 0,10+50, 3,2,2, 180, 7);
    store(-4.5-22,0,10+50, 3,3,2, 180, 1, 1);
    house(+11+33, 0,10+50, 3,2,2, 180, 1);
    store(-4.5-33,0,10+50, 3,3,2, 180, 0, 0);

    house(+11,0,-3-50, 3,3,2, 0, 7);
    store(-4.5,0,-3-50, 3,2,2, 0, 1, 1);
    store(+11+11,0,-3-50, 3,2,2, 0, 0, 0);
    house(-4.5-11,0,-3-50, 3,4,2, 0, 1);
    house(+11+22,0,-3-50, 3,2,2, 0, 1);
    house(-4.5-22,0,-3-50, 3,3,2, 0, 0);
    store(-4.5-33,0,-3-50, 3,2,2, 0, 3, 0);
    store(+11+33,0,-3-50, 3,3,2, 0, 2, 0);

    house(+11, 0,   10-50, 3,3,2, 180, 1);
    store(-4.5,0,   10-50, 3,2,2, 180, 1, 1);
    house(+11+11, 0,10-50, 3,4,2, 180, 1);
    store(-4.5-11,0,10-50, 3,2,2, 180, 2, 1);
    house(+11+22, 0,10-50, 3,4,2, 180, 0);
    store(-4.5-22,0,10-50, 3,2,2, 180, 2, 1);
    house(+11+33, 0,10-50, 3,3,2, 180, 0);
    store(-4.5-33,0,10-50, 3,3,2, 180, 3, 0);

    store(9.5, 0, 18, 3,3,2, -90, 2, 0);
    house(9.5, 0, 18+10,    3,3,2, -90, 7);
    store(9.5, 0, 18+20, 3,4,2, -90, 2, 0);
    store(9.5, 0, 18-50, 3,3,2, -90, 0, 0);
    house(9.5, 0, 18+10-50,    3,4,2, -90, 7);
    store(9.5, 0, 18+20-50, 3,3,2, -90, 0, 1);

    store(9.5+50, 0, 18, 3,2,2, -90, 2, 0);
    house(9.5+50, 0, 18+10,    3,2,2, -90, 0);
    store(9.5+50, 0, 18+20, 3,3,2, -90, 3, 0);
    store(9.5+50, 0, 18-50, 3,2,2, -90, 0, 0);
    house(9.5+50, 0, 18+10-50,    3,2,2, -90, 7);
    store(9.5+50, 0, 18+20-50, 3,3,2, -90, 1, 1);

    store(9.5-50, 0, 18, 3,2,2, -90, 2, 1);
    house(9.5-50, 0, 18+10,    3,2,2, -90, 1);
    store(9.5-50, 0, 18+20, 3,3,2, -90, 3, 1);
    store(9.5-50, 0, 18-50, 3,2,2, -90, 0, 1);
    house(9.5-50, 0, 18+10-50,    3,2,2, -90, 0);
    store(9.5-50, 0, 18+20-50, 3,3,2, -90, 1, 0);

    store(-3, 0, 18, 3,3,2, 90, 3, 0);
    house(-3, 0, 18+10,    3,3,2, 90, 0);
    store(-3, 0, 18+20, 3,4,2, 90, 0, 0);
    store(-3, 0, 18-50, 3,3,2, 90, 1, 0);
    house(-3, 0, 18+10-50,    3,4,2, 90, 7);
    store(-3, 0, 18+20-50, 3,3,2, 90, 2, 1);

    store(-3+50, 0, 18, 3,3,2, 90, 2, 1);
    house(-3+50, 0, 18+10,    3,3,2, 90, 7);
    store(-3+50, 0, 18+20, 3,4,2, 90, 1, 1);
    store(-3+50, 0, 18-50, 3,3,2, 90, 3, 1);
    house(-3+50, 0, 18+10-50,    3,4,2, 90, 0);
    store(-3+50, 0, 18+20-50, 3,3,2, 90, 0, 0);

    store(-3-50, 0, 18, 3,3,2, 90, 0, 0);
    house(-3-50, 0, 18+10,    3,3,2, 90, 0);
    store(-3-50, 0, 18+20, 3,4,2, 90, 1, 0);
    store(-3-50, 0, 18-50, 3,3,2, 90, 3, 1);
    house(-3-50, 0, 18+10-50,    3,4,2, 90, 1);
    store(-3-50, 0, 18+20-50, 3,3,2, 90, 2, 0);


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
    if (mode == 2)
    {
        glDisable(GL_DEPTH_TEST);
        speedometer();
        speedNeedle();
        glEnable(GL_DEPTH_TEST);
    }
    //  Render the scene and make it visible
    ErrCheck("display");
    glFlush();
    glutSwapBuffers();
}

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
    if (mode == 1)
    {
        //  Right arrow key - increase angle by 5 degrees
        if (key == GLUT_KEY_RIGHT){
            th += 5;
        }
        //  Left arrow key - decrease angle by 5 degrees
        else if (key == GLUT_KEY_LEFT){
            th -= 5;
        }
        //  Up arrow key - increase elevation by 5 degrees
        else if (key == GLUT_KEY_UP){
            ph += 5;
        }
        //  Down arrow key - decrease elevation by 5 degrees
        else if (key == GLUT_KEY_DOWN){
            ph -= 5;
        }
        //  PageUp key - increase dim
        else if (key == GLUT_KEY_PAGE_DOWN){
            dim += 0.1;
        }
        //  PageDown key - decrease dim
        else if (key == GLUT_KEY_PAGE_UP && dim>1){
            dim -= 0.1;
        }
        //  Keep angles to +/-360 degrees
        th %= 360;
        ph %= 360;
        //  Update projection
        //Project(fov,asp,dim);
        //  Tell GLUT it is necessary to redisplay the scene
        glutPostRedisplay();
    }
}

void keyUP(unsigned char key, int x, int y){
    keyState['w'] = false;
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
    if (mode == 1) //town viewing mode
    {
        // exit on ESC
        if (ch == 27)
            exit(0);
        //  Reset view angle
        else if (ch == '0'){
            th = 0;
            ph = 5;
        }
        else if (ch == 'd' || ch == 'D'){
            th += 5;
        }
        //  Left arrow key - decrease angle by 5 degrees
        else if (ch == 'a' || ch == 'A'){
            th -= 5;
        }
        //  Up arrow key - increase elevation by 5 degrees
        else if (ch == 'w' || ch == 'W'){
            if (ph +5 < 90)
            {
                ph += 5;
            }
        }
        //  Down arrow key - decrease elevation by 5 degrees
        else if (ch == 's' || ch == 'S'){
            if (ph-5>0)
            {
                ph -= 5;
            }
        }
        //  PageUp key - increase dim
        else if (ch == GLUT_KEY_PAGE_DOWN){
            dim += 0.1;
        }
        //  PageDown key - decrease dim
        else if (ch == GLUT_KEY_PAGE_UP && dim>1){
            dim -= 0.1;
        }
        //  Keep angles to +/-360 degrees
        th %= 360;
        ph %= 360;
    }
    if (mode == 2) // driving
    {
        if (ch == 27)
            exit(0);
        else if (ch == 'w' || ch == 'W'){
            keyState['w'] = true;
            if (!cdetection(carPosition[0]+carSpeed*Sin(carPosition[3]),carPosition[2]+carSpeed*Cos(carPosition[3])))
            {
                carPosition[2] += carSpeed*Cos(carPosition[3]);
                carPosition[0] += carSpeed*Sin(carPosition[3]);
            }
        }
        else if (ch == 'a' || ch == 'A'){
            carPosition[3] += 5;
        }
        else if (ch == 's' || ch == 'S'){
            if (!cdetection(carPosition[0]-0.5*Sin(carPosition[3]),carPosition[2]-0.5*Cos(carPosition[3])))
            {
                carPosition[2] -= 0.5*Cos(carPosition[3]);
                carPosition[0] -= 0.5*Sin(carPosition[3]);
            }
        }
        else if (ch == 'd' || ch == 'D'){
            carPosition[3] -= 5;
        }
    }
    if (mode ==3)
    {
        if (ch == 27)
            exit(0);
        else if (ch == 'w' || ch == 'W'){
            cx += 0.3*Cos(viewlr);
            cz += 0.3*Sin(viewlr);
        }
        else if (ch == 'a' || ch == 'A'){
            cx += 0.3*Sin(viewlr);
            cz -= 0.3*Cos(viewlr);
        }
        else if (ch == 's' || ch == 'S'){
            cx -= 0.3*Cos(viewlr);
            cz -= 0.3*Sin(viewlr);
        }
        else if (ch == 'd' || ch == 'D'){
            cx -= 0.3*Sin(viewlr);
            cz += 0.3*Cos(viewlr);
        }
        else if (ch == 'r' || ch == 'R'){
            if (cy+0.3 < skyBoxScale*10){
                cy += 0.3;
            }
        }
        else if (ch == 'f' || ch == 'F'){
            if (cy-0.3 > 0){
                cy -= 0.3;
            }
        }
        else if (ch == 'q' || ch=='Q'){
            viewlr-=15;
        }
        else if (ch == 'e' || ch=='E'){
            viewlr+=15;
        }
    }

    if (ch == '1')
    {
        mode = 1;
    }
    else if (ch == '2')
    {
        mode = 2;
    }
    else if (ch == '3')
    {
        mode = 3;
    }
    //  Change field of view angle
    else if (ch == '-' && ch>1)
        fov--;
    else if (ch == '+' && ch<179)
        fov++;
    //  Reproject
    Project(fov,asp,dim);
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
    Project(fov,asp,dim);
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
    glutInitWindowSize(1000,800);
    glutCreateWindow("Hanwen Zhao");
    //  Set callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(key);
    glutKeyboardUpFunc(keyUP);
    glutIdleFunc(idle);
    // Load Object
    f1Car = LoadOBJ("./ferrari-f1-race-car.obj");
    //  Load textures
    // Skybox texture
    skyBoxTexture[0] = LoadTexBMP("./texture/skyboxes/front.bmp");
    skyBoxTexture[1] = LoadTexBMP("./texture/skyboxes/back.bmp");
    skyBoxTexture[2] = LoadTexBMP("./texture/skyboxes/left.bmp");
    skyBoxTexture[3] = LoadTexBMP("./texture/skyboxes/right.bmp");
    skyBoxTexture[4] = LoadTexBMP("./texture/skyboxes/up.bmp");
    skyBoxTexture[5] = LoadTexBMP("./texture/skyboxes/down.bmp");
    // House texture
    houseTexture[0] = LoadTexBMP("./texture/brick.bmp");
    houseTexture[1] = LoadTexBMP("./texture/brick2.bmp");
    houseTexture[2] = LoadTexBMP("./texture/door.bmp");
    houseTexture[3] = LoadTexBMP("./texture/roof.bmp");
    houseTexture[4] = LoadTexBMP("./texture/roof2.bmp");
    houseTexture[5] = LoadTexBMP("./texture/window1.bmp");
    houseTexture[6] = LoadTexBMP("./texture/grass.bmp");
    houseTexture[7] = LoadTexBMP("./texture/brick3.bmp");
    // Store texture
    storeTexture[0] = LoadTexBMP("./texture/storewall.bmp");
    storeTexture[1] = LoadTexBMP("./texture/storewall2.bmp");
    storeTexture[2] = LoadTexBMP("./texture/storedoor.bmp");
    storeTexture[3] = LoadTexBMP("./texture/storewindows.bmp");
    // Logo Texture
    storeSignTexture[0] = LoadTexBMP("./texture/generalstore.bmp");
    storeSignTexture[1] = LoadTexBMP("./texture/mcdonalds.bmp");
    storeSignTexture[2] = LoadTexBMP("./texture/mlogo.bmp");
    storeSignTexture[3] = LoadTexBMP("./texture/panda.bmp");
    // Other Texture
    otherTexture[0] = LoadTexBMP("./texture/speedometer.bmp");
    otherTexture[1] = LoadTexBMP("./texture/highway.bmp");
    //  Pass control to GLUT so it can interact with the user
    ErrCheck("init");
    glutMainLoop();
    return 0;
}
