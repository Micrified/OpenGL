Raytracer 1

###Modification List

1. We implemented the intersect function in sphere, returning the normal and the distance.

2. We added Phongs lighting model in the function "trace" in scene. It adds lighting from all light sources.

3. We cleaned up the raytracer class to make it easier to add new shapes and meshes.

4. We added triangle rendering, triangles are defined as 3 points with the type "triangle" in json.

5. We added plane rendering, a plane is defined as a point and a normal vector with the type "plane" in json.
We added quad rendering, a quad is defined as 4 points with the type "quad" in json. To render a quad, we simply treat it as 2 triangles.
6. We added mesh rendering, a mesh is defined by the location of the mesh and has the type "mesh" in json, the mesh also has a scale and xyz  values for translation. We treat every 3 points from the mesh as individual triangles. 


###

Bonus Assignments

None