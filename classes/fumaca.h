#ifndef _FUMACA_H
#define _FUMACA_H

#include "manipulaVetor.h"
#include <GL/glut.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cmath>


// estrutura para Lista de Fumaca
struct Fumaca{
    Coord pos;
    float vida;
    float VelVida; 
    bool Morre;
    float VTamanho;
    float Tamanho;
    Fumaca *Complementar;
    Fumaca *Principal;
    Fumaca *proximo;
    Fumaca *anterior;       
};


class ControlaFumaca{
private:
  float TimeCria;
  Fumaca *ListaFumaca;  
  Fumaca *UltimaFumaca;
  Fumaca *aux1,*aux3; 
  Coord posicao; 
  
        
public:
  
  ControlaFumaca();
  float densidade;    
  bool  Gerar;   
  void  Update(float DeltaTime);     
  void  DestroyFumaca(Fumaca  * p); 
  void  InserFumaca(Fumaca  * p);   
  void  AdicionarRastro(float x, float y , float z,Fumaca *complet);
  void  DesenhaFumaca(); 
  void  setPosicao(Coord pos);
};

ControlaFumaca::ControlaFumaca(){
  ListaFumaca = NULL;  
  UltimaFumaca = NULL;  
  
  densidade = 0.1;                             
}


void ControlaFumaca::setPosicao(Coord pos){
  posicao = pos; 
  
  if ((UltimaFumaca != NULL)&&(Gerar))
          UltimaFumaca->pos = posicao;  
     
}

void  ControlaFumaca::Update(const float DeltaTime){
           
  TimeCria += DeltaTime;      
       
  if(!Gerar) {
    aux1 = NULL;
  }
       
       
  if ((Gerar)&&(TimeCria>0.2)){  
    aux3 = aux1;                                   
    AdicionarRastro(posicao.x,posicao.y,posicao.z,aux1);
    aux1 = UltimaFumaca;
    if (aux3!=NULL)
      aux3->Complementar = aux1;
      
    TimeCria = 0;    
  }
  

  
 Fumaca * aux =  UltimaFumaca;   
 Fumaca * aux2; 
 
  while(aux!= NULL){    
 
  aux->Tamanho += DeltaTime*aux->VTamanho;
 
  if ((aux->Morre)&&(aux->vida>0))
    aux->vida -= DeltaTime*aux->VelVida; 
  
  aux2 = aux; 
  aux = aux->anterior; 
  
  if (aux2->vida<=0)  
    DestroyFumaca(aux2);  
}
  
      
}   





//******************************************************************************
// Remove Uma Fumaca da lista de Fumaca e destroy esta Fumaca
//******************************************************************************
void ControlaFumaca::DestroyFumaca(Fumaca  * p){ 
 Fumaca  * auxproximo; 
 Fumaca  * auxanterior; 
   
 if (p == NULL) 
    return;
    
  
     
 if (p == ListaFumaca)
   ListaFumaca = p->proximo;
   
 if (p == UltimaFumaca)
   UltimaFumaca = p->anterior;
 
 auxproximo = p->proximo; 
 auxanterior =  p->anterior; 
 
 if (p->Principal!=NULL)
   p->Principal->Complementar = NULL; 
 
 delete p; 
 
 if (auxproximo!= NULL){  
  auxproximo->anterior = auxanterior;  
 }
 
  if (auxanterior!= NULL){  
  auxanterior->proximo = auxproximo;  
 }   
}

//******************************************************************************
// Insere uma Fumaca na lista de Fumaca
//******************************************************************************
void ControlaFumaca::InserFumaca(Fumaca  * p){ 
if (ListaFumaca == NULL){
   ListaFumaca = p;                 
 }else{
   UltimaFumaca->proximo = p;
   p->anterior = UltimaFumaca;      
 }     
 UltimaFumaca = p; 
}



//******************************************************************************
// adiciona rastro
//******************************************************************************
void  ControlaFumaca::AdicionarRastro(float x, float y , float z ,Fumaca *complet ){
 Fumaca * aux;
 aux = new Fumaca;
 aux->proximo  = NULL;
 aux->anterior = NULL;
  
 aux->pos.x = x; 
 aux->pos.y = y; 
 aux->pos.z = z; 

 aux->Tamanho = 1;
 aux->VTamanho = 5.5;
 aux->vida = 1.1;
 aux->VelVida = 0.1;
 aux->Complementar = complet;
 aux->Principal = NULL;
 aux->Morre = true;
 InserFumaca(aux);

}





