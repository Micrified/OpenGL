#include "material.h"


Material::Material(Color const &color, double ka, double kd, double ks, double n) {
    this->color = Color(color);
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->n  = n;
    this->texture = NULL;
    this->isTextured = false;
}

Material::Material(std::string const &textureName, double ka, double kd, double ks, double n) {
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->n  = n;
    this->texture = new Image(textureName);
    this->isTextured = true;
}


