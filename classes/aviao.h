#ifndef _AVIAO_
#define _AVIAO_

#include <GL/glut.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "Objeto.h"


class Aviao{
  protected:
    Coord movDirecao;
    Coord posicao;
    Coord aPosicao;
    
   
   
    GLfloat Turbina;
    GLfloat Ailerons; 
    GLfloat Profundor; 
    GLfloat Leme;  
    
    
   GLfloat rotTampas;
   GLfloat rotAilerons;
   
  
   GLfloat rotDirecao; 
   GLfloat maxComandos;
   GLfloat VelMax;

   GLfloat VelStal;
   GLfloat Diedro;
   GLfloat ValProfundor;
   GLfloat ValLeme;
   GLfloat Queda;
   
   GLfloat Massa;
   
   bool voando;
  
  
   
  public:        
   Aviao();
   GLfloat tremPouso; 
    int missilAtivo;
    GLfloat Forca; 
   GLfloat Potencia;
   GLfloat Velocidade; 
   GLfloat direcao;
   GLfloat inclinacao;
   GLfloat rotacao;
   Coord Cockpit;
   Coord MissilPosicao[8];
   int numMissieis;
   GLfloat AlturaTrem;
   GLfloat rotProfundor;
   GLfloat rotLeme;
   GLfloat VelMaxchao;
   GLfloat VelMaxAR;
   GLfloat rotTrem;
   bool freio;
   bool explodiu;
   bool Iexplodiu;
   int selecionaMissil();
   void Atualiza(const double deltaTime);
   void setAilerons(GLfloat v);
   void setProfundor(GLfloat v);
   void setLeme(GLfloat v);
   void setTremDePouso(bool v);
   void setAcelerador(GLfloat v); 
   void setPosicao(Coord v);
   void setPosicao(GLfloat x, GLfloat y,GLfloat z);
   void setDirecao(GLfloat v);
   void inverteTrem();
   void AumentaAcelerador();
   void DiminuiAcelerador();
   void ResetaMovimentos();
   bool PegaMissilPosicao(Coord &p , int id);
   Coord getPosicao();
   void getSegmento(Coord p[2]);
   virtual void Desenha() =0;
   virtual void DesenhaSombra(GLfloat altura) = 0;
   Coord PegaCima();
   Coord PegaDirec();
   Coord PegaPosCockpit();
   
};

Aviao::Aviao(){
     posicao.x = posicao.y = posicao.z = 0;
     movDirecao.x = movDirecao.y = movDirecao.z = 0 ;
    
}

int Aviao::selecionaMissil(){
    if(missilAtivo>0)
      missilAtivo--;
    return missilAtivo;   
}

bool Aviao::PegaMissilPosicao(Coord &p , int id){    
  
   p = MissilPosicao[id];
   if (!voando)
     p.y += AlturaTrem; 
      
     p  = TransRotacao(p, rotacao,1,0,0);
     p  = TransRotacao(p, -inclinacao,0,0,1);
     p  = TransRotacao(p, -direcao,0,1,0);
     
     p.x += posicao.x;
     p.y += posicao.y;
     p.z += posicao.z;
     
   if (voando)
     p.y += AlturaTrem;   
            
  
  return true; 
}


void Aviao::ResetaMovimentos(){
     direcao = 0;
     inclinacao = 0;
     rotacao = 0;
     Velocidade = 0;  
     Ailerons = 0; 
     Profundor = 0; 
     Leme = 0;  
     tremPouso = true;
     voando = false;
     Forca = 0;
     Turbina = 0;  
     Queda = 0; 
     explodiu = false;
     Iexplodiu = false;
     freio = false; 
     missilAtivo = numMissieis;
     
}

