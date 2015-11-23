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

#include "light.hpp"

using namespace std;

//Initialisation de la fenêtre
GLushort largeur_fenetre=600;
GLushort hauteur_fenetre=600;
GLushort pos_x_fenetre=100;
GLushort pos_y_fenetre=100;


double variationLuminosite = 0.4;
double variationBrillance = 128.0;
double variationAmbiante = 0.2;
float angle = 0.0;

/*Light declarations*/
Light staticLight(40.0, 20.0, -20.0, 0, false, GL_LIGHT0, variationAmbiante, variationBrillance, variationLuminosite);
Light rotatingLight(20.0, 20.0, 20.0, 0, true, GL_LIGHT1, variationAmbiante, variationBrillance, variationLuminosite);

char leftdown=0, middledown=0, rightdown=0;
char keyz=0, keyZ=0, keyx=0, keyX=0, keyy=0, keyY=0, keyleft=0, keydown=0, keyright=0, keyup=0;

GLfloat translatex=0, translatey=0;

GLfloat vitesse=1;

GLdouble theta_y=10,proportion=1,clipnear=0.1,clipfar=10000, distance=500;
GLdouble stepx=1, stepy=1, stepz=1;

int mousex, mousey;

char init=0;

//////////////////////////////


unsigned char ancienkey;


int nbfichierseps=0;

int choixmenu=1;

const aiScene* scene;


void Redimensionnement(int l,int h)
{
  largeur_fenetre = l;
  hauteur_fenetre = h;
  glViewport(0,0,(GLsizei)largeur_fenetre,(GLsizei)hauteur_fenetre);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(theta_y,l/(GLdouble)h, 0.01, 10000);

}

void InitObjet()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


void selection_menu_choix_camera(int value)
{
  choixmenu=value;
}


void DessinAxes()
{
  glLineWidth(3);
  glBegin(GL_LINES);
  glColor3f(1,0,0);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(10.0,0.0,0.0);
  glColor3f(0,1,0);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(0.0,10.0,0.0);
  glColor3f(0,0,1);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(0.0,0.0,10.0);
  glEnd();

}

GLuint loadBMPTexture(const char *filename)
{
    GLuint tex_id = 0;

    SDL_Surface * sdlSurface = SDL_LoadBMP(filename);

    glGenTextures (1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sdlSurface->w, sdlSurface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, sdlSurface->pixels);

    SDL_FreeSurface(sdlSurface);
    return tex_id;
}

void DessinObjet()
{
 GLUquadric * quad;
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    staticLight.LightScene();
    rotatingLight.LightScene();

    /* Cylinder */
    GLuint texID = loadBMPTexture("../TP1/barrelTexture.bmp");
    glEnable(GL_TEXTURE_2D);
    quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);

    glPushMatrix();
    glRotatef(90, 1,  0 , 0 );
    glTranslatef(0,1,-9.5);
    glTranslatef(0,4,0);
    gluCylinder(quad,2,2,2,10,10);
    gluDisk(quad, 0, 2, 50, 50);

    glTranslatef(0,0,2);
    gluDisk(quad, 0, 2.5, 50, 50);

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    /* End Cylinder */

  glLineWidth(1);
  glColor3f(0,0,0);

  glBegin(GL_TRIANGLES);

  for(unsigned int i = 0; i < scene->mNumMeshes; ++i)
  {
      aiMesh *mesh = scene->mMeshes[i];

      for(unsigned int j = 0; j < mesh->mNumFaces; ++j)
      {
          aiFace face = mesh->mFaces[j];

          for(unsigned int k = 0; k < face.mNumIndices; ++k)
          {
             unsigned int indice = face.mIndices[k];

             aiVector3D position = mesh->mVertices[indice];
             aiVector3D normal = mesh->mNormals[indice];

             glNormal3f(normal.x, normal.y, normal.z);
             glVertex3f(position.x, position.y, position.z);
          }
      }
  }


  glEnd();
}



