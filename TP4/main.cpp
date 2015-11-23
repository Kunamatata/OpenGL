#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GLES3/gl3.h>
#include <GL/glut.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

#define largeur_tex 128
#define hauteur_tex 128

GLushort largeur_fenetre=500;
GLushort hauteur_fenetre=500;

GLuint id = 0;  /* texture ID */

/* bitmap contenant les couleurs RGBA de la texture */
GLubyte image_tex[largeur_tex*hauteur_tex*4];

void CreeBitmap()
{
  int i, j;
  GLubyte r, g, b;
  for (i=0 ; i<hauteur_tex ; i++)
    {
      for (j=0 ; j<largeur_tex ; j++)
        {
          r = (i*32)%256;
          g = (j*32)%256;
          b = ((i*16)*(j*16))%256;
          image_tex[(i*largeur_tex+j)*4]   = r;
          image_tex[(i*largeur_tex+j)*4+1] = g;
          image_tex[(i*largeur_tex+j)*4+2] = b;
          image_tex[(i*largeur_tex+j)*4+3] = 255;
        }
    }
}

void CreeTexture2D(void)
{
  struct gl_texture_t *png_tex = NULL;

  CreeBitmap();
  /* Generation texture */
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  /* Param�tres de filtres lin�aires */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  /* Cr�ation de la texture */
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, largeur_tex, hauteur_tex,
               0, GL_RGBA, GL_UNSIGNED_BYTE, image_tex);
}

void Affichage(GLfloat angle_azimuth, GLfloat angle_elevation)
{
   GLUquadric * quad;

  glClearColor(1.0,1.0,1.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
glLoadIdentity();
//  glMatrixMode(GL_MODELVIEW);
//  glPushMatrix();
 gluLookAt(1, 20, 20,   /* r�glage de la cam�ra */
           0,0,0,
           0,1,0);
//  glRotatef(0.4*angle, 0.0, 1.0, 0.0);

//  glBindTexture (GL_TEXTURE_2D, id); /* s�lection de la texture */
//  glEnable (GL_TEXTURE_2D); /* activation des textures */

//  glBegin(GL_QUADS);
//  glTexCoord2f(0.0,0.0); glVertex3f(-3.0,-2.0,0.0);
//  glTexCoord2f(1.0,0.0); glVertex3f(3.0,-2.0,0.0);
//  glTexCoord2f(1.0,1.0); glVertex3f(3.0,2.0,0.0);
//  glTexCoord2f(0.0,1.0); glVertex3f(-3.0,2.0,0.0);
//  glEnd();

  //glDisable (GL_TEXTURE_2D); /* d�sactivation des textures */

  //glPopMatrix();

  //glColor3f(1,0,0);

  glPushMatrix();
  glColor3f(0,0,0);
  glBegin(GL_POLYGON);

  quad = gluNewQuadric();
  gluCylinder(quad,1.5,1.55,1.52,1000,1000);
  glRotatef(angle_elevation,1.0,0.0,0.0);

  glRotatef(angle_azimuth,0,1,0);
  glEnd();

  glPopMatrix();

}

void Redimensionnement(int l,int h)
{
  largeur_fenetre = l;
  hauteur_fenetre = h;
  glViewport(0,0,(GLsizei)largeur_fenetre,(GLsizei)hauteur_fenetre);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(20,l/(GLdouble)h, 0.01, 10000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

/** @brief Fonction d'initialisation de la fen�tre SDL
    @param windowWidth largeur de la fen�tre en pixels
    @param windowWidth largeur de la fen�tre en pixels
    @param windowTitle Titre de la fen�tre dans sa barre de titre
*/
SDL_Window * init_SDL_Window(int windowWidth, int windowHeight, const char* windowTitle){
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
  SDL_Init(SDL_INIT_VIDEO);
  /* Le double buffering permet les animations temps r�el */
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_Window *window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  return window;
}

int main(int argc, char**argv)
{

  /* Initialisation d'une fen�tre SDL */
  SDL_Window *window =init_SDL_Window(largeur_fenetre, hauteur_fenetre,
                                      "Premier exemple de texture OpenGL");
  /* d�finition du contexte OpenGL associ� � cette fen�tre */
  SDL_GLContext glcontext(SDL_GL_CreateContext(window));

  CreeTexture2D(); /* Initialisation de la texture */
int mousex, mousey;
  /******************************************************************\
 | Boucle r�cup�rant les �vennements SDL et affichant p�riodiquement  |
  \******************************************************************/
  bool terminer=false;
    bool leftButtonPressed = false;
    GLfloat angle_azimuth=-60.0 , angle_elevation=30;
  SDL_Event evenements; /* union contenant un �vennement */
  GLint currentTime, nextDueFrameDate = 0; /* pour timer affichage */
GLfloat vitesse=1;
  int nbFrames=0; /* compteur de frames */
  while(!terminer)
    {
      while (SDL_PollEvent(&evenements))  /* on d�file les �vennements */
        {
          switch (evenements.type){ /* suivant le type d'�vennement */
          case SDL_WINDOWEVENT :
            int w,h;
            SDL_GetWindowSize(window,&w,&h); /* r�cup�ration taille fen�tre */
            Redimensionnement(w, h);
            break;
          case SDL_QUIT: /* fermeture de la fen�tre */
            terminer = true;
            break;
          case SDL_MOUSEBUTTONDOWN: /* Enfoncement d'un bouton souris */
            switch (evenements.button.button){
            case SDL_BUTTON_LEFT :  /* Bouton gauche */
              leftButtonPressed=true;
              mousex = evenements.button.x; /* mémorisation coordonnées de la souris */
              mousey = evenements.button.y; /* mémorisation coordonnées de la souris */
              break;
            }
            break;
          case SDL_MOUSEBUTTONUP: /* Relachement d'un bouton souris */
            switch (evenements.button.button){
            case SDL_BUTTON_LEFT :  /* Bouton gauche */
              leftButtonPressed=false;
              break;
            }
            break;
          case SDL_MOUSEMOTION: /* Mouvement de la souris */
            if (leftButtonPressed){
              angle_elevation += vitesse*(evenements.motion.y-mousey);
              angle_azimuth += vitesse*(evenements.motion.x-mousex);
              mousex = evenements.motion.x; /* enregistrement des nouvelles */
              mousey = evenements.motion.y; /* coordonnées de la souris */
            }
          }
        }

      /* gestion des frames par secondes (ici 50 fps) */
      currentTime = SDL_GetTicks(); /* date courante en milliseconde */

      if (currentTime > nextDueFrameDate)
        {
          nextDueFrameDate = currentTime + 20; /* toutes les 20 millisecondes */
          Affichage(angle_azimuth,angle_elevation); /* on raffraichit la vue */
          nbFrames++; /* On incr�mente le compteur de frames */
          SDL_GL_SwapWindow(window); /* On envoie le buffer � l'�cran */
        }
    }

  // Lib�ration des ressources SDL
  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
