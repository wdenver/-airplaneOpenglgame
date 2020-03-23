#ifndef _CAMERA_H
#define _CAMERA_H

#include "manipulaVetor.h"
#include "aviao.h"
#include <GL/glut.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cmath>

#define PISTAPOSX  5221
#define PISTAPOSY  12002
#define AVIAOPOSX  5503
#define AVIAOPOSY  12303

class Camera{
public:
  Coord Posicao;
  Coord Alvo;
  Coord MovAlvo;
  Coord Topo;
  GLfloat direct;
  GLfloat inclina;
  GLfloat rotacao;
  GLfloat distancia;
  GLfloat roty;
  GLfloat olhaLados;
  GLfloat olhaCima;
  GLfloat viraLado;
  GLfloat auxT;
  int CameraTipo;
  int atualCameraTipo;
  GLfloat angVisao;
  bool olhaAeroporto;
  Aviao *AviaoCorrente;
  
  Camera(){
    reset();
  }
  
  void reset(){    
   Posicao = GetVetorNulo();
   Alvo = GetVetorNulo();
   MovAlvo = GetVetorNulo();
   Topo =  GetVetorNulo();
   direct  = 0;
   inclina = 0;
   rotacao = 0;
   distancia = 0;
   roty = 0;
   olhaLados = 0;
   olhaCima = 0;
   viraLado = 0;
   auxT = 0;
   CameraTipo = 0;
   atualCameraTipo = 0;
   angVisao = 0;
   olhaAeroporto = false;
  }
  
  
  void atualiza(const GLfloat DeltaTime){
  float vx;
  float vz;
  float vy; 
  float px;
  float pz;
  float py;
 
 
   
  if (viraLado > 100)viraLado=100; 
  if (viraLado < -100)viraLado=-100; 
 
  direct += (AviaoCorrente->direcao - direct)*DeltaTime*0.5;
  inclina += (AviaoCorrente->inclinacao - inclina)*DeltaTime*0.5; 
 
  if (AviaoCorrente->explodiu){
    CameraTipo = 0;
    roty += (45 - roty)*DeltaTime ;   
    distancia+=(300-distancia)*DeltaTime;   
    rotacao += DeltaTime*10;                                                    
  }
  
  if (CameraTipo==0){
          if ( atualCameraTipo!=0){
           rotacao = 0;
           distancia = 45;
           }                          
     
     distancia += (40-distancia)*DeltaTime*0.2;    
     roty += (20-roty)*DeltaTime*0.2;                      
                                
      vx = -cos((direct+rotacao)/57.3);
      vz = sin((direct+rotacao)/57.3); 
      vy = sin((inclina-roty)/57.3);

      
      px=(AviaoCorrente->getPosicao().x-vx*distancia);
      pz=(AviaoCorrente->getPosicao().z-vz*distancia);
      py=(AviaoCorrente->getPosicao().y-vy*distancia);
      
      Posicao.x =  px;
      Posicao.z =  pz;
      Posicao.y =  py;
      
      Alvo.x = AviaoCorrente->getPosicao().x;
      Alvo.y = AviaoCorrente->getPosicao().y;
      Alvo.z = AviaoCorrente->getPosicao().z;
      
      Topo.x = 0;
      Topo.y = 1;
      Topo.z = 0;  
      atualCameraTipo = 0;
  }else
  if (CameraTipo==1){ 
     
      Posicao = AviaoCorrente->PegaPosCockpit();
      Topo = AviaoCorrente->PegaCima(); 
      Coord olha;
      olha.x = -1;
      olha.y = 0;
      olha.z = 0; 
      
      if ( atualCameraTipo!=1){
           olhaLados = 0;
           viraLado = 0;
           olhaCima = 0;
           }
      
      olhaLados += (AviaoCorrente->rotLeme*0.5-olhaLados+viraLado)*DeltaTime*0.5;  
      olhaCima  += (AviaoCorrente->rotProfundor*0.5-olhaCima-8)*DeltaTime*0.5;  
      
      olha = TransRotacao(olha, olhaLados ,0,1,0);
      olha = TransRotacao(olha, -olhaCima ,0,0,1);
      olha = TransRotacao(olha,AviaoCorrente->rotacao,1,0,0);
      olha = TransRotacao(olha,-AviaoCorrente->inclinacao,0,0,1);
      olha = TransRotacao(olha,-AviaoCorrente->direcao,0,1,0);

      if (olhaAeroporto){
          if(auxT<1)
          auxT += DeltaTime*0.1;  
      }else
          if(auxT>0)
         auxT -= DeltaTime*0.2;
         
         if (auxT<0)auxT = 0;
        
      
        MovAlvo.x = AviaoCorrente->getPosicao().x+olha.x*100;
        MovAlvo.y = AviaoCorrente->getPosicao().y+olha.y*100;
        MovAlvo.z = AviaoCorrente->getPosicao().z+olha.z*100;
       
       Alvo.x = MovAlvo.x +(PISTAPOSX - MovAlvo.x)*auxT;
       Alvo.y = MovAlvo.y +(0    - MovAlvo.y)*auxT;
       Alvo.z = MovAlvo.z +(PISTAPOSY - MovAlvo.z)*auxT;
        
       
      Posicao.x =   AviaoCorrente->getPosicao().x+Posicao.x;
      Posicao.y =   AviaoCorrente->getPosicao().y+Posicao.y;
      Posicao.z =   AviaoCorrente->getPosicao().z+Posicao.z;
      atualCameraTipo = 1;
  }
   if (CameraTipo==2){ 
                            
                                  
                                  
      if ( atualCameraTipo!=2){
          vx = -cos((AviaoCorrente->direcao+2)/57.3)*cos((AviaoCorrente->inclinacao+1)/57.3);
          vz = sin((AviaoCorrente->direcao+2)/57.3)*cos((AviaoCorrente->inclinacao+1)/57.3); 
          vy = sin((AviaoCorrente->inclinacao+1)/57.3);
          Posicao.x = AviaoCorrente->getPosicao().x + vx *1000 ;  
          Posicao.y = AviaoCorrente->getPosicao().y + vy *1000 ;
          Posicao.z = AviaoCorrente->getPosicao().z + vz *1000 ;
          atualCameraTipo = 2;        
      }     
      
      vx = (Posicao.x - AviaoCorrente->getPosicao().x); 
      vy = (Posicao.y - AviaoCorrente->getPosicao().y);  
      vz = (Posicao.z - AviaoCorrente->getPosicao().z);      
      
      float dist = sqrt(vx*vx+vz*vz+vy*vy);
      if (dist > 2500)atualCameraTipo = 3; 
      
      angVisao -= dist;              
      
      Alvo.x = AviaoCorrente->getPosicao().x;
      Alvo.y = AviaoCorrente->getPosicao().y;
      Alvo.z = AviaoCorrente->getPosicao().z;
      
      Topo.x = 0;
      Topo.y = 1;
      Topo.z = 0; 
          
  }
}
  
};


#endif
