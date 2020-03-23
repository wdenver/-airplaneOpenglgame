#ifndef _F16_
#define _F16_

#include <GL/glut.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "Objeto.h"
#include "aviao.h"
#include "textura.h"




class F16: public Aviao{
  private:
   Objeto fuselagem;
   Objeto profundor;
   Objeto leme;
   Objeto rodaF;
   Objeto rodinhaF;
   Objeto rodaE;
   Objeto rodaD;
   Objeto aileronD;
   Objeto aileronE;
   Objeto tampaD;
   Objeto tampaE; 
   Objeto tampaF; 
   Objeto cockipit;
   Objeto vidro;
   Objeto sombra;
   GLuint TexFuse,TextRodas,TextCock,TexReflexo,TexSombra; 
   
      
  public:  
    F16();
    void carregarAviao();
    void Desenha();
    void DesenhaSombra(GLfloat altura);
};

F16::F16(){
     rotTrem = 0;
     rotTampas = 90;
     rotTrem = 0;
     rotAilerons = 0;
     rotProfundor = 0;
     rotLeme = 0;
     rotDirecao = 0; 
     
     Cockpit.x = -3.20*5; 
     Cockpit.y = 0.68*5;
     Cockpit.z = 0; 
     
     numMissieis = 4;
     MissilPosicao[0].x = 2; 
     MissilPosicao[0].z = 10; 
     MissilPosicao[0].y = -0.2;
     MissilPosicao[1].x = 2; 
     MissilPosicao[1].z = -10; 
     MissilPosicao[1].y = -0.2;
     MissilPosicao[2].x = 2; 
     MissilPosicao[2].z = 8; 
     MissilPosicao[2].y = -0.2;
     MissilPosicao[3].x = 2; 
     MissilPosicao[3].z = -8; 
     MissilPosicao[3].y = -0.2;
     
       
     maxComandos = 30;
     VelMax = 1000;
     VelStal = 220;
     Potencia = 1300;
     Massa = 1000;
     Diedro = 0;
     ValLeme = 1;
     VelMaxchao = 600;
     VelMaxAR = 1500;
     
     AlturaTrem = 1*5;
     
      ResetaMovimentos(); 
}

void F16::carregarAviao(){
  fuselagem.carregaArq("./dados/avioes/f16/fuselagem.obj");
  profundor.carregaArq("./dados/avioes/f16/profundor.obj");
  leme.carregaArq("./dados/avioes/f16/leme.obj");
  aileronD.carregaArq("./dados/avioes/f16/aileronD.obj");
  aileronE.carregaArq("./dados/avioes/f16/aileronE.obj");
  tampaD.carregaArq("./dados/avioes/f16/tampaD.obj");
  tampaE.carregaArq("./dados/avioes/f16/tampaE.obj");
  tampaF.carregaArq("./dados/avioes/f16/tampaF.obj");

  
  sombra.carregaArq("./dados/avioes/f16/sombra.obj");
  rodaF.carregaArq("./dados/avioes/f16/rodaF.obj");
  rodinhaF.carregaArq("./dados/avioes/f16/rodaFrent.obj");
  rodaE.carregaArq("./dados/avioes/f16/rodaE.obj");
  rodaD.carregaArq("./dados/avioes/f16/rodaD.obj");
  cockipit.carregaArq("./dados/avioes/f16/cockpit.obj");
  vidro.carregaArq("./dados/avioes/f16/vidro.obj");
  
  
  
  vidro.material.shininess = 50;
  vidro.material.diffuse[0]=0.2;
  vidro.material.diffuse[1]=0.2;
  vidro.material.diffuse[2]=0.4;
  vidro.material.diffuse[3]=1.0;
  
  
  TextRodas = LoadTextura("./dados/avioes/f16/f16_gear.tga");
  TextCock  = LoadTextura("./dados/avioes/f16/f16_Cpit.tga");
  TexFuse   = LoadTextura("./dados/avioes/f16/f16_fuse.tga");
  TexReflexo= LoadTextura("./dados/mat/reflexo.tga"); 
  TexSombra = LoadTextura("./dados/avioes/f16/sombra.tga"); 
}


