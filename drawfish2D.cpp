#include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include <math.h>
 
 #include "fssimplewindow.h"
 #include "ysglfontdata.h"

const double YsPi=3.1415927;

 class StringParser
 {
 protected:
 char *str;
 int numWord;
 int *wordTop;
 int *wordLength;
 
 public:
 StringParser();
 ~StringParser();
 void Initialize(void);
 
 int Parse(char str[]);
 int GetNumWord(void);
 void GetWord(char word[],int idx,int maxLength);
 
 protected:
 void SafeStrCpy(char dst[],char src[],int nLetters,int nLimit);
 };
 
 StringParser::StringParser()
 {
 str=NULL;
 wordTop=NULL;
 wordLength=NULL;
 Initialize();
 }
 
 StringParser::~StringParser()
 {
 Initialize();
 }
 
 void StringParser::Initialize(void)
 {
 if(NULL!=str)
 {
 delete [] str;
 str=NULL;
 }
 if(NULL!=wordTop)
 {
 delete [] wordTop;
 wordTop=NULL;
 }
 if(NULL!=wordLength)
 {
 delete [] wordLength;
 wordLength=NULL;
 }
 numWord=0;
 }
 
 int StringParser::Parse(char incoming[])
 {
 if(NULL==incoming)
 {
 Initialize();
 return 0;
 }
 
 Initialize();
 
 int L=strlen(incoming);
 str=new char [L+1];
 wordTop=new int [L];
 wordLength=new int [L];
 if(NULL==str || NULL==wordTop || NULL==wordLength)
 {
 Initialize();
 return 0;
 }
 strcpy(str,incoming);
 
 
 
 int i,state;
 
 state=0;
 numWord=0;
 for(i=0; str[i]!=0; i++)
 {
 switch(state)
 {
 case 0:
 if(str[i]!=' ' && str[i]!='\t' && str[i]!=',' && str[i]!='\n')
 {
 state=1;
 wordTop[numWord]=i;
 wordLength[numWord]=1;
 numWord++;
 }
 break;
 case 1:
 if(str[i]!=' ' && str[i]!='\t' && str[i]!=',' && str[i]!='\n')
 {
 wordLength[numWord-1]++;
 }
 else
 {
 state=0;
 }
 break;
 }
 }
 LOOPOUT:
 return numWord;
 }
 
 int StringParser::GetNumWord(void)
 {
 return numWord;
 }
 
 void StringParser::GetWord(char word[],int idx,int maxLength)
 {
 if(0<=idx && idx<numWord)
 {
 SafeStrCpy(word,str+wordTop[idx],wordLength[idx],maxLength);
 }
 else
 {
 word[0]=0;
 }
 }
 
 void StringParser::SafeStrCpy(char dst[],char src[],int nLetters,int nLimit)
 {
 int i,stopper;
 if(nLetters<nLimit)
 {
 stopper=nLetters;
 }
 else
 {
 stopper=nLimit;
 }
 
 for(i=0; i<stopper; i++)
 {
 dst[i]=src[i];
 }
 dst[stopper]=0;
 }
 
 class Vec3d
 {
 public:
 double x,y,z;
 };
 
 class Triangle
 {
 public:
 Vec3d vtx[3];
 };
 
 class Mesh
 {
     protected:
     int numTri;
     Triangle *tri;
     double x,y,theta;
     int color;
 
 public:
 Mesh(double,double,double,int);
 ~Mesh();
 void Initialize();
 
 void ReadFromFile(char fn[]);
 void Draw(void);
 };
 
 Mesh::Mesh(double movingX,double movingY,double movingTheta,int colorNum)
 {
     color=colorNum;//0 is black,1 is red
     tri=NULL;
     Initialize();
     x=movingX;
     y=movingY;
     theta=movingTheta;
 }
 
 Mesh::~Mesh()
 {
 Initialize();
 }
 
 void Mesh::Initialize()
 {
 if(NULL!=tri)
 {
 delete [] tri;
 tri=NULL;
 }
 numTri=0;
 }
 
 void Mesh::ReadFromFile(char fn[])
 {
     
 FILE *fp=fopen(fn,"r");
 if(NULL!=fp)
 {
 Initialize();
 
 char str[256];
 int numTriRead=0;
 int numVtxRead=0;
 int numVtx=0;
 tri=new Triangle [40000];
 while(NULL!=fgets(str,255,fp))
 {
 StringParser parser;
 parser.Parse(str);
 
 if(parser.GetNumWord()==4)
 {
 char param[256];
 if (numVtxRead<3)
 {
 parser.GetWord(param,1,255);
 tri[numTriRead].vtx[numVtxRead].x=atof(param);
 parser.GetWord(param,2,255);
 tri[numTriRead].vtx[numVtxRead].y=atof(param);
 parser.GetWord(param,3,255);
 tri[numTriRead].vtx[numVtxRead].z=atof(param);
 numVtxRead++;
 }
 else
 {
 numTriRead++;
 numVtxRead=0;
 parser.GetWord(param,1,255);
 tri[numTriRead].vtx[numVtxRead].x=atof(param);
 parser.GetWord(param,2,255);
 tri[numTriRead].vtx[numVtxRead].y=atof(param);
 parser.GetWord(param,3,255);
 tri[numTriRead].vtx[numVtxRead].z=atof(param);
 numVtxRead++;
 }
 numVtx++;
 }
 }
 numTri=numTriRead;
 //printf("%d vtx read %d tris read\n",numVtx,numTri);
 
 fclose(fp);
 }
 }
 
 void Mesh::Draw(void)
 {
 glLineWidth(1);
     if (color==0)
     {
         glColor3ub(0,0,0);
     }
     else if (color==1)
     {
         glColor3ub(255, 0, 0);
     }
     else if(color==2)
     {
         glColor3ub(100, 100, 100);
     }
 for(int i=0; i<numTri; ++i)//every triangle in the mesh
 {
     glBegin(GL_TRIANGLE_STRIP);
     for (int j=0; j<3; j++)//every vertex in the triangle
     {
         //rotation to theta
         double rx=cos(theta)*tri[i].vtx[j].x+sin(theta)*tri[i].vtx[j].y;
         double ry=-sin(theta)*tri[i].vtx[j].x+cos(theta)*tri[i].vtx[j].y;
         
         //translation to (x,y)
         double tx=rx+x;
         double ty=ry+y;

         glVertex2f(tx,ty);
     }
     glEnd();
 }
 }

