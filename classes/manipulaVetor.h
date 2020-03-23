#ifndef _MANIPV_
#define _MANIPV_

#include <cmath>
#include <GL/glut.h>
#include <iostream>
#define MENOR_NUMERO  0.00000001

using namespace std;

struct Coord{
   GLfloat x, y, z;
};


Coord GetVetorNulo(){
  Coord aux;
  aux.x = 0;
  aux.y = 0;
  aux.z = 0;
  return aux;    
}

GLfloat DistanciaPontos(Coord p1, Coord p2){
    GLfloat V1 = p1.x - p2.x;  
    GLfloat V2 = p1.y - p2.y; 
    GLfloat V3 = p1.z - p2.z;   
    return sqrt(V1*V1+V2*V2+V3*V3);     
}


Coord ProdutoVetorial(Coord p1,Coord p2){
 Coord aux;
	aux.x = p1.y * p2.z - p1.z * p2.y;
    aux.y = p1.z * p2.x - p1.x * p2.z;
	aux.z = p1.x * p2.y - p1.y * p2.x;
  return aux;        
}

Coord ProdutoPorEscalar(GLfloat u,Coord p1){
 Coord aux;
	aux.x = p1.x *u;
    aux.y = p1.y *u;
	aux.z = p1.z *u;
  return aux;        
}

float ProdutoEscalar(Coord p1,Coord p2){
  return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;        
}

Coord SubtraiVetores(Coord p1,Coord p2){
   Coord aux;
	aux.x = p1.x-p2.x;
    aux.y = p1.y-p2.y;
	aux.z = p1.z-p2.z;
  return aux;       
}


Coord SomaVetores(Coord p1,Coord p2){
   Coord aux;
	aux.x = p1.x+p2.x;
    aux.y = p1.y+p2.y;
	aux.z = p1.z+p2.z;
  return aux;       
}

bool VetorNulo(Coord p1){

 return ((p1.x== 0)&&(p1.y== 0)&& (p1.z== 0));      
     
}



Coord TransRotacao(Coord vet , GLfloat ang , int x , int y, int z){
  
 Coord aux;
 ang = ang/57.3;
 
 if (x==1){
   aux.x = vet.x;
   aux.y = cos(ang)*vet.y - sin(ang)*vet.z;
   aux.z = sin(ang)*vet.y + cos(ang)*vet.z;
 }
 
 if (y==1){
   aux.x = cos(ang)*vet.x - sin(ang)*vet.z;
   aux.y = vet.y;
   aux.z = sin(ang)*vet.x + cos(ang)*vet.z;
 }
 
 if (z==1){
   aux.x = cos(ang)*vet.x - sin(ang)*vet.y;
   aux.y = sin(ang)*vet.x + cos(ang)*vet.y;
   aux.z = vet.z;
 }
     
     return aux; 
      
}



int interceptaTriangulo( Coord R[], Coord T[], Coord &I )
{
    Coord    u, v, n;             // Vetor dos triangulos
    Coord    dir, w0, w;          // vetor do raio
    float     r, a, b;            // paramentro para calculo de intersecção


    // faz os vetores para o vértice do triangulo
    u = SubtraiVetores(T[1],T[0]);   
    v = SubtraiVetores(T[2],T[0]);
    
    n = ProdutoVetorial(u,v);
   
  
     // testa se o vetor é nulo
      if ( VetorNulo(n)){
        return -1;                
      }
      
    // faz o vetor de direcao do segmento de reta
    dir = SubtraiVetores(R[1],R[0]);
    
    w0 = SubtraiVetores(R[0],T[0]);
    
    a = -ProdutoEscalar(n,w0);
    
    b = ProdutoEscalar(n,dir);
        
    if (fabs(b) < MENOR_NUMERO) {   
        if (a == 0)               
            return 2;
        else return 0;          
    }

    r = a / b;
    if (r < 0.0){                  
        return 0;                  
        }               

    // Ponto de interseção com o plano
    I = SomaVetores(R[0],ProdutoPorEscalar(r,dir));
    
    if((I.x < R[0].x)&&(I.x < R[1].x)) return 0;
    if((I.x > R[0].x)&&(I.x > R[1].x)) return 0;
    
    if((I.y < R[0].y)&&(I.y < R[1].y)) return 0;
    if((I.y > R[0].y)&&(I.y > R[1].y)) return 0;
    
    if((I.z < R[0].z)&&(I.z < R[1].z)) return 0;
    if((I.z > R[0].z)&&(I.z > R[1].z)) return 0;
    

    // testa se o ponto está dentro do triangulo
    float    uu, uv, vv, wu, wv, D;
    uu = ProdutoEscalar(u,u);
    uv = ProdutoEscalar(u,v);
    vv = ProdutoEscalar(v,v);
    w = SubtraiVetores(I,T[0]);  
    wu = ProdutoEscalar(w,u);
    wv = ProdutoEscalar(w,v);
    D = uv * uv - uu * vv;

    float s, t;
    s = (uv * wv - vv * wu) / D;
    if ((s < 0.0) || (s > 1.0))        // não está dentro
        return 0;
    t = (uv * wu - uu * wv) / D;
    if ((t < 0.0) || (s + t) > 1.0)   // não está dentro
        return 0;
        

    return 1;                      // está dentro
}

#endif
