#include "classes/Objeto.h"
#include "classes/textura.h"
#include "classes/f16.h"
#include "classes/a10.h"
#include "classes/hangar.h"
#include "classes/relevo.h"
#include "classes/aeroporto.h"
#include "classes/Particula.h"
#include "classes/fumaca.h"
#include "classes/Missil.h"
#include "classes/camera.h"
//#include <windows.h>
#include <stdio.h>
//#include <gl/gl.h>
//#include <gl/glu.h>

#define PISTAPOSX 5221
#define PISTAPOSY 12002
#define AVIAOPOSX 5503
#define AVIAOPOSY 12303

#include <GL/glut.h>
#include <cstdlib>
#include <iostream>

using namespace std;

struct ListaAvioes
{
  Coord posicao;
  GLfloat rotacao;
  Aviao *tipo;
};

Camera observador1;
Camera observador2;
Camera *ObCorrente = &observador1;

bool teclas[256];

int main(int argc, char *argv[]);
void keyboard_input(unsigned char key, int x, int y);
void keyboard_Up_input(unsigned char key, int x, int y);
void GerenciaMouse(int button, int state, int x, int y);
void display(void);
void idle(void);
void menu(int);
void myInit(void);
void myReshape(int, int);
void PreparaHangar();
void PreparaVoar();

//aberto
GLfloat LightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat LightDiffuse[] = {0.9f, 0.85f, 0.8f, 1.0f};
GLfloat LightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat LightPosition[] = {20.0f, 10000.0f, 20.0f};

// garagem
GLfloat LightAmbient2[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat LightDiffuse2[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat LightSpecular2[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat LightPosition2[] = {20.0f, 10000.0f, 20.0f};

GLuint textAeroporto;

GLuint skyTopo;
GLuint skyEsquerda;
GLuint skyDireita;
GLuint skyTraz;
GLuint skyFrente;

GLfloat fogColor[4] = {0.95f, 0.95f, 0.95f, 1.0f};
GLfloat fogColor2[4] = {0.0f, 0.0f, 0.0f, 1.0f};

Missil misseis[8];
int TotalMisseis = 8;

F16 f16;
A10 a10;
Aviao *AviaoCorrente;
Hangar hangar;
Relevo relevo;
ControlaParticulas controlaParticula;

int TelaLargura, TelaAltura;
Aeroporto aeroporto;
GLUquadricObj *q;
int ultimoTime;
float rotacao = 0;
bool tela = 0;
GLfloat angX, angY;
GLfloat clicX = 0, clicY = 0, clicZ = 0;
ListaAvioes listaAvioes[1000];
int contListaAvioes = 0;
bool preenchido = true;
float DeltaTime = 0;
GLfloat Raltura;
char texto[120];

//****************************************************************************80

void DesenhaTexto(const char *string, void *font, float x, float y)
{
  glDisable(GL_TEXTURE_2D);
  glPushMatrix();
  glRasterPos2f(x, y);
  while (*string)
    glutBitmapCharacter(font, *string++);
  glPopMatrix();
  glEnable(GL_TEXTURE_2D);
}

void DesenhaTelaControles()
{
  glDepthFunc(GL_ALWAYS);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glDisable(GL_LIGHTING);
  glLoadIdentity();
  glDisable(GL_TEXTURE_2D);

  glColor4f(0, 0, 0, 0.4);
  glBegin(GL_QUADS);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, 0.0f);
  glEnd();

  glColor4f(0, 0, 0, 0.6);
  glBegin(GL_LINE_STRIP);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, 0.0f);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glEnd();

  glColor4f(1.0, 1.0, 1.0, 0.9);
  DesenhaTexto("Controles", GLUT_BITMAP_HELVETICA_12, -0.9, 0.90);
  glColor4f(1, 1, 0, 0.6);
  DesenhaTexto("A", GLUT_BITMAP_HELVETICA_12, -0.8, 0.75);
  DesenhaTexto("D", GLUT_BITMAP_HELVETICA_12, -0.8, 0.63);
  DesenhaTexto("W", GLUT_BITMAP_HELVETICA_12, -0.8, 0.51);
  DesenhaTexto("S", GLUT_BITMAP_HELVETICA_12, -0.8, 0.39);
  DesenhaTexto("Q", GLUT_BITMAP_HELVETICA_12, -0.8, 0.27);
  DesenhaTexto("E", GLUT_BITMAP_HELVETICA_12, -0.8, 0.15);
  DesenhaTexto("B", GLUT_BITMAP_HELVETICA_12, -0.8, 0.03);
  DesenhaTexto("G", GLUT_BITMAP_HELVETICA_12, -0.8, -0.09);
  DesenhaTexto("L", GLUT_BITMAP_HELVETICA_12, -0.8, -0.21);
  DesenhaTexto("[ ]", GLUT_BITMAP_HELVETICA_12, -0.8, -0.33);
  DesenhaTexto("5,6,7,8,9,0", GLUT_BITMAP_HELVETICA_12, -0.8, -0.45);
  DesenhaTexto("1,2,3", GLUT_BITMAP_HELVETICA_12, -0.8, -0.57);
  DesenhaTexto("Espaco", GLUT_BITMAP_HELVETICA_12, -0.8, -0.69);
  DesenhaTexto("Esc", GLUT_BITMAP_HELVETICA_12, -0.8, -0.81);
  DesenhaTexto("Enter", GLUT_BITMAP_HELVETICA_12, -0.8, -0.93);
  glColor4f(1, 1, 1, 0.6);
  DesenhaTexto("Ailerons Esquerda", GLUT_BITMAP_HELVETICA_12, -0.3, 0.75);
  DesenhaTexto("Ailerons Direita", GLUT_BITMAP_HELVETICA_12, -0.3, 0.63);
  DesenhaTexto("Profundor Desce", GLUT_BITMAP_HELVETICA_12, -0.3, 0.51);
  DesenhaTexto("Profundor Sobe", GLUT_BITMAP_HELVETICA_12, -0.3, 0.39);
  DesenhaTexto("Leme Esquerda", GLUT_BITMAP_HELVETICA_12, -0.3, 0.27);
  DesenhaTexto("Leme Direita", GLUT_BITMAP_HELVETICA_12, -0.3, 0.15);
  DesenhaTexto("Rodas Freio", GLUT_BITMAP_HELVETICA_12, -0.3, 0.03);
  DesenhaTexto("Trem de Pouso", GLUT_BITMAP_HELVETICA_12, -0.3, -0.09);
  DesenhaTexto("Olha Aeroporto", GLUT_BITMAP_HELVETICA_12, -0.3, -0.21);
  DesenhaTexto("Gira Camera", GLUT_BITMAP_HELVETICA_12, -0.3, -0.33);
  DesenhaTexto("Motor", GLUT_BITMAP_HELVETICA_12, -0.0, -0.45);
  DesenhaTexto("Cameras", GLUT_BITMAP_HELVETICA_12, -0.0, -0.57);
  DesenhaTexto("Missil", GLUT_BITMAP_HELVETICA_12, -0.0, -0.69);
  DesenhaTexto("Volta p/ Menu", GLUT_BITMAP_HELVETICA_12, -0.0, -0.81);
  DesenhaTexto("Reseta", GLUT_BITMAP_HELVETICA_12, -0.0, -0.93);
  glDisable(GL_BLEND);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_TEXTURE_2D);
}

