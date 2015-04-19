#include<stdlib.h>
#include <GL/glut.h>        //included glut library
#include "imageloader.h"      //loads image texture
#include<iostream>
#include<fstream>              //reads text files
using namespace std;
float chairx=0,chairy=0,chairz=0,sofax=7.5,sofay=0,sofaz=5,bedx=-7.5,bedy=0,bedz=0,tablex=0,tabley=0,tablez=0,obj,theta,texcode;
bool chairb=false,sofab=false,tableb=false,bedb=false;
void readme(){


ifstream file1("imgdatatransfer.txt");

if(file1.is_open())
    {
file1>>texcode;
while(file1>>obj){
    if(obj==0){
            file1>>bedx>>bedz;
            bedb=true;
    }
   else if(obj==2){
            file1>>sofax>>sofaz;
            sofab=true;
    }
    else if(obj==1){
            file1>>chairx>>chairz;
            chairb=true;
    }
    else if(obj==2){
            file1>>tablex>>tablez;
            tablex=tablex*20;
            tabley=tablez*20;
            tableb=true;
    }
}
file1.close();
}
}

class object   //class in which objects coordinates, normals and textures are stored
{
public:
    string slash;
    string type;
    float x[5000],y[5000],z[5000];//coordinates,
    float a[5000],b[5000],c[5000];//normals
    float m[5000],n[5000];//texture
    int f[9][5000]; //arrangements
    int i=0,j=0,l=0,k=0;
    object(int p)
    {
        ifstream file;
        if(p==0)
        {
            file.open("sofa.txt");//reads sofa
        }
        if(p==1)
        {
            file.open("armchair.txt");//reads chair
        }
        if(p==2)
        {
            file.open("bunkbed.txt");//reads bed
        }
        if(p==3)
        {
            file.open("objects/table/table.txt");// reads table
        }
        if(file.is_open())
        {
            while(file>>type)
            {
                if(type=="v")
                {
                    file>>x[i]>>y[i]>>z[i];//reads coordinates

                    i++;
                }
                else if(type=="vn")
                {
                    file>>a[j]>>b[j]>>c[j];//reads normals

                    j++;
                }
                else if(type=="vt")
                {
                    file>>m[l]>>n[l];//reads texture
                    l++;
                }
                else if(type=="f")
                {
                    file>>f[0][k]>>slash[0]>>f[1][k]>>slash[2]>>f[3][k]
                        >>f[3][k]>>slash[0]>>f[4][k]>>slash[1]>>f[5][k]
                        >>f[6][k]>>slash[0]>>f[7][k]>>slash[1]>>f[8][k];//reads faces
                    k++;
                }

            }
            file.close();
        }
    }

};
object sofaobj(0);
object chairobj(1);
object bedobj(2);
object tableobj(3);
using namespace std;
const float BOX_SIZE = 7.0; //The length of each side of the cube
float anglex = 0,angley=30;
float changex=1;            //The rotation of the box
GLuint sofatextureId,chairtextureId,bedtextureId,walltextureId,floortextureId,grasstextureId,tabletextureId;//The OpenGL id of the texture

void handleKeypress(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: //Escape key
        exit(0);
        break;
    case 97:anglex=anglex+2;//a key
    break;
    case 65:anglex=anglex+2;//a key
    break;
    case 68:anglex=anglex-2;//d key
    break;
    case 100:anglex=anglex-2;//a key
    break;
    case 119:angley=angley+2;//w key
    break;
    case 87:angley=angley+2;//w key
    break;
    case 83:angley=angley-2;//s key
    break;
    case 115:angley=angley-2;//s key
    break;
    case 121:changex=changex+0.1;//y key
    break;
    case 89:changex=changex+0.1;//y key
    break;
    case 117:changex=changex-0.1;//u key
    break;
    case 85:changex=changex-0.1;//u key
    break;
    }
}


GLuint loadTexture(Image* image)//Makes the image into a texture, and returns the id of the texture
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 image->width, image->height,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 image->pixels);
    return textureId;
}

