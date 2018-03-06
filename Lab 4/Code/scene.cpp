#include "scene.h"

#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"

#include <cmath>
#include <limits>

using namespace std;

Hit Scene::firstIntersect(Ray const &ray, ObjectPtr *obj)
{
    // Find hit object and distance
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    *obj = nullptr;

    // Determine the closest object hit in the scene.
    for (unsigned idx = 0; idx != objects.size(); ++idx)
    {
        Hit hit(objects[idx]->intersect(ray));
        if (hit.t < min_hit.t)
        {
            min_hit = hit;
            *obj = objects[idx];
        }
    }
    return min_hit;
}

Color Scene::trace(Ray const &ray)
{
    // Find hit object and distance
    ObjectPtr obj;
    Hit min_hit = firstIntersect(ray, &obj);

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material material = obj->material;          //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector

    Color diffuse;
    Color ambient;
    Color specular;

    Color color = material.color;   
    for(int i = 0; i < lights.size(); i++){
        Vector L = lights[i]->position - hit;

        Ray lightRay(hit, L);
        Hit lightIntersect = firstIntersect(lightRay, &obj);
        Vector I = lights[i]->position - lightRay.at(lightIntersect.t);

        if(lightIntersect.t < L.length() && lightIntersect.t > 0.1) continue;

        Vector R = 2 * (L.dot(N)) * N - L;
        L.normalize();
        R.normalize();

        diffuse += fmax(0,N.dot(L)) * lights[0]->color * material.kd * color;
        ambient += color * material.ka;
        specular += pow(fmax(0, R.dot(V)), material.n) * lights[0]->color * material.ks;
    }
                   // place holder

    return diffuse + ambient + specular;

}

void Scene::render(Image &img)
{
    unsigned w = img.width();
    unsigned h = img.height();
    for (unsigned y = 0; y < h; ++y)
    {
        for (unsigned x = 0; x < w; ++x)
        {
            Point pixel(x + 0.5, h - 1 - y + 0.5, 0);
            Ray ray(eye, (pixel - eye).normalized());
            Color col = trace(ray);
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