void DesenhaTelaInfoHangar()
{
  glDepthFunc(GL_ALWAYS);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glDisable(GL_LIGHTING);
  glLoadIdentity();
  glDisable(GL_TEXTURE_2D);

  glColor4f(0, 0, 0, 0.2);
  glBegin(GL_QUADS);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, 0.0f);
  glEnd();

  glColor4f(0, 0, 0, 0.6);
  glBegin(GL_LINE_STRIP);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, 0.0f);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glEnd();

  glColor4f(1.0, 1.0, 1.0, 0.9);
  DesenhaTexto("Comandos", GLUT_BITMAP_HELVETICA_12, -0.9f, 0.80f);
  glColor4f(1, 1, 0, 0.6);
  DesenhaTexto("TAB", GLUT_BITMAP_HELVETICA_12, -0.8f, 0.45f);
  DesenhaTexto("ENTER", GLUT_BITMAP_HELVETICA_12, -0.8f, 0.25f);
  DesenhaTexto("ESC", GLUT_BITMAP_HELVETICA_12, -0.8f, 0.05f);
  DesenhaTexto("[ ]", GLUT_BITMAP_HELVETICA_12, -0.8f, -0.15f);
  glColor4f(1.0, 1.0, 1.0, 0.6);
  DesenhaTexto("Seleciona aviao", GLUT_BITMAP_HELVETICA_12, -0.2f, 0.45f);
  DesenhaTexto("Voar", GLUT_BITMAP_HELVETICA_12, -0.2f, 0.25f);
  DesenhaTexto("Reset", GLUT_BITMAP_HELVETICA_12, -0.2f, 0.05f);
  DesenhaTexto("Gira camera", GLUT_BITMAP_HELVETICA_12, -0.2f, -0.15f);
  DesenhaTexto("clique no piso para adicionar", GLUT_BITMAP_HELVETICA_12, -0.9f, -0.55f);
  DesenhaTexto("avioes na tela", GLUT_BITMAP_HELVETICA_12, -0.9f, -0.70f);
  glDisable(GL_BLEND);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_TEXTURE_2D);
}