void Affichage(GLfloat angle_twist, GLfloat angle_elevation,
               GLfloat angle_azimuth, GLfloat distance)
{
  glClearColor(1.0,1.0,1.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  // rotations dans le repère du monde
  glMatrixMode(GL_MODELVIEW);
  if (keyx)
    glRotatef(vitesse,1.0,0.0,0.0);
  if (keyX)
    glRotatef(-vitesse,1.0,0.0,0.0);
  if (keyy)
    glRotatef(vitesse,0.0,1,0);
  if (keyY)
    glRotatef(-vitesse,0.0,1,0);
  if (keyz)
    glRotatef(vitesse,0.0,0.0,1.0);
  if (keyZ)
    glRotatef(-vitesse,0.0,0.0,1.0);

  keyx = keyX = keyy = keyY = keyz = keyZ = 0;

  glPushMatrix();
  glTranslatef(0.0,0.0,-distance-150);
  //  glRotatef(-angle_twist,0.0,0.0,1.0);
  glRotatef(angle_elevation,1.0,0.0,0.0);

  glRotatef(angle_azimuth,0,1,0);

  DessinObjet();

  glPopMatrix();
}

void FonctionMenu(int value)
{
  choixmenu=value;
}

/** @brief Fonction d'initialisation de la fenêtre SDL
    @param windowWidth largeur de la fenêtre en pixels
    @param windowWidth largeur de la fenêtre en pixels
    @param windowTitle Titre de la fenêtre dans sa barre de titre
*/
SDL_Window * init_SDL_Window(int windowWidth, int windowHeight, const char* windowTitle){
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
  SDL_Init(SDL_INIT_VIDEO);
  /* Le double buffering permet les animations temps réel */
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_Window *window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  return window;
}


int main(int argc, char**argv)
{
  char nomfichier[100];
  GLfloat angle_twist=0;
  GLfloat angle_elevation=30.0;
  GLfloat angle_azimuth=-60.0;
  GLfloat distance=15;
  GLfloat ambient_scene[] = {0.2,0.2,0.2, 1.0};
  GLushort  mousex, mousey;  /* permet de mémoriser la dernière position */
  /* de la souris */
   float angle = 0.0;
  bool leftButtonPressed = false;
  bool middleButtonPressed = false;

  bool shiftPressed=false;

  glutInit(&argc, argv);

  /* Initialisation d'une fenêtre SDL */
  SDL_Window *window =init_SDL_Window(largeur_fenetre, hauteur_fenetre,
                      "Chargement d'un fichier 3D avec ASSIMP");
  /* définition du contexte OpenGL associé à cette fenêtre */
  SDL_GLContext glcontext(SDL_GL_CreateContext(window));

  /* glutAddMenuEntry("Robot", 1); */
  /* glutAddMenuEntry("Bras droit", 2); */
  /* glutAddMenuEntry("Bras gauche", 3); */
  /* glutAddMenuEntry("Jambe droite", 4); */
  /* glutAddMenuEntry("Jambe gauche", 5); */
  /* glutAddMenuEntry("Avant bras droit", 6); */
  /* glutAddMenuEntry("Avant bras gauche", 7); */
  /* glutAddMenuEntry("Main droite", 8); */
  /* glutAddMenuEntry("Main gauche gauche", 9); */
  /* glutAddMenuEntry("Doigt main droite", 10); */
  /* glutAddMenuEntry("Doight main gauche", 11); */
  /* glutAttachMenu(GLUT_RIGHT_BUTTON); */

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  glEnable(GL_SMOOTH);
  glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glShadeModel(GL_SMOOTH);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);

  /******************************************************************\
 | Boucle récupérant les évennements SDL et affichant périodiquement  |
  \******************************************************************/
  bool terminer=false;
  SDL_Event evenements;
  GLint currentTime, nextDueFrameDate = 0;

  int nbFrames=0;

  // This code can not be exported to a function


  // Create an instance of the Importer class
  Assimp::Importer importer;
  // And have it read the given file with some example postprocessing
  // Usually - if speed is not the most important aspect for you - you'll
  // propably to request more postprocessing than we do in this example.
  scene = importer.ReadFile( "../TP1/fichiers3DS/camel.3ds",
                                            aiProcess_FindDegenerates |
                                            aiProcess_Triangulate |
                                            aiProcess_SortByPType |
                                            aiProcess_GenNormals);

  if( !scene)
  {
    cout << "Scene not loaded" << endl;
    return 0;
  }

  cout << "Number of meshes : " << scene->mNumMeshes << endl;

  //DoTheImportThing("../TP1/fichiers3DS/camel.3ds");

  while(!terminer)
    {
      while (SDL_PollEvent(&evenements))
    {
      switch (evenements.type){
      case SDL_KEYUP: /* Enfoncement d'une touche */
        switch (evenements.key.keysym.sym){
        case SDLK_LSHIFT: case SDLK_RSHIFT:// reverse
          shiftPressed=false;
          break;
        }
        break;
      case SDL_KEYDOWN: /* Enfoncement d'une touche */
        switch (evenements.key.keysym.sym){
        case SDLK_LSHIFT: case SDLK_RSHIFT: // reverse
          shiftPressed=true;
          break;
        case SDLK_n:
          InitObjet();
          break;
         case SDLK_i:
          staticLight.changeBrightness(0.01);

          break;
         case SDLK_k:
            staticLight.changeBrightness(-0.01);

            break;
        case SDLK_o:
         staticLight.changeShininess(1);

         break;
        case SDLK_l:
           staticLight.changeShininess(-1);
           break;
                case SDLK_e:
                 staticLight.changeAmbient(0.01);


                 break;
                case SDLK_d:
                    staticLight.changeAmbient(-0.01);
                   break;
        /* case SDLK_y:  */
        /*   keyy=1; */
        /*   break; */
        /* case SDLK_Y:  */
        /*   keyY=1; */
        /*   break; */
        /* case SDLK_z:  */
        /*   keyz=1; */
        /*   break; */
        /* case SDLK_Z:  */
        /*   keyZ=1; */
        /*   break; */
        case SDLK_s:
          if (!shiftPressed){
        sprintf(nomfichier, "output%d.eps", nbfichierseps);
        //GenerateEps(nomfichier, 0, largeur_fenetre, hauteur_fenetre);
        nbfichierseps++;
          }
          else{
            sprintf(nomfichier, "output%d.pgm", nbfichierseps);
            //GeneratePgm(nomfichier, largeur_fenetre, hauteur_fenetre);
            nbfichierseps++;
          break;
          }
        case SDLK_q : /* touche 'q', on quitte */
          terminer = true;
          break;
        }
        break;
      case SDL_MOUSEBUTTONDOWN: /* Enfoncement d'un bouton souris */
        switch (evenements.button.button){
        case SDL_BUTTON_LEFT :  /* Bouton gauche */
          leftButtonPressed=true;
          mousex = evenements.button.x; /* mémorisation coordonnées de la souris */
          mousey = evenements.button.y; /* mémorisation coordonnées de la souris */
          break;
        case SDL_BUTTON_MIDDLE :  /* Bouton gauche */
          middleButtonPressed=true;
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
        case SDL_BUTTON_MIDDLE :  /* Bouton gauche */
          middleButtonPressed=false;
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
        if (middleButtonPressed){
          distance += vitesse*(evenements.motion.y-mousey);
          mousex = evenements.motion.x; /* enregistrement des nouvelles */
          mousey = evenements.motion.y; /* coordonnées de la souris */
        }
        break;
      case SDL_WINDOWEVENT :
        int w,h;
        SDL_GetWindowSize(window,&w,&h); /* récupération taille fenêtre */
        Redimensionnement(w, h);
        break;
      case SDL_QUIT:
        terminer = true;
        break;
      }
    }
      currentTime = SDL_GetTicks();

      if (currentTime > nextDueFrameDate)
        {
          nextDueFrameDate = currentTime + 20;
          Affichage(angle_twist, angle_elevation,
                angle_azimuth, distance);
          nbFrames++;
          SDL_GL_SwapWindow(window);
        }
    }

  // Libération des ressources SDL
  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