Coord Aviao::PegaPosCockpit(){
  Coord aux;     
  aux.x = Cockpit.x;
  aux.y = Cockpit.y;
  aux.z = Cockpit.z;  
  
     if (!voando)
     aux.y += AlturaTrem;  
  
  aux  = TransRotacao(aux, rotacao,1,0,0);
  aux  = TransRotacao(aux, -inclinacao,0,0,1);
  aux  = TransRotacao(aux, -direcao,0,1,0);    
  
   if (voando)
     aux.y += AlturaTrem;  
  
  return aux;     
      
 
}


Coord Aviao::PegaCima(){
  Coord aux;          
  aux.x = 0;
  aux.y = 1;
  aux.z = 0;  
  aux  = TransRotacao(aux, rotacao,1,0,0);
  aux  = TransRotacao(aux, -inclinacao,0,0,1);
  aux  = TransRotacao(aux, -direcao,0,1,0);  
  
  return aux;  
}

Coord Aviao::PegaDirec(){     
  Coord aux;          
  aux.x = -1;
  aux.y = 0;
  aux.z = 0;  
  aux  = TransRotacao(aux, rotacao,1,0,0);
  aux  = TransRotacao(aux, -inclinacao,0,0,1);
  aux  = TransRotacao(aux, -direcao,0,1,0);
  
  return aux;
}

void Aviao::Atualiza(const double deltaTime){
     
   aPosicao = posicao;
   
   if (explodiu)  return; 
     
   float VelComandos;
    if (tremPouso)
     VelComandos = 2*deltaTime;
   else
    VelComandos = 3*deltaTime;   
 
     
    // abre e fecha trem de pouso ------------- 
     if(!tremPouso){
       if(rotTrem < 90)  
         rotTrem+= 40*deltaTime;           
          if((rotTampas > 0)&&(rotTrem>80))
            rotTampas-= 80*deltaTime;                       
     }else{
        if(rotTampas < 90)  
         rotTampas+= 80*deltaTime;   
                 
          if((rotTrem > 0)&&(rotTampas>70))
            rotTrem-= 40*deltaTime;            
     }
     //------------------------------------------       
     movDirecao = PegaDirec();
    
    
     float VelMovimento = Velocidade*1.4;
     
     if (VelMovimento>200)VelMovimento=200;
         
     rotLeme = rotLeme + (Leme-rotLeme)*VelComandos;
     rotAilerons = rotAilerons + (Ailerons-rotAilerons)*VelComandos;
     rotProfundor = rotProfundor + (Profundor-rotProfundor)*VelComandos;
     rotacao += rotAilerons*deltaTime*VelMovimento*0.014;   
     rotacao -= rotLeme*deltaTime*VelMovimento*0.014*cos(rotacao/57.3)*0.1;  
     rotacao -= rotacao*Diedro*deltaTime*0.5;
     inclinacao += cos(rotacao/57.3)*rotProfundor*deltaTime*VelMovimento*0.005;   
     inclinacao += sin(rotacao/57.3)*rotLeme*deltaTime*VelMovimento*0.002*ValLeme;
     direcao += sin(rotacao/57.3)*sin(rotProfundor/57.3)*deltaTime*VelMovimento*0.2*cos(inclinacao/57.3);
     direcao -= cos(rotacao/57.3)*sin(rotLeme/57.3)*deltaTime*VelMovimento*0.2*cos(inclinacao/57.3)*ValLeme;
     
     direcao += rotacao*Diedro*deltaTime*0.1;

     
     
     
     
      
     if ((Turbina>=Forca-1)&&(Turbina<=Forca+1))
       Forca = Turbina;
     else
     if (Turbina>Forca)
       Forca += deltaTime*20;
     else  
       Forca  -= deltaTime*20; 
   


     Velocidade += (VelMax-Velocidade)*deltaTime*0.0003*Forca*(Potencia/1000);
    // if (Forca<Velocidade)
    //   Velocidade += (Forca-Velocidade)*deltaTime*0.1;
       
     //if (Velocidade > 10000)
     //  Velocidade -= deltaTime*(Velocidade*0.03);// + movDirecao.y*1000); 
       
     if (Velocidade > 0){
        Velocidade -= deltaTime*movDirecao.y*18*(Massa/1000);
        Velocidade -= deltaTime*0.05*Velocidade;
     }
     
     posicao.x += movDirecao.x*Velocidade*deltaTime*1;
     posicao.z += movDirecao.z*Velocidade*deltaTime*1;
     posicao.y += movDirecao.y*Velocidade*deltaTime*1;
     
    // posicao.y += sin(inclinacao/57.3)*Velocidade*deltaTime;
     
     
     if (Velocidade < VelStal){
       Queda += (30-Queda)*(VelStal-Velocidade)*deltaTime*0.001 ;
       inclinacao -= deltaTime*(VelStal-Velocidade)*0.06*(1+movDirecao.y);
     }else 
       Queda *= deltaTime*0.2;  
      
      
      
      
      posicao.y -= Queda*deltaTime ;
     
      Velocidade -= Queda*movDirecao.y*deltaTime;
     
     
     if ((posicao.y<=0)&&(tremPouso))
      voando = false;
      else
      voando = true;  
     
     float veldir;
       
  
    if(posicao.y>7000)  Forca = 0;
   
     if (!voando){         
    
       if ((Queda >25)&&(Queda > movDirecao.y*Velocidade*10))
        Iexplodiu = true;
       else
        Queda = 0; 
        
       if (inclinacao<-8)
          Iexplodiu = true;
          
       if ((rotacao>10)||(rotacao<-10))
         Iexplodiu = true;   
         
         Leme = -Ailerons;
         posicao.y = 0;
         rotacao = 0;
         VelMax = VelMaxchao;
       
          if (freio)
        Velocidade -= deltaTime*0.25*Velocidade;
       
      // VelComandos = 0.5*deltaTime;

       if (inclinacao<0)inclinacao = 0;
        if (inclinacao>15)inclinacao = 15;
        
        GLfloat maxDir = 5/(Velocidade+1);
        
        if (maxDir>1) maxDir = 1;
       
       if (inclinacao<1){
         veldir+=((rotLeme*maxDir)*Velocidade - veldir)*deltaTime*0.1;
         //if (veldir>0.64)veldir = 0.64;
        // else
        // if (veldir<-0.54)veldir = -0.64;
         
         direcao -= veldir;
       }
     }else
     VelMax = VelMaxAR;


  
}