void DesenhaInfo()
{

  glDepthFunc(GL_ALWAYS);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glDisable(GL_LIGHTING);
  glLoadIdentity();
  glDisable(GL_TEXTURE_2D);

  glColor4f(0, 0, 0, 0.4);
  glBegin(GL_QUADS);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, 0.0f);
  glEnd();

  glColor4f(0, 0, 0, 0.6);
  glBegin(GL_LINE_STRIP);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, 0.0f);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glEnd();

  glColor4f(1, 1, 1, 0.9);
  DesenhaTexto("Info", GLUT_BITMAP_HELVETICA_12, -0.9f, 0.75f);
  glColor4f(1, 1, 1, 0.6);
  DesenhaTexto("Velocidade", GLUT_BITMAP_HELVETICA_12, -0.8f, 0.45f);
  DesenhaTexto("Altitude", GLUT_BITMAP_HELVETICA_12, -0.8f, 0.15f);
  DesenhaTexto("Motor", GLUT_BITMAP_HELVETICA_12, -0.8f, -0.15f);
  DesenhaTexto("Misseis", GLUT_BITMAP_HELVETICA_12, -0.8f, -0.45f);
  DesenhaTexto("Rodas", GLUT_BITMAP_HELVETICA_12, -0.8f, -0.75f);
  glColor4f(1, 1, 0, 0.6);
  sprintf(texto, "%d Km/h", (int)trunc(AviaoCorrente->Velocidade));
  DesenhaTexto(texto, GLUT_BITMAP_HELVETICA_12, 0, 0.45f);
  sprintf(texto, "%d m", (int)trunc(AviaoCorrente->getPosicao().y / 5));
  DesenhaTexto(texto, GLUT_BITMAP_HELVETICA_12, 0, 0.15f);
  sprintf(texto, "%d %%", (int)trunc(AviaoCorrente->Forca));
  DesenhaTexto(texto, GLUT_BITMAP_HELVETICA_12, 0, -0.15f);
  sprintf(texto, "%d unit", (int)trunc(AviaoCorrente->missilAtivo));
  DesenhaTexto(texto, GLUT_BITMAP_HELVETICA_12, 0, -0.45f);
  if (AviaoCorrente->tremPouso)
    DesenhaTexto("Baixo", GLUT_BITMAP_HELVETICA_12, 0, -0.75f);
  else
    DesenhaTexto("Alto", GLUT_BITMAP_HELVETICA_12, 0, -0.75f);
  glDisable(GL_BLEND);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_TEXTURE_2D);
}

