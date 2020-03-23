#ifndef _HANGAR_
#define _HANGAR_

#include <GL/glut.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "Objeto.h"





class Hangar{
private:
  Objeto forma1;
  GLuint Textura1;  
  GLuint TexReflexo; 
public:  
  Hangar(); 
  void desenhaPiso();
  void carregar();
  void Atualiza(double deltaTime);
  void Desenha();     

};

Hangar::Hangar(){
} 

void Hangar::desenhaPiso()									
{            
   float tamanho = 40; 
             
   for(int i= -10 ; i < 10 ; i++)   
   for(int j= -10 ; j < 10 ; j++){                                     		
	glBegin(GL_QUADS);								
		glNormal3f(0.0, 1.0, 0.0);	
        			glTexCoord2f(1.0f, 1.0f);				
			glVertex3f( tamanho*i, 0.0, tamanho*j);	
						glTexCoord2f(1.0f, 0.0f);					
			glVertex3f( tamanho*i, 0.0,-tamanho+tamanho*j);	
						glTexCoord2f(0.0f, 0.0f);					
			glVertex3f(-tamanho +tamanho*i, 0.0,-tamanho+tamanho*j);
        
        				
			glTexCoord2f(0.0f, 1.0f);				
			glVertex3f(-tamanho +tamanho*i, 0.0, tamanho*j);							                                         
							                                         
						                                         
		
	glEnd();						
   }     		
}

void Hangar::carregar(){
  Textura1= LoadTextura("./dados/cenario/hangar/piso.tga"); 
  TexReflexo= LoadTextura("./dados/cenario/hangar/reflexo.tga"); 
}
void Hangar::Atualiza(double deltaTime){
}
void Hangar::Desenha(){     
	 glBindTexture(GL_TEXTURE_2D, Textura1);
     desenhaPiso();
}

#endif
  
