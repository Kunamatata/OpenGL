#include <GL/glut.h>
#include "robotopengl.hpp"


#define FACTEUR_JAMBE 1.1
#define FACTEUR_BRAS 0.8

void DessinePhalange(GLfloat longueur)
{
    glPushMatrix();
    glScalef(longueur,1,1);
    glutWireCube(1);
    glPopMatrix();
}


void DessineDoigt(ParametreRobot param)
{
    glPushMatrix(); // Mémorise la transformation géométrique courante

    glTranslatef(1.5,0,0);
    glRotatef(param.angle_doigts,0,1,0);
    DessinePhalange(3);

    glTranslatef(2.5,0,0);
    glRotatef(param.angle_doigts,0,1,0);
    DessinePhalange(2);

    glTranslatef(2,0,0);
    glRotatef(param.angle_doigts,0,1,0);
    DessinePhalange(2);

    glPopMatrix(); // On revient à l'état antérieur
}

void DessinePaume(GLfloat longueur)
{
    glPushMatrix();
    glScalef(longueur,1,longueur);
    glutWireCube(1);
    glPopMatrix();
}

void DessineMain(ParametreRobot param)
{
    //glRotatef(param.angle_poignet, 0 ,0 ,1);
    //glRotatef(param.angle_twistmain+180,1,0,0);

    glPushMatrix();

    // On translate les doigts de 6 pour qu'ils soient collés à la paume, et on les centre
    glTranslatef(6,-0.75,0);

    // On dessine chaque doigt et on les translate de 1.5 pour les écarter
    for(int i = 0; i < 4; i++)
    {
         glTranslatef(0,1.5,0);
        DessineDoigt(param);
    }

    glPopMatrix();

    // On dessine le pouce en le dessinant, puis en le faisant tourner par rapport à son état d'origine, puis en le translatant
    glPushMatrix();
    glTranslatef(1.5,5.5,0);
    glRotatef(90,0,1,0);
    glRotatef(180,1,0,0);
    DessineDoigt(param);
    glPopMatrix();

    glTranslatef(3,3,0);
    glRotatef(90,1,0,0);
    DessinePaume(6);
}


void DessineAvantBras(ParametreRobot param)
{
    glRotatef(90,1,0,0);

    glPushMatrix();

    glTranslatef(18,0,0);
    glRotatef(param.angle_twistmain+220,1,0,0);
    glRotatef(param.angle_poignet,0,0,1);
    DessineMain(param);

    glPopMatrix();

    glTranslatef(9,3,0);
    glRotatef(90,1,0,0);
    glScalef(18,3,5);
    glutWireCube(1);


}

void DessineBras(ParametreRobot param)
{
    glRotatef(param.angle_twistepaule,0,1,0);
    glRotatef(param.angle_epaule,0,0,1);
    glScalef(FACTEUR_BRAS,FACTEUR_BRAS,FACTEUR_BRAS);
    glPushMatrix();
    glTranslatef(18,0,-3);
    glRotatef(param.angle_twistcoude,1,0,0);
    glRotatef(param.angle_coude,0,0,1);
    DessineAvantBras(param);

    glPopMatrix();
    glTranslatef(9,0,0);
    glScalef(18,4,6);
    glutWireCube(1);
}

void DessinePied(ParametreRobot param)
{
    glPushMatrix();
    glScalef(10,2,5);
    glutWireCube(1);
    glPopMatrix();
}

void DessineMollet(ParametreRobot param)
{
    glPushMatrix();
    glScalef(14,4,6);
    glutWireCube(1);
    glPopMatrix();
}

void DessineCuisse(ParametreRobot param)
{
    glPushMatrix();
    glScalef(18,4,6);
    glutWireCube(1);
    glPopMatrix();
}

void DessineJambe(ParametreRobot param)
{


    glPushMatrix(); // Mémorise la transformation géométrique courante


    glRotatef(param.angle_jambe,0,0,1);
    glRotatef(param.angle_twistjambes,0,1,0);
    glTranslatef(9,0,0);
    glPushMatrix();
    DessineCuisse(param);
    glPopMatrix();

    glTranslatef(16,0,0);
    glRotatef(-param.angle_genoux,0,0,1);
    DessineMollet(param);

    glTranslatef(8,4,0);
    glRotatef(param.angle_pied,0,0,1);
    glRotatef(90,0,0,1);
    DessinePied(param);

    glPopMatrix(); // On revient à l'état antérieur

}

void DessineBuste()
{
    glPushMatrix();
    glScalef(30,6,16);
    glutWireCube(1);
    glPopMatrix();
}

void DessineTete(ParametreRobot param){
    glPushMatrix();
    glScalef(1.2,1.0,0.8);
    glutWireSphere(5,5,5);
    glPopMatrix();
}

void DessineRobot(ParametreRobot paramG, ParametreRobot paramD)
{
    glPushMatrix();
    glRotatef(90,0,0,1);
    DessineBuste();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,19.5,0);
    glRotatef(90,0,0,1);
    glRotatef(paramG.angle_tete,0,0,1);
    DessineTete(paramG);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,15,10.5);
    glRotatef(-90,0,0,1);
    DessineBras(paramG);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,15,-10.5);
    glRotatef(-90,0,0,1);
    glScalef(1,1,-1);
    DessineBras(paramD);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0,-15,5);
    glRotatef(-90,0,0,1);
    DessineJambe(paramG);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-15,-5);
    glRotatef(-90,0,0,1);
    glScalef(1,1,-1);
    DessineJambe(paramD);
    glPopMatrix();
}
