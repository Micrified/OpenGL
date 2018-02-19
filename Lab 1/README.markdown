Open_GL 1

### Modification List.
1. A helper function was written to produce complex shapes easily. See newCube for an example. 
2. We had difficulty ensuring that our helper functions were drawing the faces in the correct orientations.
3. We added a new vbo/vao for drawing the pyramid, and rewrote our class a bit to allow objects to be setup for rendering anonymously (the function just takes some parameters and handles it). This made it easier to managed the new pyramid. 
4. We added translation matrices in. Made sure to modify our shaders to use them.
5. We added the perspective with a lot of difficulty. The functions aren't easy to understand or use.
6. We ran into integer division problems. 
7. We added rotation matrices and installed them for each object.
8. We added scaling matrices and installed them for all objects.
9. We hooked scaling and rotation up to the widgets. We had problems with forgetting to reset the matrices to identity for some time.
10. We made sure to apply the matrices for scaling and rotation in the right order (before translation and project to rotate about their own axis).
11. We added the sphere and mesh loading methods to help import it. 
12. We added a unitisation method to scale the sphere without having to do it after.

### Key Shortcuts. 
None.

### Bonus Assignments.
Unitisation of a model (in model class) was implemented.

### Feedback.
There is very little mentioned in the documents about using OpenGL's methods. Most of it is confusing to us and still doesn't really make much sense. It just works/magic is usually the explanation.
