#ifndef _PARTICULA_H
#define _PARTICULA_H

#include "manipulaVetor.h"
#include <GL/glut.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cmath>


struct Queimador{
  Coord referencia;
  int densidade; 
  bool Gerar;
  float time;        
};

// estrutura para Lista de particulas
struct Particula{
    float px,py,pz,vx,vy,vz,rx,vrx;
    float ry;
    float fx,fy,fz;
    float origy;
    float r,g,b,a;
    float tCor;
    float VeltCor;
    float r1,g1,b1,a1;
    float r2,g2,b2,a2;
    float r3,g3,b3,a3;
    float vida;
    bool Morre;
    bool AltoBrilho;
    float Tamanho;
    float vTamanho;
    Particula *proximo;
    Particula *anterior;       
};


class ControlaParticulas{
private:
  Particula *ListaParticula;  
  Particula *UltimaParticula; 
  Queimador queima[20];
  int contQueima;        
public:
  ControlaParticulas();
  void  Reset();   
  void  Update(float aDeltaTime);     
  void  DestroyParticula(Particula  * p); 
  void  InserParticula(Particula  * p);   
  void  CriarParticulaFogo(float x, float y , float z);
  void  CriarParticulaExplode(float x, float y , float z);
  void  GerarExplosao(float x, float y , float z);
  void  DesenhaParticulas();
  void  AdicinaQueimador(Coord referencia,int densidade); 
};


void  ControlaParticulas::Reset(){
  contQueima = 0;    
}  

ControlaParticulas::ControlaParticulas(){
 Particula *ListaParticula;  
 Particula *UltimaParticula;  
 Reset();                
}

void  ControlaParticulas::AdicinaQueimador(Coord referencia,int densidade){
   queima[contQueima].referencia = referencia;
   queima[contQueima].densidade = densidade;
   queima[contQueima].time = 0;
   queima[contQueima].Gerar = true; 
   contQueima++;   
}

void  ControlaParticulas::Update(const float aDeltaTime){
       
     
  
  for(int i = 0 ; i < contQueima; i ++){
      if((queima[i].Gerar)&&(queima[i].time>0.3)){
        CriarParticulaFogo(queima[i].referencia.x,
                           queima[i].referencia.y,
                           queima[i].referencia.z); 
        queima[i].time = 0;                          
      } 
       queima[i].time +=  aDeltaTime;      
  }

  
 float DeltaTime = aDeltaTime*50;
  
 Particula * aux =  UltimaParticula;   
 Particula * aux2; 
  while(aux!= NULL){    
  aux->vy += aux->fy*DeltaTime; 
  aux->vx += aux->fx*DeltaTime;  
  aux->vz += aux->fz*DeltaTime;     
  aux->px += aux->vx*DeltaTime;
  aux->pz += aux->vz*DeltaTime;
  aux->py += aux->vy*DeltaTime;  
  aux->rx += aux->vrx*DeltaTime;
  aux->Tamanho *= aux->vTamanho;
  
  
  if (aux->tCor<1){
    aux->r = aux->r1 +(aux->r2-aux->r1)*aux->tCor;     
    aux->g = aux->g1 +(aux->g2-aux->g1)*aux->tCor; 
    aux->b = aux->b1 +(aux->b2-aux->b1)*aux->tCor;   
    aux->a = aux->a1 +(aux->a2-aux->a1)*aux->tCor;     
  }else
  if (aux->tCor<2){
    aux->r = aux->r2 +(aux->r3-aux->r2)*(aux->tCor-1);     
    aux->g = aux->g2 +(aux->g3-aux->g2)*(aux->tCor-1); 
    aux->b = aux->b2 +(aux->b3-aux->b2)*(aux->tCor-1);
    aux->a = aux->a2 +(aux->a3-aux->a2)*(aux->tCor-1);           
  }else{
    aux->r = aux->r3;     
    aux->g = aux->g3; 
    aux->b = aux->b3; 
    aux->a = aux->a3;      
  }
  aux->tCor += DeltaTime*aux->VeltCor;

  
  
  
  if ((aux->Morre)&&(aux->vida>0))
    aux->vida -= DeltaTime; 
  
  aux2 = aux; 
  aux = aux->anterior; 
  
  if ((aux2->vida<=0)||(aux2->origy > aux2->py))  
    DestroyParticula(aux2);  
}
  
      
}   





//******************************************************************************
// Remove Uma partícula da lista de partículas e destroy esta partícula
//******************************************************************************
void ControlaParticulas::DestroyParticula(Particula  * p){ 
 Particula  * auxproximo; 
 Particula  * auxanterior; 
   
 if (p == NULL) 
    return;
     
 if (p == ListaParticula)
   ListaParticula = p->proximo;
   
 if (p == UltimaParticula)
   UltimaParticula = p->anterior;
 
 auxproximo = p->proximo; 
 auxanterior =  p->anterior; 
 
 delete p; 
 
 if (auxproximo!= NULL){  
  auxproximo->anterior = auxanterior;  
 }
 
  if (auxanterior!= NULL){  
  auxanterior->proximo = auxproximo;  
 }   
}

//******************************************************************************
// Insere uma Partícula na lista de Partículas
//******************************************************************************
void ControlaParticulas::InserParticula(Particula  * p){ 
if (ListaParticula == NULL){
   ListaParticula = p;                 
 }else{
   UltimaParticula->proximo = p;
   p->anterior = UltimaParticula;      
 }     
 UltimaParticula = p; 
}



