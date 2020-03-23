#ifndef _TEXTURA_
#define _TEXTURA_

#include "tga.h"
//#include <windows.h>
#include <stdio.h>
//#include <gl/gl.h>
//#include <gl/glu.h>
# include <cstdlib>
# include <iostream>

using namespace std;


GLint LoadTextura(char *TexName)
  {
   TGAImg Img;        
   GLuint Texture;

 
   if(Img.Load(TexName)!=IMG_OK)
    return -1;

   glGenTextures(1,&Texture);            
   glBindTexture(GL_TEXTURE_2D,Texture); 

   
    if(Img.GetBPP()==24)
     glTexImage2D(GL_TEXTURE_2D,0,3,Img.GetWidth(),Img.GetHeight(),0,GL_RGB,GL_UNSIGNED_BYTE,Img.GetImg());
    else if(Img.GetBPP()==32)
     glTexImage2D(GL_TEXTURE_2D,0,4,Img.GetWidth(),Img.GetHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,Img.GetImg());
    else
     return -1;

   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//GL_LINEAR,GL_NEAREST
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

   return Texture;
}

#endif