void DesenhaRadar()
{
  glDepthFunc(GL_ALWAYS);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glDisable(GL_LIGHTING);
  glLoadIdentity();
  glDisable(GL_TEXTURE_2D);
  glColor4f(0, 0.3, 0, 0.5);
  glBegin(GL_QUADS);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, 0.0f);
  glEnd();

  glColor4f(1, 1, 1, 0.6);
  glBegin(GL_LINE_STRIP);
  glVertex3f(0.00, 0.10f, 0.0f);
  glVertex3f(0.02, 0.05f, 0.0f);
  glVertex3f(0.10, 0.00f, 0.0f);
  glVertex3f(0.02, 0.00f, 0.0f);
  glVertex3f(0.02, -0.08f, 0.0f);
  glVertex3f(0.06, -0.10f, 0.0f);
  glVertex3f(-0.06, -0.10f, 0.0f);
  glVertex3f(-0.02, -0.08f, 0.0f);
  glVertex3f(-0.02, 0.00f, 0.0f);
  glVertex3f(-0.10, 0.00f, 0.0f);
  glVertex3f(-0.02, 0.05f, 0.0f);
  glVertex3f(0.0, 0.1f, 0.0f);
  glEnd();

  glPushMatrix();
  glRotatef(-AviaoCorrente->direcao, 0, 0, 1);
  glTranslatef(AviaoCorrente->getPosicao().z / 30000.0, AviaoCorrente->getPosicao().x / 30000.0, 0);
  glColor4f(0.3, 0.7, 0.3, 0.3);
  glBegin(GL_LINES);
  for (int i = -100; i < 100; i++)
  {
    glVertex3f(i / 10.0, 10.0f, 0.0f);
    glVertex3f(i / 10.0, -10.0f, 0.0f);
    glVertex3f(10.0f, i / 10.0, 0.0f);
    glVertex3f(-10.0f, i / 10.0, 0.0f);
  }
  glEnd();

  glPushMatrix();
  glTranslatef(-AVIAOPOSY / 30000.0, -AVIAOPOSX / 30000.0, 0);
  glColor4f(0.8, 0.8, 0.3, 0.8);
  glBegin(GL_LINE_STRIP);
  glVertex3f(-0.10, 0.05f, 0.0f);
  glVertex3f(0.10, 0.05f, 0.0f);
  glVertex3f(0.10, -0.05f, 0.0f);
  glVertex3f(-0.10, -0.05f, 0.0f);
  glVertex3f(-0.10, 0.05f, 0.0f);
  glEnd();
  glPopMatrix();

  for (int k = 0; k < TotalMisseis; k++)
  {
    if (!misseis[k].ativo)
      continue;
    if (!misseis[k].liberado)
      continue;
    glPushMatrix();
    glTranslatef(-misseis[k].getPosicao().z / 30000.0f, -misseis[k].getPosicao().x / 30000.0f, 0);
    if (misseis[k].explodiu)
    {
      glColor4f(0.8, 0.0, 0.0, 0.8);
      glBegin(GL_LINES);
      glVertex3f(-0.03, 0.03f, 0.0f);
      glVertex3f(0.03, -0.03f, 0.0f);
      glVertex3f(0.03, 0.03f, 0.0f);
      glVertex3f(-0.03, -0.03f, 0.0f);
      glEnd();
    }
    else
    {
      glColor4f(0.8, 0.8, 0.8, 0.8);
      glBegin(GL_LINE_STRIP);
      glVertex3f(-0.03, 0.03f, 0.0f);
      glVertex3f(0.03, 0.03f, 0.0f);
      glVertex3f(0.03, -0.03f, 0.0f);
      glVertex3f(-0.03, -0.03f, 0.0f);
      glVertex3f(-0.03, 0.03f, 0.0f);
      glEnd();
    }
    glPopMatrix();
  }

  glPopMatrix();

  glColor4f(0, 0, 0, 0.6);
  glBegin(GL_LINE_STRIP);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, 0.0f);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glEnd();

  glColor4f(0.7, 1.0, 0.7, 0.8);
  DesenhaTexto("Radar", GLUT_BITMAP_HELVETICA_12, -0.9f, 0.85f);

  glDisable(GL_BLEND);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_TEXTURE_2D);
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1024, 700);
  glutCreateWindow("Trabalho de Computa��o Gr�fica - Waldir Denver");
  glutKeyboardFunc(keyboard_input);
  glutKeyboardUpFunc(keyboard_Up_input);
  glutMouseFunc(GerenciaMouse);
  glutReshapeFunc(myReshape);
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  myInit();
  glutMainLoop();
  return 0;
}

bool TestaColisaoCenario(Coord segmento[2], Coord &ponto)
{
  // bool saida = false;

  if (aeroporto.TestaColisao(segmento, ponto))
    return true;

  if (relevo.TestaColisao(segmento, ponto))
    return true;

  return false;
}

void AtualizaVoo()
{
  //bool auxb;
  Raltura = relevo.PegaAltura(AviaoCorrente->getPosicao().x, AviaoCorrente->getPosicao().z);

  Coord p[2];
  Coord s;
  AviaoCorrente->getSegmento(p);
  p[0].y += 2;
  p[1].y += 2;
  if (TestaColisaoCenario(p, s))
  {
    AviaoCorrente->explodiu = true;
    controlaParticula.GerarExplosao(s.x, s.y, s.z);
    controlaParticula.AdicinaQueimador(s, 1);
    AviaoCorrente->setPosicao(s);
  }

  if ((AviaoCorrente->Iexplodiu) && (!AviaoCorrente->explodiu))
  {
    AviaoCorrente->explodiu = true;
    s = AviaoCorrente->getPosicao();
    controlaParticula.GerarExplosao(s.x, s.y, s.z);
    controlaParticula.AdicinaQueimador(s, 1);
  }

  for (int k = 0; k < TotalMisseis; k++)
  {
    if (!misseis[k].ativo)
      continue;
    misseis[k].Atualiza(DeltaTime);
    misseis[k].getSegmento(p);
    if (TestaColisaoCenario(p, s))
    {
      misseis[k].explodiu = true;
      controlaParticula.GerarExplosao(s.x, s.y, s.z);
      controlaParticula.AdicinaQueimador(s, 1);
    }
  }
}

void keyboard_Up_input(unsigned char key, int x, int y)
{
  teclas[key] = false;
  //cout<< (int)key;
}

