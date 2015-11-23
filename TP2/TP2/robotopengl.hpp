

typedef struct
{
  GLfloat angle_tete;
  GLfloat angle_epaule;
  GLfloat angle_twistepaule;
  GLfloat angle_coude;
  GLfloat angle_twistcoude;
  GLfloat angle_poignet;
  GLfloat angle_twistmain;
  GLfloat angle_doigts;
  GLfloat angle_jambe;
  GLfloat angle_twistjambes;
  GLfloat angle_genoux;
  GLfloat angle_pied;
}ParametreRobot;

void DessinePhalange(GLfloat longueur);
void DessineDoigt(ParametreRobot param);
void DessineMain(ParametreRobot param);
void DessineAvantBras(ParametreRobot param);
void DessineBras(ParametreRobot param);
void DessineJambe(ParametreRobot param);

void DessineRobot(ParametreRobot paramG, ParametreRobot paramD);
int GeneratePgm(char *nomfichier,  unsigned int larg, unsigned int haut);
int GenerateEps(char *nomfichier, int inColor,  unsigned int larg, unsigned int haut);


