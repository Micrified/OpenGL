Open_GL 2

### Introduction.

For the final OpenGL assignment, we were required to:
1. Add common animation to the scene.
2. Animate a complete scene with at least two different meshes, two different texture images, and two instances of each mesh each animated in their own way.
3. Produce a basic water shader using sin waves.
4. Animate water shader.

This particular README is only concerned with objectives 1 and 2.

### Adding Common Animation

Adding a common animation to the scene was straightforward and accomplished as instructed, by applying a repeated translation to a mesh each time the paint() method is called.

### Animating a Complete Scene

Our final scene is composed of the following objects.
1. A sphere (Earth), rotating slowly on it's axis.
2. A sphere (Moon), orbiting the Earth sphere with it's own rotation.
3. An asteroid orbiting the moon, while the moon orbits the Earth.
4. An asteroid flying over the Earth towards the camera, before restarting behind the Earth.

The following changes were made in order to achieve this scene.
* To allow all these meshes to be handled easily, we created a class named `ModelObject` to hold each mesh's texture pointer, material, VBO/VAO buffer pointers, translation/rotation/scaling matrices, animation details, and transform function (most importantly).
* We modified `MainView` to include meta-transformations like the scene rotation, scale, and translation.
* We added a `sceneObjects` array to `MainView` to contain all our `ModelObject` classes to be rendered.
* We modified `PaintGL` in `MainView` to render all objects in a loop.
* We modified `MainView` to setup all our objects and assign them transform functions.

The most important aspect of our changes were the transform functions, which describe how an object moves between calls to `paintGL`. All transform functions are written in the `ModelObject` class as standard functions (they are not instance or class functions of `ModelObject`). We write a function for each object we wish to transform, each only requiring a time parameter and pointer to a `ModelObject` instance. When setting up our `ModelObject` instances, we assign to their `transform` variable the function we want it to use. Within the `paintGL` loop, we have only to invoke the function saved in the object to have it transform itself.
We finally touched off by modifying our `setScale` and `setRotation` methods in `MainView` to apply changes to the scene's scale and rotation matrices. These matrices are then applied along with each scene object's individual transformations in order to uniformly scale and rotate the scene.

### Key Shortcuts.
None.

### Bonus Assignments.
Unitisation of a model (in model class) was implemented (again).
