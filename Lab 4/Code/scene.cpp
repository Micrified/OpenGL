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
        // To avoid grain in the production image, self-collisions are ignored.
        if (idx == ignoringObjectAtIndex) {
            continue;
        }

        // Otherwise, pick closest object based on minimum distance.
        Hit hit(objects[idx]->intersect(ray));
        if (hit.t < min_hit.t)
        {
            min_hit = hit;
            objectIndex = idx;
        }
    }

    // If a pointer was supplied, save the nearest object found at the address of the pointer.
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

    /*
     *************************************************************************
     *                          Extract Material                             *
     *************************************************************************
    */

    ObjectPtr obj = objects[objectIndex];       // The object struck.
    Material material = obj->material;          // The object material.
    Point hit = ray.at(min_hit.t);              // The point hit on the object.
    Vector N = min_hit.N;                       // Normal vector to hit point.
    Vector V = -ray.D;                          // The viewing vector.

    Color diffuse;                              // Diffuse component.
    Color specular;                             // Specular component.
    Color color;                                // Surface color.

    /*
     *************************************************************************
     *                            Extract Color                              *
     *************************************************************************
    */

    // If texture in use, use (u,v) coordinates to extract texture color.
    if (material.isTextured) {
        color = obj->getTextureColorAtPoint(hit);
    } else {
        // Otherwise use regular material color.
        color = material.color;  
    }

    /*
     *************************************************************************
     *                          Compute Illumination                         *
     *************************************************************************
    */

    for(unsigned i = 0; i < lights.size(); i++){

        // Ray from the hit-point to the light.
        Vector L = lights[i]->position - hit;

        // [Copy] Ray from the hit-point to the light.
        Ray lightRay(hit, L);

        // Determine if any object lies in the path of the light ray. Ignore self.
        Hit lightIntersect = firstIntersect(lightRay, NULL, objectIndex);

        // If shadows are enabled, and something intersects the light ray, block it.
        if(hasShadows && lightIntersect.t < L.length()) continue;
        
        // Otherwise compute the reflection vector.
        Vector R = 2 * (L.dot(N)) * N - L;
        L.normalize();
        R.normalize();

        // Add to the diffuse and specular components.
        diffuse += fmax(0,N.dot(L)) * lights[i]->color * material.kd * color;
        specular += pow(fmax(0, R.dot(V)), material.n) * lights[i]->color * material.ks;
        
    }

    // Compute reflection based on view-angle.
    Vector reflect = 2 * (V.dot(N)) * N - V;
    reflect.normalize();

    // Add reflective component based on other objects hit using trace recursively.
    Color reflection = trace(Ray(hit, reflect), depth - 1, objectIndex) * material.ks;

    // Compute ambient component.
    Color ambient = color * material.ka;

    // Return sum of all components.
    return diffuse + ambient + specular + reflection;
}

void Scene::render(Image &img)
{
    // Variable 'a', for 'anti-aliasing', is initialized to our super sampling factor.
    unsigned a = superSamplingFactor;
    unsigned w = img.width();
    unsigned h = img.height();

    // For all pixels in the image.
    for (unsigned y = 0; y < h; ++y)
    {
        for (unsigned x = 0; x < w; ++x)
        {   

            /* Anti-Aliasing: Sample over area and average to smooth. */
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
