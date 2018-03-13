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
        bool isTextured;       // true if the material is a texture
        Image texture;

        Material() = default;

        Material(Color const &color, double ka, double kd, double ks, double n, bool isTextured)
        :   
            color(color),
            ka(ka),
            kd(kd),
            ks(ks),
            n(n),
            isTextured(isTextured)
        {}

        Material(Image texture, double ka, double kd, double ks, double n, bool isTextured)
        :
            ka(ka),
            kd(kd),
            ks(ks),
            n(n),
            isTextured(isTextured),
            texture(texture)
        {}
};

#endif
