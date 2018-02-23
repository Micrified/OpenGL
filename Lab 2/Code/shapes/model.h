#ifndef MODEL_H_
#define MODEL_H_

#include "../object.h"
#include "../objloader.h"
#include <exception>
#include <fstream>
#include <iostream>

class Model: public Object
{
    public:
        Model(std::string a);

        virtual Hit intersect(Ray const &ray);

        
};

#endif