//******************************************************************************
// Cria uma Partícula do tipo fogo e insere esta Partícula na lista de Partículas
//******************************************************************************
void ControlaParticulas::CriarParticulaFogo(float x, float y , float z){
 Particula * aux;
 aux = new Particula;
 aux->proximo  = NULL;
 aux->anterior = NULL;
 aux->fx       = 0;
 aux->fz       = 0;
 aux->fy       = +0.0002;
 aux->px       = x;
 aux->py       = y;
 aux->pz       = z;
 aux->ry       = rand()%360;
 aux->origy    = aux->py;
 aux->vx    = ((rand()%20)/100.0)-0.1;
 aux->vy    = ((rand()%25)/100.0);
 aux->vz    = ((rand()%20)/100.0)-0.1;
 aux->vx    -= aux->vx/2;
 aux->vy    -= aux->vy/2;
 aux->vz    -= aux->vz/2;
 aux->Morre = true; 
 aux->AltoBrilho = false;
 
 aux->vida  = 1000;
 aux->vrx   = 0.01;
 
 aux->r1     = 1.0;
 aux->g1     = 1.0;
 aux->b1     = 0.0; 
 aux->a1     = 1.0;
 
 aux->r2     = 1.0;
 aux->g2     = 0.0;
 aux->b2     = 0.0; 
 aux->a2     = 0.2;
 
 aux->r3     = 0.0;
 aux->g3     = 0.0;
 aux->b3     = 0.0;
 aux->a3     = 0.2; 
 
 aux->VeltCor = 0.002;
 
 aux->tCor = 0;
 aux->Tamanho = 10; 
 aux->vTamanho = 1.001;
 InserParticula(aux);
}


//******************************************************************************
// Cria uma Partícula do tipo explosao e insere esta Partícula na lista de Partículas
//******************************************************************************
void ControlaParticulas::CriarParticulaExplode(float x, float y , float z){
 Particula * aux;
 aux = new Particula;
 aux->proximo  = NULL;
 aux->anterior = NULL;
 aux->fx       = 0;
 aux->fz       = 0;
 aux->fy       = -0.0001;
 aux->px       = x;
 aux->py       = y;
 aux->pz       = z;
 aux->ry       = rand()%360;
 aux->origy    = aux->py;
 aux->vx    = ((rand()%100)/100.0)-0.5;
 aux->vy    = ((rand()%50)/100.0);
 aux->vz    = ((rand()%100)/100.0)-0.5;
 
 aux->vx *= 0.5;
 aux->vy *= 0.5;
 aux->vz *= 0.5;
 
 aux->vida  = 1000;
 aux->vrx   = 0;
 aux->Morre = true;
 
 aux->AltoBrilho = false;
 
 aux->r1     = 1.0;
 aux->g1     = 1.0;
 aux->b1     = 0.0; 
 aux->a1     = 0.5; 
 
 aux->r2     = 0.0;
 aux->g2     = 0.0;
 aux->b2     = 0.0; 
 aux->a2     = 0.2;
 
 aux->r3     = 1.0;
 aux->g3     = 1.0;
 aux->b3     = 1.0;
 aux->a3     = 0.0; 
 
 aux->VeltCor = 0.002;
 aux->tCor = 0;
 aux->Tamanho = 10; 
 aux->vTamanho = 1.002;
 InserParticula(aux);
}


//******************************************************************************
// Desenha um quadrado que representa uma partícula
//******************************************************************************
void ControlaParticulas::DesenhaParticulas(){  
 Particula * aux =  UltimaParticula;   
 
  glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);	
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);			
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDisable ( GL_CULL_FACE); 
 
 
 while(aux!= NULL){ 
  GLfloat aux2 = aux->Tamanho*1.3;
  
   
    if (aux->AltoBrilho)
     glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    else
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glPushMatrix(); 
  glTranslatef(aux->px,aux->py,aux->pz);
  glRotatef(aux->ry,0,1,0);
  glRotatef(aux->rx,0,0,1);
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(aux->r,aux->g,aux->b,aux->a);  
    //glColor4f(aux->r,aux->g,aux->b,1);
    glVertex3f( 0,0,0);
    glColor4f(aux->r,aux->g,aux->b,0.0);   
    glVertex3f( -aux->Tamanho,-aux->Tamanho,0);   
    glVertex3f(  0,-aux2,0);  
    glVertex3f( +aux->Tamanho,-aux->Tamanho,0);      
    glVertex3f( +aux2,0,0);  
    glVertex3f( +aux->Tamanho,+aux->Tamanho,0);
    glVertex3f( 0,aux2,0);
    glVertex3f( -aux->Tamanho,+aux->Tamanho,0); 
    glVertex3f( -aux2,0,0);  
    glVertex3f( -aux->Tamanho,-aux->Tamanho,0);
  glEnd();

  glPopMatrix(); 
  aux = aux->anterior; 
 }
 
   
  glEnable(GL_LIGHTING);	
  glDisable(GL_BLEND); 
  glEnable ( GL_CULL_FACE);
  glDepthMask(GL_TRUE);
  glEnable(GL_TEXTURE_2D);	
 
}


void  ControlaParticulas::GerarExplosao(float x, float y , float z){
 for(int i = 0 ; i< 200 ; i++){   
   CriarParticulaExplode(x,y,z);
 }      
}


#endif
