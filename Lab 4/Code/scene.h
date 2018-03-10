#ifndef SCENE_H_
#define SCENE_H_

#include "light.h"
#include "object.h"
#include "triple.h"

#include <vector>

#define NONE    -1

// Forward declerations
class Ray;
class Image;

class Scene
{
    std::vector<ObjectPtr> objects;
    std::vector<LightPtr> lights;   // no ptr needed, but kept for consistency
    Point eye;

    bool hasShadows;
    int superSamplingFactor;
    int maxRecursion;

    public:
        // finds the first intersect of a ray and an object
        Hit firstIntersect(Ray const &ray, int *objectIndexPtr, int ignoringObjectAtIndex);

        // trace a ray into the scene and return the color
        Color trace(Ray const &ray, int depth, int ignore = NONE);

        // render the scene to the given image
        void render(Image &img);


        void addObject(ObjectPtr obj);
        void addLight(Light const &light);
        void setEye(Triple const &position);
        void setHasShadows (bool hasShadows);
        void setSuperSamplingFactor (int superSamplingFactor);
        void setMaxRecursion (int maxRecursion);

        unsigned getNumObject();
        unsigned getNumLights();
};

#endif
