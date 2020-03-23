#ifndef _OBJETO_H
#define _OBJETO_H

#include "manipulaVetor.h"
#include <GL/glut.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cmath>



using namespace std;





struct Material{
 bool habilitaText;     
 GLfloat ambient[4];
 GLfloat diffuse[4];  
 GLfloat specular[4]; 
 GLfloat shininess; 
 GLfloat emission[4];      
};



struct CoordText{
   GLfloat t1,t2;
};   

struct Faces{
   GLint v1, v2, v3, n1, n2, n3,t1,t2,t3;
};


class Objeto{
  private:
  Coord *vertices;
  Coord *normais;
  CoordText *textura;  
  Faces *faces;
  int contVertices, contNormais, contFaces, contTextura;

  public:
  Coord posicao;  
  GLfloat scala;     
  GLfloat rotx,roty,rotz;
  Material material;       
  Objeto();       
  void carregaArq(char* fname);
  void desenha(bool preenchido);
  bool TestaColisao(Coord segmento[2],Coord &ponto);
};

Objeto::Objeto(){
  posicao.x = posicao.y = posicao.z = 0; 
  rotx = roty = rotz = 0; 
  scala = 1;  

  material.habilitaText = true;
  material.shininess = 80; 
  material.specular[0]=material.specular[1]=material.specular[2]=material.specular[3]=1.0;  
  material.diffuse[0]=material.diffuse[1]=material.diffuse[2]=material.diffuse[3]=0.8;
  material.ambient[0]=material.ambient[1]=material.ambient[2]=material.ambient[3]=0.5;  
  material.emission[0]= material.emission[1]=material.emission[2]=material.emission[3]=0;    
         
}
  
bool Objeto::TestaColisao(Coord segmento[2],Coord &ponto){
 Coord triangulo[3];

  for (int j = 0; j < contFaces; ++j )
  {
      triangulo[0] = vertices[faces[j].v1-1];
      triangulo[1] = vertices[faces[j].v2-1];
      triangulo[2] = vertices[faces[j].v3-1];
      if (interceptaTriangulo(segmento,triangulo,ponto) == 1)
        return true;    
  } 
  return false;     
}  
  
  
  
void Objeto::carregaArq(char* fname){
  string Tipo;
  char descarta;
  contVertices = contNormais  = contFaces = contTextura = 0;

  ifstream myfile;
  myfile.open(fname);
  if(!myfile){
    cout << "Arquivo " << fname << " não aberto\n";
    exit (0);
  }
  
  // conta os tipos para a alocação ---
  while(true){
    myfile >> Tipo;
    if (Tipo == "v")       contVertices++;
    else if(Tipo ==  "vn") contNormais++;
    else if(Tipo == "f")   contFaces++;
    else if(Tipo == "vt")  contTextura++;  
    if (myfile.eof())
     break;
   }
   //---
     
   vertices = new Coord[contVertices];
   normais  = new Coord[contNormais];
   faces    = new Faces[contFaces];
   textura  = new CoordText[contTextura];
        
   contVertices = contNormais = contFaces = contTextura = 0;
   
   
   // reinicia a leitura ---  
   myfile.clear();
   myfile.seekg(0);

   while(true){
        myfile >> Tipo;
        
        if (Tipo == "v"){
           myfile >> vertices[contVertices].x;
           myfile >> vertices[contVertices].y;
           myfile >> vertices[contVertices].z;
           contVertices++;
        }
        else if(Tipo ==  "vn"){
           myfile >> normais[contNormais].x;
           myfile >> normais[contNormais].y;
           myfile >> normais[contNormais].z;
           contNormais++;
        }
        else if(Tipo ==  "vt"){
           myfile >> textura[contTextura].t1;
           myfile >> textura[contTextura].t2;       
           textura[contTextura].t2 = 1- textura[contTextura].t2;           
           contTextura++;
        }
        else if(Tipo == "f"){
           myfile >> faces[contFaces].v1;
           myfile >> descarta;
           myfile >> faces[contFaces].t1; 
           myfile >> descarta;
           myfile >> faces[contFaces].n1;
          
           myfile >> faces[contFaces].v2;
           myfile >> descarta;
           myfile >> faces[contFaces].t2; 
           myfile >> descarta;
           myfile >> faces[contFaces].n2;
           
           myfile >> faces[contFaces].v3;
           myfile >> descarta;
           myfile >> faces[contFaces].t3; 
           myfile >> descarta;
           myfile >> faces[contFaces].n3;

           contFaces++;
        }

        if (myfile.eof())
           break;
        }
}


void Objeto::desenha(bool preenchido){
    
  if (preenchido)   
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    
  
  
  glMaterialfv(GL_FRONT, GL_AMBIENT,  material.ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,  material.diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
  glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);
  glMaterialfv(GL_FRONT, GL_EMISSION, material.emission);
  
   if (material.habilitaText)
     glEnable(GL_TEXTURE_2D);
   else
     glDisable(GL_TEXTURE_2D);
  
  
  glPushMatrix();
  glTranslatef(posicao.x,posicao.y,posicao.z);
  
  glRotatef(roty,0,1,0);
  glRotatef(rotx,1,0,0);
  glRotatef(rotz,0,0,1);
  
  glScalef(scala,scala,scala);  
  
  
    
  //glColor4f(1.0,1.0,1.0,1.0);
  for (int j = 0; j < contFaces; ++j )
  { 
      
    glBegin ( GL_POLYGON );         
        glTexCoord2f( textura [faces[j].t1-1].t1,  textura [faces[j].t1-1].t2);
        glNormal3f (  normais [faces[j].n1-1].x,   normais [faces[j].n1-1].y,normais [faces[j].n1-1].z );    
        glVertex3f (  vertices[faces[j].v1-1].x,   vertices[faces[j].v1-1].y,vertices[faces[j].v1-1].z );  
       
        glTexCoord2f( textura [faces[j].t2-1].t1,  textura [faces[j].t2-1].t2);
        glNormal3f (  normais [faces[j].n2-1].x,   normais [faces[j].n2-1].y,normais [faces[j].n2-1].z );
        glVertex3f (  vertices[faces[j].v2-1].x,   vertices[faces[j].v2-1].y,vertices[faces[j].v2-1].z );
        
        glTexCoord2f( textura [faces[j].t3-1].t1,  textura [faces[j].t3-1].t2);
        glNormal3f (  normais [faces[j].n3-1].x,   normais [faces[j].n3-1].y,normais [faces[j].n3-1].z );
        glVertex3f (  vertices[faces[j].v3-1].x,   vertices[faces[j].v3-1].y,vertices[faces[j].v3-1].z );
    glEnd( );
   }      
 glPopMatrix();  
   
}

#endif