void initRendering()
{
    glEnable(GL_DEPTH_TEST);        //need for gl 3d stuffs
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    //input various images
    Image* sofa = loadBMP("sofa.bmp");
    sofatextureId = loadTexture(sofa);
    delete sofa;
    Image* chair = loadBMP("armchair_n.bmp");
    chairtextureId = loadTexture(chair);
    delete chair;
    Image* bed = loadBMP("bunkbed.bmp");
    bedtextureId = loadTexture(bed);
    delete bed;
    Image* wall = loadBMP("wall1.bmp");
    walltextureId = loadTexture(wall);
    delete wall;
    Image* floor;
    if(texcode==0){floor = loadBMP("txtb1.bmp");}
    else if(texcode==1){floor = loadBMP("txtb1.bmp");}
    else if(texcode==2){floor = loadBMP("txtb1.bmp");}
    floortextureId = loadTexture(floor);
    delete floor;
    Image* grass = loadBMP("grass1.bmp");
    grasstextureId = loadTexture(grass);
    delete grass;
    Image* table = loadBMP("objects/table/table.bmp");
    tabletextureId = loadTexture(table);
    delete table;
}

void handleResize(int w, int h)//when window is resized
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 20000.0);
}

void drawScene()  //all the scene drawn in 3d are to be scripted here
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW); //gl uses model in matrixmode
    glLoadIdentity();

    glTranslatef(0.0, -8.0, -20.0); //starting point of camera
    glScalef(changex,changex,changex); //gets scaled on pressing y/u


    GLfloat ambientLight[] = {0.7f, 0.7f, 0.7f, 1.0f};//basic light
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightPos[] = {0, 3.5,0, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glRotatef(angley, 1.0f, 0.0f, 0.0f);    //rotation axis x
    glRotatef(anglex, 0.0f, 1.0f, 0.0f);    //rotation axis y

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,sofatextureId);//uses sofa texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(1.0f, 1.0f, 1.0f);//background color
    glBegin(GL_TRIANGLES);