//******************************************************************************
// Desenha um quadrado que representa uma partícula
//******************************************************************************
void ControlaFumaca::DesenhaFumaca(){  
 Fumaca * aux =  UltimaFumaca;   
 
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);	
    glDisable(GL_LIGHTING);		
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDisable ( GL_CULL_FACE); 
    glDisable(GL_TEXTURE_2D);
  glPushMatrix();
 
 
 while(aux!= NULL){  
     
    if (aux->Complementar!= NULL){     
                            
        float intensidade;
        float intensidade2;
        
        if (aux->vida<1)
        intensidade = densidade*aux->vida; 
        else
        intensidade = densidade; 
        
        if (aux->vida<1)
        intensidade2 = densidade*aux->Complementar->vida; 
        else
        intensidade2 = densidade; 
        
        
                          
                               
        glBegin(GL_QUADS);      
        glColor4f(0.6,0.6,0.6,0); 
        glVertex3f( aux->pos.x,aux->pos.y+aux->Tamanho,aux->pos.z); 
        glVertex3f( aux->Complementar->pos.x,aux->Complementar->pos.y+aux->Tamanho,aux->Complementar->pos.z); 
        glColor4f(1,1,1,intensidade2); 
        glVertex3f( aux->Complementar->pos.x,aux->Complementar->pos.y,aux->Complementar->pos.z);
        glColor4f(1,1,1,intensidade); 
        glVertex3f( aux->pos.x,aux->pos.y,aux->pos.z);  
        
        glColor4f(0.6,0.6,0.6,0); 
        glVertex3f( aux->pos.x,aux->pos.y-aux->Tamanho,aux->pos.z); 
        glVertex3f( aux->Complementar->pos.x,aux->Complementar->pos.y-aux->Tamanho,aux->Complementar->pos.z); 
        glColor4f(0.5,0.5,0.5,intensidade2);  
        glVertex3f( aux->Complementar->pos.x,aux->Complementar->pos.y,aux->Complementar->pos.z);
        glColor4f(0.5,0.5,0.5,intensidade); 
        glVertex3f( aux->pos.x,aux->pos.y,aux->pos.z);
        
        glColor4f(0.6,0.6,0.6,0); 
        glVertex3f( aux->pos.x+aux->Tamanho,aux->pos.y,aux->pos.z); 
        glVertex3f( aux->Complementar->pos.x+aux->Tamanho,aux->Complementar->pos.y,aux->Complementar->pos.z); 
        glColor4f(1,1,1,intensidade2); 
        glVertex3f( aux->Complementar->pos.x,aux->Complementar->pos.y,aux->Complementar->pos.z);
        glColor4f(1,1,1,intensidade); 
        glVertex3f( aux->pos.x,aux->pos.y,aux->pos.z);
        
        glColor4f(0.6,0.6,0.6,0); 
        glVertex3f( aux->pos.x-aux->Tamanho,aux->pos.y,aux->pos.z); 
        glVertex3f( aux->Complementar->pos.x-aux->Tamanho,aux->Complementar->pos.y,aux->Complementar->pos.z); 
        glColor4f(1,1,1,intensidade2); 
        glVertex3f( aux->Complementar->pos.x,aux->Complementar->pos.y,aux->Complementar->pos.z);
        glColor4f(1,1,1,intensidade); 
        glVertex3f( aux->pos.x,aux->pos.y,aux->pos.z);    
        
        glColor4f(0.6,0.6,0.6,0); 
        glVertex3f( aux->pos.x,aux->pos.y,aux->pos.z-aux->Tamanho); 
        glVertex3f( aux->Complementar->pos.x,aux->Complementar->pos.y,aux->Complementar->pos.z-aux->Tamanho); 
        glColor4f(1,1,1,intensidade2); 
        glVertex3f( aux->Complementar->pos.x,aux->Complementar->pos.y,aux->Complementar->pos.z);
        glColor4f(1,1,1,intensidade); 
        glVertex3f( aux->pos.x,aux->pos.y,aux->pos.z);  
        
        glColor4f(0.6,0.6,0.6,0); 
        glVertex3f( aux->pos.x,aux->pos.y,aux->pos.z+aux->Tamanho); 
        glVertex3f( aux->Complementar->pos.x,aux->Complementar->pos.y,aux->Complementar->pos.z+aux->Tamanho); 
        glColor4f(1,1,1,intensidade2); 
        glVertex3f( aux->Complementar->pos.x,aux->Complementar->pos.y,aux->Complementar->pos.z);
        glColor4f(1,1,1,intensidade); 
        glVertex3f( aux->pos.x,aux->pos.y,aux->pos.z); 
        
        glEnd();
    }     
  aux = aux->anterior; 
 }
 
 
  glPopMatrix();  
  glEnable(GL_LIGHTING);	
  glDisable(GL_BLEND); 
  glEnable ( GL_CULL_FACE);
  glDepthMask(GL_TRUE);
  glEnable(GL_TEXTURE_2D);
}




#endif