void keyboard_input(unsigned char key, int x, int y)
{
  teclas[key] = true;
  teclas[key - 'A' + 'a'] = true;

  //cout<< (int)key;
  switch (key)
  {
  case '[':
    ObCorrente->rotacao -= 5;
    ObCorrente->viraLado -= 5;
    break;
  case ']':
    ObCorrente->rotacao += 5;
    ObCorrente->viraLado += 5;
    break;
  case 'g':
    if (tela == 1)
      AviaoCorrente->inverteTrem();
    break;
  case 27:
    PreparaHangar();
    break;
  case 13:
    PreparaVoar();
    break;
  case 'p':
    preenchido = !preenchido;
    cout << "x: " << AviaoCorrente->getPosicao().x;
    cout << "  y: " << AviaoCorrente->getPosicao().y;
    cout << "  z: " << AviaoCorrente->getPosicao().z << endl;
    break;
  case 9:
    if (tela == 0)
    {
      if (AviaoCorrente == &a10)
        AviaoCorrente = &f16;
      else
        AviaoCorrente = &a10;
    }

    break;
  case '1':
    ObCorrente->CameraTipo = 0;
    break;
  case '2':
    ObCorrente->CameraTipo = 1;
    break;
  case '3':
    if (tela == 1)
      ObCorrente->CameraTipo = 2;
    break;
  case '5':
    AviaoCorrente->setAcelerador(0);
    break;
  case '6':
    AviaoCorrente->setAcelerador(20);
    break;
  case '7':
    AviaoCorrente->setAcelerador(40);
    break;
  case '8':
    AviaoCorrente->setAcelerador(60);
    break;
  case '9':
    AviaoCorrente->setAcelerador(80);
    break;
  case '0':
    AviaoCorrente->setAcelerador(100);
    break;
  case 32:
    int IDmissil = AviaoCorrente->selecionaMissil();

    if (IDmissil >= 0)
      misseis[IDmissil].Liberar();
    break;
  }
}

void DesenhaCeu()
{
  float Tamanho = 1000;
  float Pbaixo = -0.9 - 0.00025 * ObCorrente->Posicao.y;

  glPushMatrix();
  glTranslatef(ObCorrente->Posicao.x, ObCorrente->Posicao.y, ObCorrente->Posicao.z);
  glScalef(Tamanho, Tamanho / 2, Tamanho);
  glColor4f(1, 1, 1, 1);

  // frente
  glBindTexture(GL_TEXTURE_2D, skyFrente);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, 1.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(1.0f, Pbaixo, 1.0f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-1.0f, Pbaixo, 1.0f);
  glEnd();

  // traz
  glBindTexture(GL_TEXTURE_2D, skyTraz);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(1.0f, Pbaixo, -1.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, -1.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-1.0f, Pbaixo, -1.0f);
  glEnd();

  // Topo
  glBindTexture(GL_TEXTURE_2D, skyTopo);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(1.0f, 1.0f, -1.0f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, 1.0f);
  glEnd();

  // direita
  glBindTexture(GL_TEXTURE_2D, skyDireita);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(1.0f, Pbaixo, 1.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, -1.0f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(1.0f, Pbaixo, -1.0f);
  glEnd();

  // esquerda
  glBindTexture(GL_TEXTURE_2D, skyEsquerda);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-1.0f, 1.0f, 1.0f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-1.0f, Pbaixo, 1.0f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-1.0f, Pbaixo, -1.0f);
  glEnd();
  glPopMatrix();
}

void DesenhaAvioes()
{
  glEnable(GL_LIGHTING);
  f16.Desenha();
  a10.Desenha();
  for (int i = 0; i < contListaAvioes; i++)
  {
    glPushMatrix();
    glTranslatef(listaAvioes[i].posicao.x, listaAvioes[i].posicao.y, listaAvioes[i].posicao.z);
    glTranslatef(-listaAvioes[i].tipo->getPosicao().x, -listaAvioes[i].tipo->getPosicao().y, -listaAvioes[i].tipo->getPosicao().z);
    listaAvioes[i].tipo->Desenha();
    glPopMatrix();
  }
}

void DesenhaSombras(GLfloat altura)
{
  a10.DesenhaSombra(altura);
  f16.DesenhaSombra(altura);
  for (int i = 0; i < contListaAvioes; i++)
  {
    glPushMatrix();
    glTranslatef(listaAvioes[i].posicao.x, listaAvioes[i].posicao.y, listaAvioes[i].posicao.z);
    glTranslatef(-listaAvioes[i].tipo->getPosicao().x, -listaAvioes[i].tipo->getPosicao().y, -listaAvioes[i].tipo->getPosicao().z);
    listaAvioes[i].tipo->DesenhaSombra(altura);
    glPopMatrix();
  }
}

