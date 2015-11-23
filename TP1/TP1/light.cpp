#include "light.hpp"
#include <iostream>

Light::Light(){

}

Light::Light(double x, double y, double z, int angle, bool rotate, GLenum lightName, double & ambient, double & shininess, double & brightness){
    position[0] = x;
    position[1] = y;
    position[2] = z;
    position[3] = 1.0f;
    this->angle = angle;
    this->rotate = rotate;
    this->lightName = lightName; // For example GL_LIGHT0
    this->ambient = ambient;
    this->brightness = brightness;
    this->shininess = shininess;
}

void Light::LightScene(){
    this->angle = angle;

    GLfloat specularLight[] = {brightness, brightness, brightness, 1.0};
    GLfloat diffuseLight[] = {brightness, brightness, brightness, 1.0};
    GLfloat diffuse[] = {1.0,1.0,1.0,1.0};
    GLfloat specular[] = {1.0,1.0,1.0,1.0};
    GLfloat ambientLight[] = {ambient, ambient, ambient, ambient};

    glPushMatrix();

    // If we rotate, we rotate around Z-axis
    if(rotate)
    {
        angle = (angle == 359 ? 0 : angle+1);
        glRotatef(angle, 0, 0, 1);
    }

    glLightfv(lightName, GL_DIFFUSE, diffuseLight);
    glLightfv(lightName, GL_SPECULAR, specularLight);
    glLightfv(lightName, GL_POSITION, position);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientLight);
    glPopMatrix();

}

void Light::Activate(bool state){
    rotate = state;
}

void Light::changeBrightness(float i){
    if((brightness > 0.0 && i < 0) || (brightness < 1.0 && i > 0))
        brightness += i;

    if(brightness > 1)
        brightness = 1;

    if(brightness < 0)
        brightness = 0;
}

void Light::changeShininess(float i){
    if((shininess > 0 && i < 0) || (shininess < 128 && i > 0))
        shininess += i;

    if(shininess > 128)
        shininess = 128;

    if(shininess < 0)
        shininess = 0;
}

void Light::changeAmbient(float i){
    if((ambient > 0 && i < 0) || (ambient < 1 && i > 0))
        ambient += i;

    if(ambient > 1)
        ambient = 1;

    if(ambient < 0)
        ambient = 0;
}

void Light::setAmbient(double v){
    ambient = v;
}
void Light::setBrightness(double v){
    brightness = v;
}

void Light::setShininess(double v){
    shininess = v;
}

double Light::getAmbient(){
    return ambient;
}

double Light::getBrightness(){
    return brightness;
}

double Light::getShininess(){
    return shininess;
}