void DrawOneSet(double x,double y)
{
    Mesh lotusAndBuds(x,y,0,1);
    lotusAndBuds.ReadFromFile("/Users/jingli/Documents/CMUcourse/2013Fall/24-780/fish/lotusAndBuds.stl");
    lotusAndBuds.Draw();
    
    Mesh lotusLeaf(x,y,0,0);
    lotusLeaf.ReadFromFile("/Users/jingli/Documents/CMUcourse/2013Fall/24-780/fish/lotusLeaf.stl");
    lotusLeaf.Draw();
    
    Mesh stem(x,y,0,2);
    stem.ReadFromFile("/Users/jingli/Documents/CMUcourse/2013Fall/24-780/fish/stem.stl");
    stem.Draw();
    
    Mesh ripple(x,y,0,0);
    ripple.ReadFromFile("/Users/jingli/Documents/CMUcourse/2013Fall/24-780/fish/ripple.stl");
    ripple.Draw();
    
    Mesh dragonFly(x,y,0,0);
    dragonFly.ReadFromFile("/Users/jingli/Documents/CMUcourse/2013Fall/24-780/fish/dragonFly.stl");
    dragonFly.Draw();
    
    
}

void DrawBackground(void)
{
    double x1=100.0,y1=300.0;
    double x2=650.0,y2=300.0;
    
    DrawOneSet(x1,y1);
    DrawOneSet(x2,y2);
}


int main(void)
{
    
    FsOpenWindow(16,16,800,600,1);
    
    FsPollDevice();
    while(FSKEY_NULL==FsInkey())
    {
        FsPollDevice();
        
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        glColor3ub(0,0,0);
        
        DrawBackground();
        
        ////////////////////////////////////////////////////////////////////////////////////////////
        //You can draw a fish use the code below
        double x=400.0;
        double y=300.0;
        double theta=30.0*YsPi/180.0;//theta is in radian
        Mesh blackFish(x,y,theta,0);//0 means a black , 1 means a red ,2 means gray
        blackFish.ReadFromFile("/Users/jingli/Documents/CMUcourse/2013Fall/24-780/fish/ChineseFish.stl");
        blackFish.Draw();
        
        Mesh redFish(x+50,y,0,1);//0 means a black , 1 means a red ,2 means gray
        redFish.ReadFromFile("/Users/jingli/Documents/CMUcourse/2013Fall/24-780/fish/ChineseFish.stl");
        redFish.Draw();

        
        
        



        
        FsSwapBuffers();
        FsSleep(10);
    }
}