void DesenhaVoando()
{

  LightPosition[0] = AviaoCorrente->getPosicao().x;
  LightPosition[1] = AviaoCorrente->getPosicao().y + 10000;
  LightPosition[2] = AviaoCorrente->getPosicao().z;
  glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

  if (preenchido)
  {
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    DesenhaCeu();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FOG);
  }

  glPushMatrix();
  glTranslatef(0, -1.0, 0);
  relevo.desenha(preenchido, AviaoCorrente->getPosicao().x, AviaoCorrente->getPosicao().z, 0);
  glPopMatrix();

  glEnable(GL_LIGHTING);
  aeroporto.Desenha(preenchido);
  AviaoCorrente->Desenha();

  for (int i = 0; i < TotalMisseis; i++)
    misseis[i].Desenha();

  controlaParticula.DesenhaParticulas();
  DesenhaSombras(Raltura + 0.2);

  


  //-------- debug de colisão ------------------
    // Coord v[4];
    // relevo.PegaQuadro(v,AviaoCorrente->getPosicao());
    // GLfloat alt = relevo.PegaAltura(AviaoCorrente->getPosicao().x,AviaoCorrente->getPosicao().z ) ;   
    // glPushMatrix();
    // glTranslatef(AviaoCorrente->getPosicao().x,alt ,AviaoCorrente->getPosicao().z);
    // glutWireSphere(10.0, 12, 12);
    // glPopMatrix();    
   
    // glColor3f(0,1,0);
    // glDisable(GL_LIGHTING); 
    //  glBegin(GL_LINE_STRIP);
    //   glVertex3f(v[0].x,v[0].y,v[0].z);
    //   glVertex3f(v[1].x,v[1].y,v[1].z);   
    //   glVertex3f(v[2].x,v[2].y,v[2].z);
    //   glVertex3f(v[0].x,v[0].y,v[0].z);    
    //   glVertex3f(v[3].x,v[3].y,v[3].z);
    //   glVertex3f(v[2].x,v[2].y,v[2].z);
    //   glVertex3f(v[0].x,v[0].y,v[0].z);
    //  glEnd(); 
		     
        				
    //  glBegin(GL_LINES);
    //   glVertex3f(AviaoCorrente->getPosicao().x,alt ,AviaoCorrente->getPosicao().z);
    //   glVertex3f(AviaoCorrente->getPosicao().x,AviaoCorrente->getPosicao().y,AviaoCorrente->getPosicao()Z.z);   
    //   glVertex3f(AviaoCorrente->getPosicao().x,alt ,AviaoCorrente->getPosicao().z);
    //   glVertex3f(v[0].x,v[0].y,v[0].z);    
    //   glVertex3f(AviaoCorrente->getPosicao().x,alt ,AviaoCorrente->getPosicao().z);
    //   glVertex3f(v[1].x,v[1].y,v[1].z);   
    //   glVertex3f(AviaoCorrente->getPosicao().x,alt ,AviaoCorrente->getPosicao().z);
    //   glVertex3f(v[2].x,v[2].y,v[2].z);     
    //   glVertex3f(AviaoCorrente->getPosicao().x,alt ,AviaoCorrente->getPosicao().z);
    //   glVertex3f(v[3].x,v[3].y,v[3].z);     
    //  glEnd();
  //----------------------------------------------------
    
}

void DesenhaHangar()
{

  double eqr[] = {0.0f, -1.0f, 0.0f, 0.0f};

  glColorMask(0, 0, 0, 0);
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 1, 1);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  glDisable(GL_DEPTH_TEST);
  hangar.Desenha();
  glEnable(GL_DEPTH_TEST);
  glColorMask(1, 1, 1, 1);
  glStencilFunc(GL_EQUAL, 1, 1);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glEnable(GL_CLIP_PLANE0);
  glClipPlane(GL_CLIP_PLANE0, eqr);
  glDisable(GL_CULL_FACE);
  glPushMatrix();
  glScalef(1.0f, -1.0f, 1.0f);
  glLightfv(GL_LIGHT0, GL_POSITION, LightPosition2);
  glTranslatef(0.0f, 0, 0.0f);
  DesenhaAvioes();
  glPopMatrix();
  glEnable(GL_CULL_FACE);
  glDisable(GL_CLIP_PLANE0);
  glDisable(GL_STENCIL_TEST);
  glLightfv(GL_LIGHT0, GL_POSITION, LightPosition2);
  glEnable(GL_BLEND);
  glDisable(GL_LIGHTING);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  hangar.Desenha();
  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
  DesenhaSombras(0.0);
  DesenhaAvioes();
}

void transformaObservador(Camera ob)
{
  glLoadIdentity();
  gluLookAt(ob.Posicao.x, ob.Posicao.y, ob.Posicao.z,
            ob.Alvo.x, ob.Alvo.y, ob.Alvo.z,
            ob.Topo.x, ob.Topo.y, ob.Topo.z);
}