if(sofab){
    for (int i=0; i<sofaobj.k; i++)//for loop used to make each triangle in 3d
    {
        glNormal3f(sofaobj.a[sofaobj.f[2][i]-1],sofaobj.b[sofaobj.f[2][i]-1],sofaobj.c[sofaobj.f[2][i]-1]);//normal
        glTexCoord2f(sofaobj.m[sofaobj.f[1][i]-1],sofaobj.n[sofaobj.f[1][i]-1]);//texture
        glVertex3f(sofaobj.x[sofaobj.f[0][i]-1]+sofax,sofaobj.y[sofaobj.f[0][i]-1]+sofay,sofaobj.z[sofaobj.f[0][i]-1]+sofaz);//coordinates
        glNormal3f(sofaobj.a[sofaobj.f[5][i]-1],sofaobj.b[sofaobj.f[5][i]-1],sofaobj.c[sofaobj.f[5][i]-1]);
        glTexCoord2f(sofaobj.m[sofaobj.f[4][i]-1],sofaobj.n[sofaobj.f[4][i]-1]);
        glVertex3f(sofaobj.x[sofaobj.f[3][i]-1]+sofax,sofaobj.y[sofaobj.f[3][i]-1]+sofay,sofaobj.z[sofaobj.f[3][i]-1]+sofaz);
        glNormal3f(sofaobj.a[sofaobj.f[8][i]-1],sofaobj.b[sofaobj.f[8][i]-1],sofaobj.c[sofaobj.f[8][i]-1]);
        glTexCoord2f(sofaobj.m[sofaobj.f[7][i]-1],sofaobj.n[sofaobj.f[7][i]-1]);
        glVertex3f(sofaobj.x[sofaobj.f[6][i]-1]+sofax,sofaobj.y[sofaobj.f[6][i]-1]+sofay,sofaobj.z[sofaobj.f[6][i]-1]+sofaz);
    }}
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,chairtextureId);//user chair texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
if(chairb){
    for (int i=0; i<sofaobj.k; i++)//for loop used to make each triangle in 3d
    {
        glNormal3f(chairobj.a[chairobj.f[2][i]-1],chairobj.b[chairobj.f[2][i]-1],chairobj.c[chairobj.f[2][i]-1]);
        glTexCoord2f(chairobj.m[chairobj.f[1][i]-1],chairobj.n[chairobj.f[1][i]-1]);
        glVertex3f(chairobj.x[chairobj.f[0][i]-1]+chairx,chairobj.y[chairobj.f[0][i]-1]+chairy,chairobj.z[chairobj.f[0][i]-1]+chairz);
        glNormal3f(chairobj.a[chairobj.f[5][i]-1],chairobj.b[chairobj.f[5][i]-1],chairobj.c[chairobj.f[5][i]-1]);
        glTexCoord2f(chairobj.m[chairobj.f[4][i]-1],chairobj.n[chairobj.f[4][i]-1]);
        glVertex3f(chairobj.x[chairobj.f[3][i]-1]+chairx,chairobj.y[chairobj.f[3][i]-1]+chairy,chairobj.z[chairobj.f[3][i]-1]+chairz);
        glNormal3f(chairobj.a[chairobj.f[8][i]-1],chairobj.b[chairobj.f[8][i]-1],chairobj.c[chairobj.f[8][i]-1]);
        glTexCoord2f(chairobj.m[chairobj.f[7][i]-1],chairobj.n[chairobj.f[7][i]-1]);
        glVertex3f(chairobj.x[chairobj.f[6][i]-1]+chairx,chairobj.y[chairobj.f[6][i]-1]+chairy,chairobj.z[chairobj.f[6][i]-1]+chairz);
    }}
    glEnd();
    glDisable(GL_TEXTURE_2D);//chair texture disabled
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,bedtextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
if(bedb){
    for (int i=0; i<sofaobj.k; i++)//for loop used to make each triangle in 3d
    {
        glNormal3f(bedobj.a[bedobj.f[2][i]-1],bedobj.b[bedobj.f[2][i]-1],bedobj.c[bedobj.f[2][i]-1]);
        glTexCoord2f(bedobj.m[bedobj.f[1][i]-1],bedobj.n[bedobj.f[1][i]-1]);
        glVertex3f(bedobj.x[bedobj.f[0][i]-1]+bedx,bedobj.y[bedobj.f[0][i]-1]+bedy,bedobj.z[bedobj.f[0][i]-1]+bedz);
        glNormal3f(bedobj.a[bedobj.f[5][i]-1],bedobj.b[bedobj.f[5][i]-1],bedobj.c[bedobj.f[5][i]-1]);
        glTexCoord2f(bedobj.m[bedobj.f[4][i]-1],bedobj.n[bedobj.f[4][i]-1]);
        glVertex3f(bedobj.x[bedobj.f[3][i]-1]+bedx,bedobj.y[bedobj.f[3][i]-1]+bedy,bedobj.z[bedobj.f[3][i]-1]+bedz);
        glNormal3f(bedobj.a[bedobj.f[8][i]-1],bedobj.b[bedobj.f[8][i]-1],bedobj.c[bedobj.f[8][i]-1]);
        glTexCoord2f(bedobj.m[bedobj.f[7][i]-1],bedobj.n[bedobj.f[7][i]-1]);
        glVertex3f(bedobj.x[bedobj.f[6][i]-1]+bedx,bedobj.y[bedobj.f[6][i]-1]+bedy,bedobj.z[bedobj.f[6][i]-1]+bedz);
    }}
    glEnd();//for making triangles
    glDisable(GL_TEXTURE_2D);

    glScalef(0.05,0.05,0.05);//scale function used for table due to size being too large

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,tabletextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
if(tableb){
    for (int i=0; i<sofaobj.k; i++)//for loop used to make each triangle in 3d
    {
        glNormal3f(tableobj.a[tableobj.f[2][i]-1],tableobj.b[tableobj.f[2][i]-1],tableobj.c[tableobj.f[2][i]-1]);
        glTexCoord2f(tableobj.m[tableobj.f[1][i]-1],tableobj.n[tableobj.f[1][i]-1]);
        glVertex3f(tableobj.x[tableobj.f[0][i]-1]+tablex,tableobj.y[tableobj.f[0][i]-1]+tabley,tableobj.z[tableobj.f[0][i]-1]+tablez);
        glNormal3f(tableobj.a[tableobj.f[5][i]-1],tableobj.b[tableobj.f[5][i]-1],tableobj.c[tableobj.f[5][i]-1]);
        glTexCoord2f(tableobj.m[tableobj.f[4][i]-1],tableobj.n[tableobj.f[4][i]-1]);
        glVertex3f(tableobj.x[tableobj.f[3][i]-1]+tablex,tableobj.y[tableobj.f[3][i]-1]+tabley,tableobj.z[tableobj.f[3][i]-1]+tablez);
        glNormal3f(tableobj.a[tableobj.f[8][i]-1],tableobj.b[tableobj.f[8][i]-1],tableobj.c[tableobj.f[8][i]-1]);
        glTexCoord2f(tableobj.m[tableobj.f[7][i]-1],tableobj.n[tableobj.f[7][i]-1]);
        glVertex3f(tableobj.x[tableobj.f[6][i]-1]+tablex,tableobj.y[tableobj.f[6][i]-1]+tabley,tableobj.z[tableobj.f[6][i]-1]+tablez);
    }
}
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glScalef(20,20,20);//scale to normal



    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,floortextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(1.0f, 1.0f, 1.0f);
    //floor
    glBegin(GL_QUADS);

    glNormal3f(0,1,0);
    glTexCoord2f(0,0);
    glVertex3f(20,0,-15);
    glTexCoord2f(8,0);
    glVertex3f(-20,0,-15);
    glTexCoord2f(8,4);
    glVertex3f(-20,0,15);
    glTexCoord2f(0,4);
    glVertex3f(20,0,15);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,walltextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(1.0f, 1.0f, 1.0f);
    //wall
    glBegin(GL_QUADS);

    glNormal3f(-1,0,0);
    glTexCoord2f(0,0);
    glVertex3f(20,0,-15);
    glTexCoord2f(2,0);
    glVertex3f(20,8,-15);
    glTexCoord2f(2,7.5);
    glVertex3f(20,8,15);
    glTexCoord2f(0,7.5);
    glVertex3f(20,0,15);


    glNormal3f(0,0,1);
    glTexCoord2f(0,0);
    glVertex3f(20,0,-15);
    glTexCoord2f(2,0);
    glVertex3f(20,8,-15);
    glTexCoord2f(2,10);
    glVertex3f(-20,8,-15);
    glTexCoord2f(0,10);
    glVertex3f(-20,0,-15);


    glNormal3f(1,0,0);
    glTexCoord2f(0,0);
    glVertex3f(-20,0,-15);
    glTexCoord2f(2,0);
    glVertex3f(-20,8,-15);
    glTexCoord2f(2,7.5);
    glVertex3f(-20,8,15);
    glTexCoord2f(0,7.5);
    glVertex3f(-20,0,15);


    glNormal3f(0,0,-1);
    glTexCoord2f(0,0);
    glVertex3f(20,0,15);
    glTexCoord2f(2,0);
    glVertex3f(20,8,15);
    glTexCoord2f(2,10);
    glVertex3f(-20,8,15);
    glTexCoord2f(0,10);
    glVertex3f(-20,0,15);

    glEnd();
    glDisable(GL_TEXTURE_2D);
      glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,grasstextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(1.0f, 1.0f, 1.0f);
    //floor
    glBegin(GL_QUADS);//grass

    glNormal3f(0,1,0);
    glTexCoord2f(0,0);
    glVertex3f(400,-0.1,-300);
    glTexCoord2f(40,0);
    glVertex3f(-400,-0.1,-300);
    glTexCoord2f(40,30);
    glVertex3f(-400,-0.1,300);
    glTexCoord2f(0,30);
    glVertex3f(400,-0.1,300);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

//Called every 25 milliseconds
void update(int value)
{
    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv)
{   readme();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);//initial window size

    glutCreateWindow("home Design Software");
    initRendering();

    glutDisplayFunc(drawScene);//called the drawscene function
    glutKeyboardFunc(handleKeypress);//follewed the commands of keyboard
    glutReshapeFunc(handleResize);
    glutTimerFunc(25, update, 0);//updaTES EVERY 25 milliseconds

    glutMainLoop();//main loop keeps on running and never ends until escape is pressed
    return 0;
}
