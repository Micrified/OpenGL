Open_GL 2

### Introduction.

For this assignment, we had to use OpenGL to perform the following.
1. Show normal vectors on a cat mesh.
2. Implement phong shading on the cat.
3. Implement gouraud shading on the cat.
4. Upload and apply textures to the cat in both the phong and gouraud shaders.

### Questions

**Question**: Describe the difference between transforming the normal vectors for the cat vs not doing so when rendered.

When not transforming the normal vectors, the cat only displayed a static/baked set of normals to the viewer at any angle. No visible change occurred as the cat was rotated, flipped, or rolled.
The difference can be seen in the screenshots staticNormals and activeNormals. In activeNormals, the normals are behaving as they should. In static, they are unchanged.

In the screenshot StaticVActiveNormals, the static normals (on the left) can be seen to match or appear similar to the active normals (on the right). This is shown to contrast with it as it changes.


**Question**: Experiment with different wrapping and filter methods in your final application and describe what each one does in your README.

* We experimented with changing `GL_CLAMP_TO_EDGE` with `GL_CLAMP_TO_BORDER`. But we didn´t observe much of a change. The screenshot ClampBorder demonstrates the latter filter.
* We experimented with changing `GL_CLAMP_TO_EDGE` with `GL_DEPTH_CLAMP_NV` and multiplied the texture coordinate point by `(2,2)` as recommended. It seemed to displace the texture over the cat. The face appeared on its back. The screenshot ClampNV shows this.
* 

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
