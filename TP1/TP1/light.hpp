#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GLES3/gl3.h>
#include <GL/glut.h>

class Light
{
private :
    int angle;
    bool rotate;
    GLenum lightName;
    GLfloat position[4];
    double shininess; // Brillance
    double brightness; // Luminosité
    double ambient;

public:
    Light();
    Light(double , double , double, int, bool , GLenum, double &, double &, double &);
    void changeBrightness(float);
    void changeShininess(float);
    void changeAmbient(float);
    void LightScene();
    void Activate(bool);

    void setAmbient(double);
    void setBrightness(double);
    void setShininess(double);

    double getAmbient();
    double getBrightness();
    double getShininess();

};

#endif // LIGHT_HPP
