#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include "scene.h"
#include <string>

// Symbolic Constants.
#define OBJ_UNDEFINED   -1
#define OBJ_SPHERE      0
#define OBJ_TRIANGLE    1
#define OBJ_PLANE       2
#define OBJ_QUAD        3
#define OBJ_MESH        4

// Forward declerations
class Light;
class Material;

#include "json/json_fwd.h"

class Raytracer
{
    Scene scene;

    public:

        // Provided Public Methods.
        bool readScene(std::string const &ifname);
        void renderToFile(std::string const &ofname);

    private:

        // Helper Private Method for mapping object-type to integer.
        int objectType (std::string const &ofname);

        // Helper Private Methods for loading objects.
        void loadSphere (nlohmann::json const &node, std::vector<ObjectPtr> &sceneObjects);
        void loadTriangle (nlohmann::json const &node, std::vector<ObjectPtr> &sceneObjects);
        void loadPlane (nlohmann::json const &node, std::vector<ObjectPtr> &sceneObjects);
        void loadQuad (nlohmann::json const &node, std::vector<ObjectPtr> &sceneObjects);
        void loadMesh (nlohmann::json const &node, std::vector<ObjectPtr> &sceneObjects);

        // Provided Private Methods.
        bool parseObjectNode(nlohmann::json const &node);

        Light parseLightNode(nlohmann::json const &node) const;
        Material parseMaterialNode(nlohmann::json const &node) const;
};

#endif
