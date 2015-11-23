#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GLES3/gl3.h>
#include <GL/glut.h>
#include "robotopengl.hpp"

/************************
   Constantes, variables globales et initialisations
************************/

//Initialisation de la fenêtre
GLushort largeur_fenetre=600;
GLushort hauteur_fenetre=600;
GLushort pos_x_fenetre=100;
GLushort pos_y_fenetre=100;


char leftdown=0, middledown=0, rightdown=0;
char keyz=0, keyZ=0, keyx=0, keyX=0, keyy=0, keyY=0, keyleft=0, keydown=0, keyright=0, keyup=0;


GLfloat translatex=0, translatey=0;

GLfloat vitesse=1;

GLdouble theta_y=10,proportion=1,clipnear=0.1,clipfar=10000, distance=500;
GLdouble stepx=1, stepy=1, stepz=1;

int mousex, mousey;

char init=0;

///////////////////////////////
// Variables du robot;

ParametreRobot paramG, paramD, *paramSelect;;


unsigned char ancienkey;


int nbfichierseps=0;

int choixmenu=1;

void InitRobot()
{
  paramG.angle_tete=-10;
  paramG.angle_epaule=-10;
  paramG.angle_twistepaule=-10;
  paramG.angle_coude=10;
  paramG.angle_twistcoude=0;
  paramG.angle_poignet=0;
  paramG.angle_twistmain=120;
  paramG.angle_doigts=10;
  paramG.angle_jambe=10;
  paramG.angle_twistjambes=-10;
  paramG.angle_genoux=10;
  paramG.angle_pied=0;
 
  paramD.angle_tete=-10;
  paramD.angle_epaule=-10;
  paramD.angle_twistepaule=-30;
  paramD.angle_coude=20;
  paramD.angle_twistcoude=0;
  paramD.angle_poignet=10;
  paramD.angle_twistmain=120;
  paramD.angle_doigts=20;
  paramD.angle_jambe=30;
  paramD.angle_twistjambes=-15;
  paramD.angle_genoux=30;
  paramD.angle_pied=0;

  paramSelect=&paramG;
}


void InitZero()
{
  paramG.angle_tete=0;
  paramG.angle_epaule=0;
  paramG.angle_twistepaule=0;
  paramG.angle_coude=0;
  paramG.angle_twistcoude=0;
  paramG.angle_poignet=0;
  paramG.angle_twistmain=90;
  paramG.angle_doigts=0;
  paramG.angle_jambe=0;
  paramG.angle_twistjambes=0;
  paramG.angle_genoux=0;
  paramG.angle_pied=0;
 
  paramD.angle_tete=0;
  paramD.angle_epaule=0;
  paramD.angle_twistepaule=0;
  paramD.angle_coude=0;
  paramD.angle_twistcoude=0;
  paramD.angle_poignet=0;
  paramD.angle_twistmain=90;
  paramD.angle_doigts=0;
  paramD.angle_jambe=0;
  paramD.angle_twistjambes=0;
  paramD.angle_genoux=0;
  paramD.angle_pied=0;

  paramSelect=&paramG;
}

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



