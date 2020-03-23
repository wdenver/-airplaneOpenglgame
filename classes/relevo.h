#ifndef _RELEVO_
#define _RELEVO_

#include "tga.h"
#include "textura.h"
//#include <windows.h>
#include <stdio.h>
//#include <gl/gl.h>
//#include <gl/glu.h>
# include <GL/glut.h>
# include <cstdlib>
# include <iostream>

typedef unsigned char BYTE;
			


using namespace std;

class Relevo{
  private:
    TGAImg Img;
    GLuint Texture;
    GLuint Texture2;
    int height,width; 
    float fatorTamanho;
    float fatorAltura;
    GLfloat px,pz;
    bool preenchido;
    int passo;
    float tamanho; 
  
    int Altura(BYTE *pHeightMap, int X, int Y);	
  public:
    Relevo();
    float largura,comprimento;
    GLint carregaArq(char* TexName);
    GLint carregaDetalhe(char* TexName);
    void DesenhaElev(int dist, bool mapNormal, GLfloat Alfa);
    void desenha(bool _preenchido, double x , double z , double alt);  
    void PegaQuadro(Coord v[],Coord P);
    GLfloat PegaAltura(double x , double z);  
    bool TestaColisao(Coord segmento[2],Coord &ponto);      
};


Relevo::Relevo(){
    passo = 50; 
    tamanho = 200; 
    fatorTamanho = 16;
    fatorAltura = 4;//3;             
}


bool Relevo::TestaColisao(Coord segmento[2],Coord &ponto){
   Coord p;

   p = segmento[0];

    Coord v[4];
    PegaQuadro(v,p);
    
    if (interceptaTriangulo(segmento ,v, ponto)==1)
      return  true;

     v[1] = v[3];
        
    if (interceptaTriangulo(segmento ,v, ponto)==1) 
       return  true;
       
    
    p = segmento[1];

    PegaQuadro(v,p);
    
    if (interceptaTriangulo(segmento ,v, ponto)==1)
      return  true;

     v[1] = v[3];
        
    if (interceptaTriangulo(segmento ,v, ponto)==1) 
       return  true;   
       
    
    
    return false;
        
     
} 



void Relevo::PegaQuadro(Coord v[],Coord P){
  int cx,cz;
  
  int posX = (int)(P.x/fatorTamanho);
  int posY = (int)(P.z/fatorTamanho);
  
  
  cx = (posX/passo);
  cx = cx*passo;
  cz = (posY/passo);
  cz = cz*passo; 
  
  if (cx<0) cx-=passo;
  if (cz<0) cz-=passo;
  
  
  
  v[0].x = cx*fatorTamanho;
  v[0].z = cz*fatorTamanho;
  v[0].y = Altura(Img.GetImg(), cx, cz)*fatorAltura;
  
  v[1].x = (cx+passo)*fatorTamanho;
  v[1].z = cz*fatorTamanho;
  v[1].y = Altura(Img.GetImg(), cx+passo, cz)*fatorAltura;
  
  v[2].x = (cx+passo)*fatorTamanho;
  v[2].z = (cz+passo)*fatorTamanho;
  v[2].y = Altura(Img.GetImg(), cx+passo, cz+passo)*fatorAltura;
  
  v[3].x = (cx)*fatorTamanho;
  v[3].z = (cz+passo)*fatorTamanho;
  v[3].y = Altura(Img.GetImg(), cx, cz+passo)*fatorAltura;
        
}


GLint Relevo::carregaDetalhe(char* TexName){
 Texture2 = LoadTextura(TexName);
           
}


