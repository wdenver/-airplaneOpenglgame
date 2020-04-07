#ifndef _A10_
#define _A10_

#include <GL/glut.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "Objeto.h"
#include "aviao.h"
#include "textura.h"




class A10: public Aviao{
  private:
   Objeto fuselagem;
   Objeto profundor;
   Objeto leme;
   Objeto rodaF;
   Objeto rodaE;
   Objeto rodaD;
   Objeto aileronD;
   Objeto aileronE;
   Objeto cockipit;
   Objeto vidro;
   Objeto sombra;
   GLuint TexFuse,TextRodas,TextCock,TexReflexo,TexSombra; 
   
      
  public:  
    A10();
    void carregarAviao();
    void Desenha();
    void DesenhaSombra(GLfloat altura);
};

A10::A10(){
     rotTrem = 0;
     rotTampas = 90;
     rotTrem = 0;
     rotAilerons = 0;
     rotProfundor = 0;
     rotLeme = 0;
     rotDirecao = 0; 
     
     Cockpit.x = -2.1*5; 
     Cockpit.y = 0.76*5;
     Cockpit.z = 0; 
     
     maxComandos = 20;
     VelMax = 1000;
     VelStal = 150;
     Potencia =1000;
     Massa = 3000;
     Diedro = 4;
     ValLeme = 0;
     VelMaxchao = 300;
     VelMaxAR = 1000;
     
     AlturaTrem = 1*5;
     
     
     
     numMissieis = 8;
     MissilPosicao[0].x = 5; 
     MissilPosicao[0].z = 12; 
     MissilPosicao[0].y = -0.8;
     MissilPosicao[1].x = 5; 
     MissilPosicao[1].z = -12; 
     MissilPosicao[1].y = -0.8;
     MissilPosicao[2].x = 5; 
     MissilPosicao[2].z = 14; 
     MissilPosicao[2].y = -0.4;
     MissilPosicao[3].x = 5; 
     MissilPosicao[3].z = -14; 
     MissilPosicao[3].y = -0.4; 
     MissilPosicao[4].x = 5; 
     MissilPosicao[4].z = 16; 
     MissilPosicao[4].y = -0.3;
     MissilPosicao[5].x = 5; 
     MissilPosicao[5].z = -16; 
     MissilPosicao[5].y = -0.3;  
     MissilPosicao[6].x = 5; 
     MissilPosicao[6].z = 18; 
     MissilPosicao[6].y = 0.1;
     MissilPosicao[7].x = 5; 
     MissilPosicao[7].z = -18; 
     MissilPosicao[7].y = 0.1;
     
     
     
     
      ResetaMovimentos(); 
}

void A10::carregarAviao(){
  fuselagem.carregaArq("./dados/avioes/a10/fuse.obj");
  profundor.carregaArq("./dados/avioes/a10/profundor.obj");
  sombra.carregaArq("./dados/avioes/a10/sombra.obj");
  rodaF.carregaArq("./dados/avioes/a10/rodaF.obj");
  rodaE.carregaArq("./dados/avioes/a10/rodaE.obj");
  rodaD.carregaArq("./dados/avioes/a10/rodaD.obj");
  cockipit.carregaArq("./dados/avioes/a10/cockipit.obj");
  vidro.carregaArq("./dados/avioes/a10/vidro.obj");
  
  
  
  vidro.material.shininess = 30;
  vidro.material.diffuse[0]=0.2;
  vidro.material.diffuse[1]=0.6;
  vidro.material.diffuse[2]=0.2;
  vidro.material.diffuse[3]=1.0;
  
  
  TextRodas = LoadTextura("./dados/avioes/a10/a10_gear.tga");
  TextCock  = LoadTextura("./dados/avioes/a10/a10_Cpit.tga");
  TexFuse   = LoadTextura("./dados/avioes/a10/a10_fuse.tga");
  TexReflexo= LoadTextura("./dados/mat/reflexo.tga"); 
  TexSombra = LoadTextura("./dados/avioes/a10/sombra.tga"); 
}


void A10::DesenhaSombra(GLfloat altura){
     
    GLfloat intensidade; 
    
    if(posicao.y<23)
      intensidade = 1;
    else
    intensidade = 23/(posicao.y); 
    
    glPushMatrix ();
     
        
    glTranslatef(posicao.x  , altura , posicao.z);
     glScalef(0.25f, 0.25f, 0.25f);    
    glRotatef(direcao,0,1,0);
    glTranslatef(2  , 0  , 0);	
    glColor4f(0,0,0,intensidade);
    
    glEnable(GL_BLEND);	
    glDisable(GL_LIGHTING);		
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
    // glBlendFunc(GL_ONE, GL_ONE);	
    glBindTexture(GL_TEXTURE_2D, TexSombra);
    sombra.desenha(true);
    glEnable(GL_LIGHTING);	
    glDisable(GL_BLEND); 
    glPopMatrix ();  
}