void DessinObjet()
{	
  DessinAxes();
  glLineWidth(1);
  glColor3f(0,0,0);
  DessineRobot(paramG, paramD);
   /*
  if (choixmenu != 1)
    DessinAxes();
  glLineWidth(1);
  glColor3f(0,0,0);
  //  glutWireTeapot(5);
  switch(choixmenu)
    {
    case 1:
      DessineRobot(paramG, paramD);
      break;
    case 2:
      DessineBras(paramD);
      break;
    case 3:
      DessineBras(paramG);
      break;
    case 4:
      DessineJambe(paramD);
      break;
    case 5:
      DessineJambe(paramG);
      break;
    case 6:
      DessineAvantBras(paramD);
      break;
    case 7:
      DessineAvantBras(paramG);
      break;
    case 8:
      DessineMain(paramD);
      break;
    case 9:
      DessineMain(paramG);
      break;
    case 10:
      DessineDoigt(paramD);
      break;
    case 11:
      DessineDoigt(paramG);
      break;
    }
    */
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
  glTranslatef(0.0,0.0,-distance-500);
  //  glRotatef(-angle_twist,0.0,0.0,1.0);
  glRotatef(angle_elevation,1.0,0.0,0.0);
  glRotatef(angle_azimuth,0.0,1,0);

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


  InitRobot();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);

  /******************************************************************\
 | Boucle récupérant les évennements SDL et affichant périodiquement  |
  \******************************************************************/
  bool terminer=false;
  SDL_Event evenements;
  GLint currentTime, nextDueFrameDate = 0;

  int nbFrames=0;
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
	    case SDLK_l:
	      paramSelect = & paramG;
	      break;
	    case SDLK_r:
	      paramSelect = & paramD;
	      break;
	    case SDLK_a:break;
	    case SDLK_i:
	      InitRobot();
	      InitObjet();
	      break;
	    /* case SDLK_x:  */
	    /*   keyx=1; */
	    /*   break; */
	    /* case X:  */
	    /*   keyX=1; */
	    /*   break; */
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
	    case SDLK_d:
	      paramSelect->angle_doigts += (shiftPressed ? -1 : 1)*vitesse;
	      break;
	    case SDLK_p:
	      paramSelect->angle_poignet += (shiftPressed ? -1 : 1)*vitesse;
	      break;
	    case SDLK_c:
	      paramSelect->angle_coude += (shiftPressed ? -1 : 1)*vitesse;
	      break;
	    case SDLK_u:
	      paramSelect->angle_twistmain += (shiftPressed ? -1 : 1)*vitesse;
	      break;
	    case SDLK_v:
	      paramSelect->angle_twistcoude += (shiftPressed ? -1 : 1)*vitesse;
	      break;
	    case SDLK_e:
	      paramSelect->angle_epaule += (shiftPressed ? -1 : 1)*vitesse;
	      break;
	    case SDLK_w:
	      paramSelect->angle_twistepaule -= (shiftPressed ? -1 : 1)*vitesse;
	      break;
	    case SDLK_g:
	      paramSelect->angle_genoux += (shiftPressed ? -1 : 1)*vitesse;
	      break;
	    case SDLK_f:
	      paramSelect->angle_pied += (shiftPressed ? -1 : 1)*vitesse;
	      break;
	    case SDLK_j:
	      paramSelect->angle_jambe += (shiftPressed ? -1 : 1)*vitesse;
	      break;
	    case SDLK_t:
	      paramG.angle_tete -= (shiftPressed ? -1 : 1)*vitesse;
	      paramD.angle_tete -= (shiftPressed ? -1 : 1)*vitesse;
	      break;
	    case SDLK_m:
	      paramSelect->angle_twistjambes -=  vitesse;
	      break;
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
	    case SDLK_h:
	      fprintf(stderr,"0 : sélectioner le coté gauche\n");
	      fprintf(stderr,"1 : sélectioner le coté droit\n");
	      fprintf(stderr,"d, D : angle des doigts\n");
	      fprintf(stderr,"p, P : angle du poignet\n");
	      fprintf(stderr,"u, U : twist du poignet\n");
	      fprintf(stderr,"c, C : angle du coude\n");
	      fprintf(stderr,"v, V : twist du coude\n");
	      fprintf(stderr,"e, E : angle de l'épaule\n");
	      fprintf(stderr,"w, W : twist du l'épaule\n");
	      fprintf(stderr,"j, J : angle des jambes\n");
	      fprintf(stderr,"g, G : angle des genoux\n");
	      fprintf(stderr,"r, R : angle des pieds\n");
	      fprintf(stderr,"t, T : angle de la tête\n");
	      fprintf(stderr,"m, M : twist des jambes\n");

	      break;

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

 