void Relevo::DesenhaElev(int dist, bool mapNormal, GLfloat Alfa){ 
    float x,y,z; 

    int cx ;
    int cz ;
 
    cx = int(px/fatorTamanho); 
    cz = int(pz/fatorTamanho);
    

    
  
    glEnable(GL_TEXTURE_2D);
  //  glColor4f(1.0,1.0,1.0,1.0);  

//preenchido = false;

//	glDisable(GL_TEXTURE_2D);

   glPushMatrix();
  // glTranslatef(cx , 0 ,cz);
   
   // cx = 0; 
   // cz = 0;
   
   
     
    if (preenchido)   
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  

  
    
    
    float esc = 0;     
    
    int menor_x , maior_x;  
    int menor_y , maior_y;  
    int multx, multy;   
    
    cx = (cx/passo);
    cx = cx*passo;
    cz = (cz/passo);
    cz = cz*passo;

    
    for(int X= -dist ; X < +dist ; X ++)
    for(int Y= -dist ; Y < +dist ; Y ++){  
            
          
                                               		
	//glBegin(GL_QUADS);
    glBegin(GL_TRIANGLE_FAN);
           

            multx = 10;   
            multy = 10;  
          /*  
            
            if (abs(X) < front){
              if (X<0) auxFront = -front; 
              else
              auxFront = front;        
                       
    		  menor_x = cx+X*passo;
    		  maior_x = cx+(X+1)*passo;  
            //  menor_y = cz+Y*passo;		
            //  maior_y = cz+(Y+1)*passo;
            }else{
            menor_x = cx+((X-auxFront)*multx +  (float)auxFront) *passo;
            maior_x = cx+((X-auxFront+1)*multx +(float)auxFront) *passo; 
           //  menor_y = cz+((Y-front)*multy +  (float)front) *passo;
           // maior_y = cz+((Y-front+1)*multy +(float)front) *passo; 
             }*/
             
              menor_x = cx+X*passo;
    		  maior_x = cx+(X+1)*passo;  
             
    	      menor_y = cz+Y*passo;		
              maior_y = cz+(Y+1)*passo;
   			
            		
            x = menor_x;
            z = menor_y;  
            y = Altura(Img.GetImg(), menor_x, menor_y ); 
            glColor4f(1.0f, 1.0f, 1.0f, Alfa -(abs(X) + abs(Y))/dist );  
                    			
		    glNormal3f(0.0, 0.0, 1.0);	
            if(mapNormal)				
			glTexCoord2f(x/width, z/height);
            else
            glTexCoord2f(x/10, z/10);
            	
            
             			
			glVertex3f(x*fatorTamanho, y*fatorAltura, z*fatorTamanho);
            
			x = menor_x;										
			z = maior_y; ;
		    y = Altura(Img.GetImg(), menor_x, maior_y );
		    glColor4f(1.0f, 1.0f, 1.0f, Alfa -(abs(X) + abs(Y+1))/dist ); 
            
          //  glNormal3f(0.0, 1.0, 0.0);					
            if(mapNormal)				
			glTexCoord2f(x/width, z/height);
            else
            glTexCoord2f(x/10, z/10);
            						
			glVertex3f(x*fatorTamanho, y*fatorAltura, z*fatorTamanho);	
            
			x = maior_x;										
			z = maior_y ;
			y = Altura(Img.GetImg(), maior_x, maior_y);
			glColor4f(1.0f, 1.0f, 1.0f, Alfa -(abs(X+1) + abs(Y+1))/dist );  
            
          //  glNormal3f(0.0, 1.0, 0.0);					
			if(mapNormal)				
			glTexCoord2f(x/width, z/height);
            else
            glTexCoord2f(x/10, z/10);
            						
			glVertex3f(x*fatorTamanho, y*fatorAltura, z*fatorTamanho);	
            
			x = maior_x;
            z = menor_y ;										
			y = Altura(Img.GetImg(), maior_x,menor_y);
			glColor4f(1.0f, 1.0f, 1.0f, Alfa -(abs(X+1) + abs(Y))/dist );  
			
            
          //  glNormal3f(0.0, 1.0, 0.0);					
            if(mapNormal)				
			glTexCoord2f(x/width, z/height);
            else
            glTexCoord2f(x/10, z/10);
            						
			glVertex3f(x*fatorTamanho, y*fatorAltura, z*fatorTamanho);	
            
          	
          glEnd();			                                         											
   }
   
   glPopMatrix();    
     
     
}


int Relevo::Altura(BYTE *Mapa, int X, int Y)		
{ 
    X += width*50;
    Y += height*50;

	int x = X % width;							
	int y = Y % height;	   						

	if(!Mapa) return 0;							

	return Mapa[(x+ y * width)*4+3];				
}




GLfloat Relevo::PegaAltura(double x , double z){

    Coord raio[2];
    Coord Ponto;
    Coord p;
    p.x = x;
    p.z = z; 
    
    Coord v[4];
    PegaQuadro(v,p);
    
    raio[0].x = x;
    raio[0].z = z;
    raio[0].y = 5000;
    
    raio[1].x = x;
    raio[1].z = z;
    raio[1].y = -100;
    
  
    if (interceptaTriangulo(raio ,v, Ponto)>0)
      return  Ponto.y;

     v[1] = v[3];
        
     if (interceptaTriangulo(raio ,v, Ponto)>0) 
       return  Ponto.y;
    

      return -1000;
}



GLint Relevo::carregaArq(char* TexName){
     
 
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

   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    width = Img.GetWidth();
    height = Img.GetHeight(); 
    largura = width*fatorTamanho;
    comprimento = height*fatorTamanho;
   return Texture;   
}


void Relevo::desenha(bool _preenchido, double x , double z , double alt){
  px = x;
  pz = z;   
  
  preenchido =_preenchido;
     
     
  glPushMatrix();   

  
  glColor4f(1.0f, 1.0f, 1.0f, 0.5f);	
   glBindTexture(GL_TEXTURE_2D,Texture); 
   glTranslatef(0,-2,0);
   DesenhaElev(30,true,1);
   glTranslatef(0,1,0);
   
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
  	 
   glBindTexture(GL_TEXTURE_2D,Texture2); 
   if (alt < 4000)
   DesenhaElev(8,false,(8000-alt)*0.00005);
   glDisable(GL_BLEND);
   
  
  glPopMatrix();
     
}  


#endif
