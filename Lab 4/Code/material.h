#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "triple.h"
#include "image.h"


class Material
{
    public:
        Color color;        // base color
        double ka;          // ambient intensity
        double kd;          // diffuse intensity
        double ks;          // specular intensity
        double n;           // exponent for specular highlight size
        Image *texture;      // Pointer to the image texture for the material.
        bool isTextured;    // Boolean conditional: Whether or not object is textured.

        Material() = default;

        Material(Color const &color, double ka, double kd, double ks, double n);

        Material(std::string const &textureName, double ka, double kd, double ks, double n);
};

#endif