void F16:: DesenhaSombra(GLfloat altura){
     
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

void F16::Desenha(){
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
    
    
    float altTrem =  8*sin(rotTrem/57.3);
    
   
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
     glTranslatef(35  , 23  , 0);
     glRotatef(rotProfundor,0,0,1);
     glTranslatef(-35  , -23  , 0);
     profundor.desenha(true);
    glPopMatrix ();
    
    // leme
    glPushMatrix ();      
     glTranslatef(35  , 37  , 0);
     glRotatef(-30,0,0,1);
     glRotatef(rotLeme,0,1,0);
     glRotatef(30,0,0,1);
     glTranslatef(-35  , -37  , 0);
     leme.desenha(true);
    glPopMatrix ();
    
    // Aileron Esquerdo
    glPushMatrix ();      
     glTranslatef(4  , 23  , 30);
     glRotatef(5,0,1,0);
     glRotatef(rotAilerons,0,0,1);
     glRotatef(-5,0,1,0);
     glTranslatef(-4  , -23  , -30);
    aileronE.desenha(true);
    glPopMatrix ();
    
    // Aileron Direito
    glPushMatrix ();      
     glTranslatef(4  , 23  , -30);
     glRotatef(-5,0,1,0);
     glRotatef(-rotAilerons,0,0,1);
     glRotatef(5,0,1,0);
     glTranslatef(-4  , -23  , 30);
     aileronD.desenha(true);
    glPopMatrix ();
     
    //tampa do trem de pouso esquerdo
     glPushMatrix ();      
     glTranslatef(-17  , 18  , 8);
     glRotatef(-rotTampas,1,0,0);
     glTranslatef(17  , -18  , -8);
     tampaE.desenha(true);
     glPopMatrix ();
     
     //tampa do trem de pouso direito
     glPushMatrix ();      
     glTranslatef(-17  , 18  , -8);
     glRotatef(rotTampas,1,0,0);
     glTranslatef(17  , -18  , 8);
     tampaD.desenha(true);
     glPopMatrix ();
     
     //tampa do trem de pouso da frente
     glPushMatrix ();      
     glTranslatef(-50  , 13  , -3);
     glRotatef(rotTampas,1,0,0);
     glTranslatef(50  , -13  , 3);
     tampaF.desenha(true);
     glPopMatrix ();
    
 
    
    glBindTexture(GL_TEXTURE_2D, TextRodas);
    
    // trem de pouso da frente
     glPushMatrix ();      
     glTranslatef(-54  , 14  , 0);
     glRotatef(rotTrem*1.5,0,0,1);      
     glTranslatef(54  , -14  , 0);
     rodaF.desenha(true);
     glRotatef(rotLeme,0,1,0);
     glPopMatrix ();   
     glPushMatrix ();   
     glTranslatef(-56  , 14  , 0);
     glRotatef(rotTrem*1.5,0,0,1);    
     glRotatef(-rotLeme,0,1,0);      
     glTranslatef(56  , -14  , 0);
     rodinhaF.desenha(true);
     glPopMatrix ();
     
    
    // trem de pouso Esquerdo
     glPushMatrix ();      
     glTranslatef(-10  , 18  , 5);
     glRotatef(-30,1,0,0);
     glRotatef(-rotTrem,0,0,1);
     glRotatef(-rotTrem*0.3,0,1,0);
     glRotatef(30,1,0,0);
     glTranslatef(10  , -18  , -5);
     rodaE.desenha(true);
     glPopMatrix ();
     
     // trem de pouso direito
     glPushMatrix ();      
     glTranslatef(-10  , 18  , -5);
     glRotatef(30,1,0,0);
     glRotatef(-rotTrem,0,0,1);
     glRotatef(rotTrem*0.3,0,1,0);
     glRotatef(-30,1,0,0);
     glTranslatef(10  , -18  , 5);
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

    
    /*
    glDisable(GL_LIGHTING); 
    glTranslatef(-10  , 25  , 0);	
    glScalef(10,10,10);
     glBegin(GL_LINES);
      glVertex3f(10.0,0,0.0);
      glVertex3f(-10.0,0.0,0.0);
      glVertex3f(0.0,10,0.0);
      glVertex3f(0.0,-10.0,0.0);
      glVertex3f(0.0,0.0,10);
      glVertex3f(0.0,0.0,-10);
     glEnd(); 
     */
    
     
    
    glPopMatrix (); 
    
}

#endif
