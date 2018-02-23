#include "raytracer.h"

#include "image.h"
#include "light.h"
#include "material.h"
#include "triple.h"

// =============================================================================
// -- Include all your shapes here ---------------------------------------------
// =============================================================================

#include "shapes/sphere.h"
#include "shapes/triangle.h"
#include "shapes/quad.h"
#include "shapes/plane.h"
#include "objloader.h"

// =============================================================================
// -- End of shape includes ----------------------------------------------------
// =============================================================================

#include "json/json.h"

#include <exception>
#include <fstream>
#include <iostream>

using namespace std;        // no std:: required
using json = nlohmann::json;

// =============================================================================
// -- Helper Methods for loading objects ------------------------------
// =============================================================================

// Prepares a sphere object for the scene.
void Raytracer::loadSphere (json const &node, vector<ObjectPtr> &sceneObjects) {
    Point p(node["position"]);
    double r = node["radius"];
    sceneObjects.push_back(ObjectPtr(new Sphere(p, r)));
}

// Prepares a triangle object for the scene.
void Raytracer::loadTriangle (json const &node, vector<ObjectPtr> &sceneObjects) {
    Point a(node["point_a"]);
    Point b(node["point_b"]);
    Point c(node["point_c"]);
    sceneObjects.push_back(ObjectPtr(new Triangle(a, b, c)));
}

// Prepares a plane object for the scene.
void Raytracer::loadPlane (json const &node, vector<ObjectPtr> &sceneObjects) {
    Point a(node["point_a"]);
    Vector n(node["normal"]);
    sceneObjects.push_back(ObjectPtr(new Plane(a, n)));
}

// Prepares a quad object for the scene.
void Raytracer::loadQuad (json const &node, std::vector<ObjectPtr> &sceneObjects) {
    cerr << "node" << "\n";
    Point a(node["point_a"]);
    Point b(node["point_b"]);
    Point c(node["point_c"]);
    Point d(node["point_d"]);
    sceneObjects.push_back(ObjectPtr(new Quad(a, b, c, d)));
}

// Prepares a model object for the scene.
void Raytracer::loadMesh (json const &node, vector<ObjectPtr> &sceneObjects) {
    int s = 60, dx = 300, dy = 300, dz = 100;

    // 1. Obtain file name. Load in object model.
    string filePath = node["model"];
    OBJLoader *model = new OBJLoader(filePath);

    // 2. Extract vertex data and normal data.
    vector<Vertex> vs = model->vertex_data();
    for (unsigned i = 0; i < vs.size(); i += 3) {

        // Extract three points as type Vertex.
        Vertex a = vs[i], b = vs[i + 1], c = vs[i + 2];

        // Convert to points with optional adjustments.
        Point p = Triple(a.x * s + dx, a.y * s + dy, a.z * s + dz);
        Point q = Triple(b.x * s + dx, b.y * s + dy, b.z * s + dz);
        Point r = Triple(c.x * s + dx, c.y * s + dy, c.z * s + dz); 
        
        // Construct a triangle and push it to the scene object buffer.
        ObjectPtr obj = ObjectPtr(new Triangle(p, q, r));
        sceneObjects.push_back(obj);
    }
}

bool Raytracer::parseObjectNode(json const &node)
{
    // Vector of objects to be added to the scene. 
    std::vector<ObjectPtr> sceneObjects;

// =============================================================================
// -- Determine type and parse object parametrers ------------------------------
// =============================================================================

    // 1. Obtain the object-type string.
    string objectTypeString = node["type"];

    // 2. Load the specified objects.
    switch (objectType(objectTypeString)) {
        case OBJ_SPHERE: loadSphere(node, sceneObjects); break;
        case OBJ_TRIANGLE: loadTriangle(node, sceneObjects); break;
        case OBJ_PLANE: loadPlane(node, sceneObjects); break;
        case OBJ_QUAD: loadQuad(node, sceneObjects); break;
        case OBJ_MESH: loadMesh(node, sceneObjects); break;
        default: cerr << "Unknown object type: \"" << objectTypeString << "\".\n";
    }

// =============================================================================
// -- End of object reading ----------------------------------------------------
// =============================================================================

    // Parse material and add objects to the scene
    Material material = parseMaterialNode(node["material"]);
    for (unsigned i = 0; i < sceneObjects.size(); i++) {
        ObjectPtr obj = sceneObjects[i];
        obj->material = material;
        scene.addObject(obj);
    }

    return true;
}

Light Raytracer::parseLightNode(json const &node) const
{
    Point pos(node["position"]);
    Color col(node["color"]);
    return Light(pos, col);
}

Material Raytracer::parseMaterialNode(json const &node) const
{
    Color color(node["color"]);
    double ka = node["ka"];
    double kd = node["kd"];
    double ks = node["ks"];
    double n  = node["n"];
    return Material(color, ka, kd, ks, n);
}

// Custom Method which maps a object--type-string to an integer. 
int Raytracer::objectType (std::string const &ofname) {
    if (ofname.compare("sphere") == 0) return OBJ_SPHERE;
    if (ofname.compare("triangle") == 0) return OBJ_TRIANGLE;
    if (ofname.compare("plane") == 0) return OBJ_PLANE;
    if (ofname.compare("quad") == 0) return OBJ_QUAD;
    if (ofname.compare("mesh") == 0) return OBJ_MESH;
    return OBJ_UNDEFINED;
}

bool Raytracer::readScene(string const &ifname)
try
{
    // Read and parse input json file
    ifstream infile(ifname);
    if (!infile) throw runtime_error("Could not open input file for reading.");
    json jsonscene;
    infile >> jsonscene;

// =============================================================================
// -- Read your scene data in this section -------------------------------------
// =============================================================================

    Point eye(jsonscene["Eye"]);
    scene.setEye(eye);

    // TODO: add your other configuration settings here

    for (auto const &lightNode : jsonscene["Lights"])
        scene.addLight(parseLightNode(lightNode));

    unsigned objCount = 0;
    for (auto const &objectNode : jsonscene["Objects"])
        if (parseObjectNode(objectNode))
            ++objCount;

    cout << "Parsed " << objCount << " objects.\n";

// =============================================================================
// -- End of scene data reading ------------------------------------------------
// =============================================================================

    return true;
}
catch (exception const &ex)
{
    cerr << ex.what() << '\n';
    return false;
}

void Raytracer::renderToFile(string const &ofname)
{
    // TODO: the size may be a settings in your file
    Image img(400, 400);
    cout << "Tracing...\n";
    scene.render(img);
    cout << "Writing image to " << ofname << "...\n";
    img.write_png(ofname);
    cout << "Done.\n";
}