void display(void)
{

  glViewport(0, 0, TelaLargura, TelaAltura);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70, (float)TelaLargura / TelaAltura, 1, 16000);
  glMatrixMode(GL_MODELVIEW);
  transformaObservador(observador1);
  if (tela == 0)
  {
    DesenhaHangar();
  }
  else if (tela == 1)
  {
    DesenhaVoando();
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (tela == 1)
  {
    glViewport(TelaLargura - 200, TelaAltura - 200, 190, 190);
    DesenhaRadar();

    glViewport(TelaLargura - 200, TelaAltura - 490, 190, 120);
    DesenhaInfo();

    glViewport(TelaLargura - 200, TelaAltura - 690, 190, 190);
    DesenhaTelaControles();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, 1, 1, 16000);
    glMatrixMode(GL_MODELVIEW);
    ObCorrente = &observador2;
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    transformaObservador(observador2);
    glViewport(TelaLargura - 200, TelaAltura - 360, 190, 150);
    DesenhaVoando();
    ObCorrente = &observador1;
  }
  else
  {
    glViewport(10, 30, 190, 190);
    DesenhaTelaInfoHangar();
  }

  //glColorMask(0,0,0,0);

  glutSwapBuffers();
}

void PreparaHangar()
{
  glLoadIdentity();
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glFogfv(GL_FOG_COLOR, fogColor2);
  glFogf(GL_FOG_DENSITY, 0.8f);
  glHint(GL_FOG_HINT, GL_DONT_CARE);
  glFogf(GL_FOG_START, 100.0f);
  glFogf(GL_FOG_END, 250.0f);
  glEnable(GL_FOG);

  glDisable(GL_LIGHT1);
  glEnable(GL_LIGHT0);

  ObCorrente->reset();
  ObCorrente->distancia = 30;
  ObCorrente->distancia = 10;
  ObCorrente->roty = 90;

  contListaAvioes = 0;

  AviaoCorrente = &a10;
  AviaoCorrente->ResetaMovimentos();
  AviaoCorrente->setPosicao(0, 0, 0);
  AviaoCorrente->setAcelerador(0);
  AviaoCorrente->direcao = 0;

  AviaoCorrente = &f16;
  AviaoCorrente->ResetaMovimentos();
  AviaoCorrente->setPosicao(0, 0, 50);
  AviaoCorrente->setAcelerador(0);
  AviaoCorrente->direcao = 0;

  tela = 0;
}

void PreparaVoar()
{
  AviaoCorrente->ResetaMovimentos();
  AviaoCorrente->setPosicao(AVIAOPOSX, 0, AVIAOPOSY);
  AviaoCorrente->setAcelerador(0);
  AviaoCorrente->direcao = -90;
  AviaoCorrente->rotTrem = 0;

  for (int i = 0; i < 8; i++)
    misseis[i].ativo = false;

  TotalMisseis = AviaoCorrente->numMissieis;
  for (int i = 0; i < TotalMisseis; i++)
  {
    misseis[i].AviaoSuporte = AviaoCorrente;
    misseis[i].ResetaMovimentos();
    misseis[i].ativo = true;
  }

  ObCorrente->reset();
  ObCorrente->distancia = 90;
  ObCorrente->roty = 90;
  ObCorrente->direct = -45;
  controlaParticula.Reset();

  glLoadIdentity();
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glFogfv(GL_FOG_COLOR, fogColor);
  glFogf(GL_FOG_DENSITY, 0.8f);
  glHint(GL_FOG_HINT, GL_DONT_CARE);
  glFogf(GL_FOG_START, 200.0f);
  glFogf(GL_FOG_END, 14000.0f);
  glEnable(GL_FOG);

  glDisable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  tela = 1;
  observador2.CameraTipo = 2;
}

void idle(void)
{
  int time;

  observador1.AviaoCorrente = AviaoCorrente;
  observador2.AviaoCorrente = AviaoCorrente;
  if (observador1.CameraTipo != 0)
    observador2.CameraTipo = 0;
  else
    observador2.CameraTipo = 2;

  time = glutGet(GLUT_ELAPSED_TIME);
  DeltaTime += (((time - ultimoTime) / 500.0) - DeltaTime) * 0.1;
  if (DeltaTime > 0.9)
    DeltaTime = 0.9;

  if (tela == 0)
    ObCorrente->rotacao += DeltaTime * 10;

  if (teclas['l'])
    ObCorrente->olhaAeroporto = true;
  else
    ObCorrente->olhaAeroporto = false;

  if (teclas['a'])
    AviaoCorrente->setAilerons(90);
  else if (teclas['d'])
    AviaoCorrente->setAilerons(-90);
  else
    AviaoCorrente->setAilerons(0);

  if (teclas['q'])
    AviaoCorrente->setLeme(-90);
  else if (teclas['e'])
    AviaoCorrente->setLeme(90);
  else
    AviaoCorrente->setLeme(0);

  if (teclas['w'])
    AviaoCorrente->setProfundor(-90);
  else if (teclas['s'])
    AviaoCorrente->setProfundor(90);
  else
    AviaoCorrente->setProfundor(0);

  if (teclas['b'])
  {
    AviaoCorrente->freio = true;
  }
  else
  {
    AviaoCorrente->freio = false;
  }

  f16.Atualiza(DeltaTime);
  a10.Atualiza(DeltaTime);

  if (tela == 0)
  {
    AviaoCorrente->setAcelerador(0);
  }
  else if (tela == 1)
  {
    AtualizaVoo();
  }

  controlaParticula.Update(DeltaTime);
  observador1.atualiza(DeltaTime);
  observador2.atualiza(DeltaTime);

  glutPostRedisplay();

  ultimoTime = time;
}