void A10::Desenha(){
    //glEnable(GL_LIGHTING);	
    glColor4f(1.0,1.0,1.0,1.0);
      
    glPushMatrix ();      
    glTranslatef(posicao.x  , posicao.y , posicao.z);
    
    
    /*glDisable(GL_LIGHTING); 
    glTranslatef(0  , 0  , 0);	
   // glScalef(10,10,10);
     glBegin(GL_LINES);
      glVertex3f(5.0,0,0.0);
      glVertex3f(-5.0,0.0,0.0);
      glVertex3f(0.0,5,0.0);
      glVertex3f(0.0,-5.0,0.0);
      glVertex3f(0.0,0.0,5);
      glVertex3f(0.0,0.0,-5);
     glEnd(); */
    
    
    //float altTrem =  8*sin(rotTrem/57.3f);
    
   
    if (voando)
    glTranslatef(0 , AlturaTrem , 0);
    glRotatef(direcao,0,1,0);
    glRotatef(-inclinacao,0,0,1);
    glRotatef(rotacao,1,0,0);
    
    if (voando)
    glTranslatef(0  , -AlturaTrem , 0);
  
  
  
       
    glTranslatef(0.5  , 0  , 0);	
           
           
    glScalef(0.25f, 0.25f, 0.25f);       
    glBindTexture(GL_TEXTURE_2D, TexFuse);
    fuselagem.desenha(true);
    
    // profundor
    glPushMatrix ();      
     glTranslatef(77  , 23  , 0);
     glRotatef(rotProfundor*1.5,0,0,1);
     glTranslatef(-77  , -23  , 0);
     profundor.desenha(true);
    glPopMatrix ();
    
 
    glBindTexture(GL_TEXTURE_2D, TextRodas);   
    // trem de pouso da frente
     glPushMatrix ();     
     glTranslatef(-48  , 17  , 0);     
     glRotatef(-rotTrem,0,0,1); 
     if(tremPouso)
       glRotatef(-rotLeme*1.3,0,1,0);       
     glTranslatef(48  , -17  , 0); 
     rodaF.desenha(true);
     glPopMatrix ();   
          
    // trem de pouso Esquerdo
     glPushMatrix ();      
     glTranslatef(0  , 17  , 29);
     glRotatef(-10,1,0,0);
     glRotatef(-rotTrem*0.9,0,0,1);
     glRotatef(10,1,0,0);
     glTranslatef(0  , -17  , -29);
     rodaE.desenha(true);
     glPopMatrix ();
     
     // trem de pouso direito
     glPushMatrix ();      
     glTranslatef(0  , 17  , -29);
     glRotatef(10,1,0,0);
     glRotatef(-rotTrem*0.9,0,0,1);
     glRotatef(-10,1,0,0);
     glTranslatef(0  , -17  , 29);
     
     rodaD.desenha(true);
     glPopMatrix ();
    
    
    glBindTexture(GL_TEXTURE_2D, TextCock);
    cockipit.desenha(true);
  
    glDisable(GL_LIGHTING);	
    glColor4f(1.0f, 1.0f, 1.0f, 0.4f);		
    glEnable(GL_BLEND);									
   // glDisable ( GL_CULL_FACE);	
   	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_GEN_S);				
    glEnable(GL_TEXTURE_GEN_T);	
    glBindTexture(GL_TEXTURE_2D, TexReflexo);
    vidro.desenha(true);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);	
    glEnable(GL_LIGHTING);	
   // glEnable ( GL_CULL_FACE);

    
    
  /*  glDisable(GL_LIGHTING); 
    glTranslatef(-48  , 18 , 0);	
    glScalef(10,10,10);
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
     glBegin(GL_LINES);
      glVertex3f(10.0,0,0.0);
      glVertex3f(-10.0,0.0,0.0);
      glVertex3f(0.0,10,0.0);
      glVertex3f(0.0,-10.0,0.0);
      glVertex3f(0.0,0.0,10);
      glVertex3f(0.0,0.0,-10);
     glEnd(); */
     
    
     
    
    glPopMatrix (); 
    
}

#endif
