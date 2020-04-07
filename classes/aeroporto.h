#ifndef _AEROPORTO_
#define _AEROPORTO_

#include <GL/glut.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "Objeto.h"




/**
* Classe que desenha o aeroporto
*/
class Aeroporto{
private:
  Objeto ObjPista;
  Objeto Obj1;
  Objeto ObjT;
  Objeto ObjGrama;
  GLuint TextPista;  
  GLuint TextGrama; 
  GLuint TexObj1; 
  GLuint TexObjT; 
public:  
  Aeroporto(); 
  void desenha();
  void carregar();
  void Atualiza(double deltaTime);
  void Desenha(bool preenchido);
  bool TestaColisao(Coord segmento[2],Coord &ponto);   

};


/**
 * Construtor
 */
Aeroporto::Aeroporto(){
} 



/**
 * testa a colisão de um segmento de reta com este aeroporto
 */
bool Aeroporto::TestaColisao(Coord segmento[2],Coord &ponto){
   
   Coord auxSegmento[2];
   
   auxSegmento[0] = segmento[0];
   auxSegmento[1] = segmento[1];
   
   auxSegmento[0].x -= 5593;
   auxSegmento[0].y -= 0;
   auxSegmento[0].z -= 12467;
   auxSegmento[1].x -= 5593;
   auxSegmento[1].y -= 0;
   auxSegmento[1].z -= 12467;
   
   bool colidiu = false;
    if (ObjT.TestaColisao(auxSegmento,ponto))
      colidiu = true; 
    else
    if (Obj1.TestaColisao(auxSegmento,ponto))
      colidiu = true;   
      
    
   ponto.x += 5593;
   ponto.y += 0;
   ponto.z += 12467;

    
   return colidiu; 
     
}   


/**
 * carrega os vetores e as imagens que compoe o aeroporto
 */
void Aeroporto::carregar(){
  ObjPista.carregaArq("./dados/cenario/aeroporto/pista.obj");
  ObjGrama.carregaArq("./dados/cenario/aeroporto/grama.obj");
  Obj1.carregaArq("./dados/cenario/aeroporto/obj1.obj");
  ObjT.carregaArq("./dados/cenario/aeroporto/objT.obj");
  TextPista= LoadTextura("./dados/cenario/aeroporto/pista.tga"); 
  TextGrama= LoadTextura("./dados/cenario/aeroporto/grama.tga"); 
  TexObj1= LoadTextura("./dados/cenario/aeroporto/obj1.tga"); 
  TexObjT= LoadTextura("./dados/cenario/aeroporto/objT.tga"); 
}



void Aeroporto::Atualiza(double deltaTime){
}


void Aeroporto::Desenha(bool preenchido){ 
     
     
     glColor4f(1,1,1,1);
         
	 
	 glPushMatrix();
     glTranslatef(5593 , 0 , 12467);
     
    glEnable(GL_BLEND);		
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
    glBindTexture(GL_TEXTURE_2D, TextGrama);
    ObjGrama.desenha(preenchido);
    glDisable(GL_BLEND);  
     
     glBindTexture(GL_TEXTURE_2D, TextPista);
     ObjPista.desenha(preenchido);
     
          glBindTexture(GL_TEXTURE_2D, TexObj1);
      Obj1.desenha(preenchido);
   
	
    glDepthMask(GL_FALSE);
    glDisable ( GL_CULL_FACE); 
    glEnable(GL_BLEND);		
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
    glBindTexture(GL_TEXTURE_2D, TexObjT);
    ObjT.desenha(preenchido);
    glDisable(GL_BLEND);  
    glEnable ( GL_CULL_FACE);
    glDepthMask(GL_TRUE);
 
     
  
    glPopMatrix();
}

#endif
  