void myInit(void)
{
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glEnable(GL_LIGHTING);
  glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient2);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse2);
  glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular2);
  glLightfv(GL_LIGHT0, GL_POSITION, LightPosition2);

  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
  glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

  glEnable(GL_CULL_FACE);

  glClearDepth(1.0f);
  glClearStencil(0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glEnable(GL_NORMALIZE);
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);

  q = gluNewQuadric();
  gluQuadricNormals(q, GL_SMOOTH);
  gluQuadricTexture(q, GL_TRUE);

  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

  aeroporto.carregar();
  hangar.carregar();
  relevo.carregaDetalhe("./dados/cenario/hangar/mato.tga");
  relevo.carregaArq("./dados/cenario/hangar/terra.tga");

  textAeroporto = LoadTextura("./dados/cenario/hangar/aeroporto.tga");
  skyTopo = LoadTextura("./dados/cenario/ceu/topo.tga");
  skyEsquerda = LoadTextura("./dados/cenario/ceu/esquerda.tga");
  skyDireita = LoadTextura("./dados/cenario/ceu/direita.tga");
  skyTraz = LoadTextura("./dados/cenario/ceu/traz.tga");
  skyFrente = LoadTextura("./dados/cenario/ceu/frente.tga");

  f16.carregarAviao();
  a10.carregarAviao();
  AviaoCorrente = &f16;
  Missil::Carrega();
  ObCorrente->AviaoCorrente = AviaoCorrente;

  for (int i = 0; i < 8; i++)
    misseis[i].ID = i;

  PreparaHangar();
}

void myReshape(int w, int h)
{
  TelaLargura = w;
  TelaAltura = h;
}

void GerenciaMouse(int button, int state, int x, int y)
{

  //GLfloat CPosx;
  //GLfloat CPosY;
  Coord auxVet;

  angX = ((x - TelaLargura / 2.0) / TelaLargura) * 70 * (float)TelaLargura / TelaAltura;
  angY = ((y - TelaAltura / 2.0) / TelaAltura) * 70;

  auxVet.x = ObCorrente->Alvo.x - ObCorrente->Posicao.x;
  auxVet.y = ObCorrente->Alvo.y - ObCorrente->Posicao.y;
  auxVet.z = ObCorrente->Alvo.z - ObCorrente->Posicao.z;

  auxVet = TransRotacao(auxVet, angX, 0, 1, 0);
  auxVet.y += sin((-angY) / 57.3) * DistanciaPontos(ObCorrente->Posicao, ObCorrente->Alvo);

  Coord raio[2];
  Coord Chao[4];
  Coord Ponto;
  Chao[0].x = 500;
  Chao[0].y = 0;
  Chao[0].z = 500;
  Chao[1].x = -500;
  Chao[1].y = 0;
  Chao[1].z = 500;
  Chao[2].x = -500;
  Chao[2].y = 0;
  Chao[2].z = -500;
  Chao[3].x = 500;
  Chao[3].y = 0;
  Chao[3].z = -500;

  raio[0] = ObCorrente->Posicao;
  raio[1].x = ObCorrente->Posicao.x + auxVet.x * 20;
  raio[1].y = ObCorrente->Posicao.y + auxVet.y * 20;
  raio[1].z = ObCorrente->Posicao.z + auxVet.z * 20;

  bool Adicionar = false;
  if (interceptaTriangulo(raio, Chao, Ponto))
  {
    clicX = Ponto.x;
    clicY = Ponto.y;
    clicZ = Ponto.z;
    Adicionar = true;
  }
  else
  {
    Chao[1] = Chao[3];
    if (interceptaTriangulo(raio, Chao, Ponto))
    {
      clicX = Ponto.x;
      clicY = Ponto.y;
      clicZ = Ponto.z;
      Adicionar = true;
    }
  }

  if (button == GLUT_LEFT_BUTTON)
    if (state == GLUT_DOWN)
    {

      if (Adicionar)
      {
        listaAvioes[contListaAvioes].tipo = AviaoCorrente;
        listaAvioes[contListaAvioes].posicao.x = clicX;
        listaAvioes[contListaAvioes].posicao.y = clicY;
        listaAvioes[contListaAvioes].posicao.z = clicZ;
        contListaAvioes++;
      }
    }
}
