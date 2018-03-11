## Report

### Shadows

Shadows were implemented with the `trace` method of the `Scene` class. In order to produce shadows, we determined whether light rays incident from a source intersected any other objects prior to the object being hit in question. If any did, then a shadow would be produced by not adding the light rays diffuse and specular components to our overall sum. 

We did encounter some anomalies when we computed this originally, which produced odd looking grain effects in our scene. This was solved by ignoring self-intersections when determining whether a shadow should appear. 

### Reflections

Reflections are computed within the `trace` method of the `Scene` class as well, and contribute towards the total illumination like shadows do. We implemented this just as suggested in the lab assignment, by recursively using the `trace` method to add illumination components from reflected rays (which also depends on the material). We had to edit our `trace` method a bit to include a `depth` parameter though.

### [Optional] Refraction
Omitted.

### [Optional] Blurred Reflection
Omitted.

### Anti-aliasing

Super-sampling was computed in the `render` method of the `Scene` class. Depending on a anti-aliasing factor (E.G 2x, 4x, 8x), we used a loop and a computed offset to add the color components of nearby rays to a total. We then averaged our result to obtain an average color. This helped smooth the image a bit, but wasn't as effective as we thought it would be when scaled up. 

We also added, (as instructed), a configurable option for anti-aliasing in the scene files.

### Texturing

Texture mapping, at least for the sphere, was implemented within the `Sphere` class. We added a method called `getTextureColorAtPoint` in order to return the color of the texture at the given hit point on the sphere object.

Just like previous objectives in the assignment, we used the forumla from the Fundamentals of Computer Graphics book (chapter on 2D Texture Mapping) to create a (u,v) reflectance from the coordinate system of the texture to that of a sphere (polar). We made sure to not allow for non-negative theta as well. 

For the implementation of reflectance, we used Rodrigues's rotation forumula. The formula depends on a rotation vector and angle, which we added to the the `Sphere` class as properties, and added loader methods to fetch these properties in the `raytracer.cpp` which contains the scene loading methods. We had to program in an exception for rotations about the z axis `(0,0,1)` though (rotations about our chosen "unit" rotation vector), so that it simply returned the vector from the sphere to the hit point instead of the rotated one.

### Normal/Bump Mapping

Omitted.