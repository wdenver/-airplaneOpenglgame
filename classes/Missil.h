#ifndef _MISSIL_H
#define _MISSIL_H

#include <GL/glut.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "Objeto.h"
#include "fumaca.h"


class Missil{
  private:
    static Objeto *corpo;
    static GLuint Textura;          
         
  protected:
            

    
            
    Coord movDirecao;
    Coord posicao;
    Coord aPosicao; 
    GLfloat Velocidade; 
    GLfloat rotAilerons;
    ControlaFumaca Fumaca;
    GLfloat queda;
    
    GLfloat Idirecao;
    GLfloat Iinclinacao;
    
    float acelerador;
    float TempoVoando;
   
  public:        
   Missil();
   GLfloat direcao;
   GLfloat inclinacao;
   GLfloat rotacao;
   Aviao *AviaoSuporte;
   bool ativo;
    
   bool liberado;
   int ID;
   bool explodiu;
   void Atualiza(const double deltaTime);
   void setPosicao(Coord v);
   void setDirecao(GLfloat v);
   void setPosicao(GLfloat x, GLfloat y,GLfloat z);
   void Liberar();
   void getSegmento(Coord p[2]);
   Coord getPosicao();
   void Desenha();
   void ResetaMovimentos();
   static void Carrega(){ 
                          corpo = new Objeto; 
                          corpo->carregaArq("./dados/avioes/missil/missil.obj");                         
                          Textura = LoadTextura("./dados/avioes/missil/missil.tga");                        
                         return;
                        }
   
   Coord PegaCima();
   Coord PegaDirec();
   Coord PegaPosCockpit();
   
};

Objeto *Missil::corpo = NULL;
GLuint  Missil::Textura = 0;

Missil::Missil(){
     posicao.x = posicao.y = posicao.z = 0;
     movDirecao.x = movDirecao.y = movDirecao.z = 0 ;
     ResetaMovimentos();  
     AviaoSuporte = NULL;   
}



void Missil::Liberar(){
 liberado = true;    
}


void Missil::Desenha(){
    glPushMatrix();
    glTranslatef(posicao.x,posicao.y,posicao.z);
    glRotatef(direcao,0,1,0);
    glRotatef(-inclinacao,0,0,1);
    glRotatef(rotacao,1,0,0);
    glBindTexture(GL_TEXTURE_2D, Textura);
     glScalef(0.25f, 0.25f, 0.25f);       
    //glutWireSphere(2.0, 12, 12);
    //glutSolidCube(2);
    corpo->desenha(true);
    glPopMatrix();   
    
    
    Fumaca.DesenhaFumaca(); 
      
     
}

void Missil::ResetaMovimentos(){
     direcao = 0;
     inclinacao = 0;
     Velocidade = 0;  
     liberado = false; 
     explodiu = false;  
     acelerador = 0; 
     Fumaca.densidade = 1.0;
     TempoVoando = 0;
}


Coord Missil::PegaCima(){
  Coord aux;          
  aux.x = 0;
  aux.y = 1;
  aux.z = 0;  
  aux  = TransRotacao(aux, -inclinacao,0,0,1);
  aux  = TransRotacao(aux, -direcao,0,1,0);  
  
  return aux;  
}

Coord Missil::PegaDirec(){     
  Coord aux;          
  aux.x = -1;
  aux.y = 0;
  aux.z = 0;  
  aux  = TransRotacao(aux, -inclinacao,0,0,1);
  aux  = TransRotacao(aux, -direcao,0,1,0);
  
  return aux;
}

void Missil::Atualiza(const double deltaTime){
    
    if (!ativo) return; 
     
    aPosicao = posicao;
    Fumaca.Update(deltaTime); 
    
    
    if (explodiu)
      return;  
     
    if ((AviaoSuporte!=NULL)&&(!liberado)){
     Coord mpos;
      Fumaca.Gerar = false;
     AviaoSuporte->PegaMissilPosicao(mpos,ID);
     setPosicao(mpos);
     rotacao    =  AviaoSuporte->rotacao;     
     inclinacao =  AviaoSuporte->inclinacao; 
     direcao    =  AviaoSuporte->direcao; 
     Velocidade =  (AviaoSuporte->Velocidade)*0.98;
     queda = 1;
      Idirecao = direcao ;
      Iinclinacao = inclinacao; 
      TempoVoando = 0;
     
      
    }else{
        
        if(queda<20)
          queda += deltaTime*10;
          
        TempoVoando+= deltaTime;  
        
        if(TempoVoando>10){
                           Idirecao    += rand()%180 - 90;
                           Iinclinacao -= rand()%60;
              
              TempoVoando = 0;             
        }
        
        
        direcao += (Idirecao-direcao)*deltaTime*0.2;
        inclinacao += (Iinclinacao-inclinacao)*deltaTime*0.2;
          
        Fumaca.Gerar = true;     
        posicao.y -= queda*deltaTime;  
      //  
        movDirecao = PegaDirec();           
        
        if (acelerador < 1)
          acelerador  += 0.01*deltaTime;
          
          if (acelerador<=0.05)
            Fumaca.densidade = acelerador*15;
        
        Velocidade+= (2000-Velocidade)*deltaTime*acelerador; 
        
        posicao.x += movDirecao.x*Velocidade*deltaTime*1;
        posicao.z += movDirecao.z*Velocidade*deltaTime*1;
        posicao.y += movDirecao.y*Velocidade*deltaTime*1; 
        
        Fumaca.setPosicao(posicao);             
    }
    
     
 
}


void Missil::setPosicao(Coord v){
     posicao.x = v.x;
     posicao.y = v.y;
     posicao.z = v.z;  
     aPosicao = posicao;
     
     Fumaca.setPosicao(posicao); 
}

Coord Missil::getPosicao(){
     return posicao;    
}

void Missil::getSegmento(Coord p[2]){ 
   p[0]= posicao;
   p[1]= aPosicao;  
}

void Missil::setPosicao(GLfloat x, GLfloat y,GLfloat z){
     posicao.x = x;
     posicao.y = y;
     posicao.z = z;     
}

void Missil::setDirecao(GLfloat v){
     direcao = v;
}
#endif
  