void Aviao::setAilerons(GLfloat v){
  if (v > 45) v = maxComandos;else
  if (v < -45) v = -maxComandos; 
  Ailerons = v;
}

void Aviao::setProfundor(GLfloat v){
  if (v > 45) v = maxComandos;else
  if (v < -45) v = -maxComandos;
  Profundor = v; 
}

void Aviao::setLeme(GLfloat v){
  if (v > 45) v = maxComandos;else
  if (v < -45) v = -maxComandos;
  Leme = v; 
}

void Aviao::setTremDePouso(bool v){
     tremPouso = v;
}

void Aviao::inverteTrem(){
     tremPouso = !tremPouso;
}

void Aviao::setAcelerador(GLfloat v){
     Turbina = v;
     if (Turbina > 100) Turbina = 100;
}

void Aviao::AumentaAcelerador(){
     Turbina += 5;
     if (Turbina > 100) Turbina = 100;
}
void Aviao::DiminuiAcelerador(){
     Turbina -= 5;
     if (Turbina < 0) Turbina = 0;
}

void Aviao::setPosicao(Coord v){
     posicao.x = v.x;
     posicao.y = v.y;
     posicao.z = v.z;
     
     aPosicao = posicao;
     
}

Coord Aviao::getPosicao(){
     return posicao;    
}

void Aviao::getSegmento(Coord p[2]){
   
   p[0]= posicao;
   p[1]= aPosicao;
   
}

void Aviao::setPosicao(GLfloat x, GLfloat y,GLfloat z){
     posicao.x = x;
     posicao.y = y;
     posicao.z = z;     
}

void Aviao::setDirecao(GLfloat v){
     direcao = v;
}
#endif
  
