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
    for (unsigned idx = 0; idx != objects.size(); ++idx)
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
    int currentObjectIndex = objectIndex;
    Material material = obj->material;          //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector

    Color diffuse;
    Color specular;


    Color color = material.color;   
    for(int i = 0; i < lights.size(); i++){
        Vector L = lights[i]->position - hit;

        Ray lightRay(hit, L);
        Hit lightIntersect = firstIntersect(lightRay, NULL, objectIndex);

        if(lightIntersect.t < L.length()) continue;
        
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
    unsigned a = 4;
    unsigned w = img.width();
    unsigned h = img.height();
    for (unsigned y = 0; y < h; ++y)
    {
        for (unsigned x = 0; x < w; ++x)
        {   
            Color col;
            double offset = 1.0 / (double)(a + a); 
            for (unsigned i = 1; i <= a; ++i) {
                for (unsigned j = 1; j <= a; j++) {
                    Point pixel(x + i * offset, h - 1 - y + j * offset, 0);
                    Ray ray(eye, (pixel - eye).normalized());
                    col += trace(ray);
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

unsigned Scene::getNumObject()
{
    return objects.size();
}

unsigned Scene::getNumLights()
{
    return lights.size();
}
