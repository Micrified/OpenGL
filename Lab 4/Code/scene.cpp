#include "scene.h"

#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"

#include <cmath>
#include <limits>


using namespace std;


Hit Scene::firstIntersect(Ray const &ray, int *objectIndexPtr, int ignoringObjectAtIndex)
{
    // Find hit object and distance
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    int objectIndex = NONE;

    // Determine the closest object hit in the scene.
    for (int idx = 0; idx != (int)objects.size(); ++idx)
    {   
        if (idx == ignoringObjectAtIndex) {
            continue;
        }

        Hit hit(objects[idx]->intersect(ray));
        if (hit.t < min_hit.t)
        {
            min_hit = hit;
            objectIndex = idx;
        }
    }
    if (objectIndexPtr != NULL) {
        *objectIndexPtr = objectIndex;
    }
    return min_hit;
}

Color Scene::trace(Ray const &ray, int depth, int ignore)
{
    // Find hit object and distance
    int objectIndex;
    Hit min_hit = firstIntersect(ray, &objectIndex, ignore);
    
    // No hit? Return background color.
    if (objectIndex == NONE || depth == 0) return Color(0.0, 0.0, 0.0);

    ObjectPtr obj = objects[objectIndex];
    Material material = obj->material;          //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector

    Color diffuse;
    Color specular;

    /*********************** Get Color **************************/
    Color color;
    if (material.isTextured) {
        color = obj->getTextureColorAtPoint(hit);
    } else {
        color = material.color;  
    }

    /*********************** Compute illumination **************************/

    for(unsigned i = 0; i < lights.size(); i++){
        Vector L = lights[i]->position - hit;

        Ray lightRay(hit, L);
        Hit lightIntersect = firstIntersect(lightRay, NULL, objectIndex);

        if(hasShadows && lightIntersect.t < L.length()) continue;
        
        Vector R = 2 * (L.dot(N)) * N - L;
        L.normalize();
        R.normalize();

        diffuse += fmax(0,N.dot(L)) * lights[i]->color * material.kd * color;
        specular += pow(fmax(0, R.dot(V)), material.n) * lights[i]->color * material.ks;
        
    }

    Vector reflect = 2 * (V.dot(N)) * N - V;
    reflect.normalize();
    Color reflection = trace(Ray(hit, reflect), depth - 1, objectIndex) * material.ks;

    Color ambient = color * material.ka;

    return diffuse + ambient + specular + reflection;

}

void Scene::render(Image &img)
{
    unsigned a = superSamplingFactor;
    unsigned w = img.width();
    unsigned h = img.height();
    for (unsigned y = 0; y < h; ++y)
    {
        for (unsigned x = 0; x < w; ++x)
        {   

            /* Anti-Aliasing */
            Color col;
            double offset = 1.0 / (double)(a + a); 
            for (unsigned i = 1; i <= a; ++i) {
                for (unsigned j = 1; j <= a; j++) {
                    Point pixel(x + i * offset, h - 1 - y + j * offset, 0);
                    Ray ray(eye, (pixel - eye).normalized());
                    col += trace(ray, maxRecursion);
                }
            }
            col /= a * a;
            col.clamp();
            img(x, y) = col;
        }
    }
}

// --- Misc functions ----------------------------------------------------------

void Scene::addObject(ObjectPtr obj)
{
    objects.push_back(obj);
}

void Scene::addLight(Light const &light)
{
    lights.push_back(LightPtr(new Light(light)));
}

void Scene::setEye(Triple const &position)
{
    eye = position;
}

void Scene::setHasShadows (bool hasShadows) {
    this->hasShadows = hasShadows;
}

void Scene::setSuperSamplingFactor (int superSamplingFactor) {
    this->superSamplingFactor = superSamplingFactor;
}

void Scene::setMaxRecursion (int maxRecursion) {
    this->maxRecursion = maxRecursion;
}

unsigned Scene::getNumObject()
{
    return objects.size();
}

unsigned Scene::getNumLights()
{
    return lights.size();
}
