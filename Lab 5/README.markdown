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
The difference can be seen in the screenshots Normals/staticNormals and Normals/activeNormals. In activeNormals, the normals are behaving as they should. In static, they are unchanged.

In the screenshot Normals/StaticVActiveNormals, the static normals (on the left) can be seen to match or appear similar to the active normals (on the right). This is shown to contrast with it as it changes.


**Question**: Experiment with different wrapping and filter methods in your final application and describe what each one does in your README. All the following screenshots are in /Filters.

* We experimented with changing `GL_CLAMP_TO_EDGE` with `GL_CLAMP_TO_BORDER`. But we didn´t observe much of a change. The screenshot ClampBorder demonstrates the latter filter.
* We experimented with changing `GL_CLAMP_TO_EDGE` with `GL_DEPTH_CLAMP_NV` and multiplied the texture coordinate point by `(2,2)` as recommended. It seemed to displace the texture over the cat. The face appeared on its back. The screenshot ClampNV shows this.
* We experimented with changing `GL_NEAREST` to `GL_LINEAR` and kept our original `GL_CLAMP_TO_EDGE` (along with the texture pointer multiplication) to get the screenshot LinearFilter. It shows a mostly gray cat with some texture on its face.
* We tried the combination `GL_DEPTH_STENCIL_TEXTURE_MODE` with `GL_NEAREST` but ended up getting a screenshot almost identical to `GL_DEPTH_CLAMP_NV`. The screenshot for this combination is titled StencilMode.

### Steps.
1. We began by implementing the normal shader. To do this, we just reused our old assignments color coordinates with those of the normal vectors. We then passed this to our vertex shader along with a normal transform uniform, applied the transform, and passed it to the fragment shader for rendering.
2. Once completing the normal vertex and fragment shaders, we copied them twice and renamed them for the phong and gouraud shaders.
3. We implemented switching by keeping a pointer to the active shader as a global variable in the mainview class. Upon changing the selected button in the UI, we simply set the pointer to the correct shader and called for an update. We used crude colors to differentiate between gouraud and phong at this stage when testing it.
4. We first began by implementing gouraud. To do this, we made sure to add a light and material uniform in all our vertex shaders (because we didn´t want to have to write custom paint code for each shader). We then implemented the illumination model within the vertex shader before passing the illumination constant to the fragment shader where we applied. We did have some confusion when our light source seemed to transform inversely to cat on all axes. However, we then found out that the perspective matrix was being included in the computations where it should not have been. After solving this the gouraud shading seemed to work.
5. We then implemented the phong shader. We decided to try and do most of the adjustments within the vertex shader, and then pass the adjusted coordinate for the light point, vertex, and normal vector to the fragment shader. The fragment shader then computed the illumination constant and applied to all fragments.
6. We began to implemented textures by changing our vertex type to support two extra floats. The floats were labeled `tx` and `ty`, for ẗhe x and y coordinates of the texture. We then updated our attributes in mainview to support these two extra points. Finally, we added another location variable inside all our vertex shaders to store the texture coordinate.
7. We followed this up by adding a `GLuint` location/pointer to hold our texture. Then we copied in the image-to-bytes method supplied, wrote our own custom method for setting up the texture and uploading it. Finally, we added the layout uniform in the fragment shader and made sure to initialize a location for it for all shader programs.
8. We experimented with different filters.

### Key Shortcuts. 
None.

### Bonus Assignments.
Unitisation of a model (in model class) was implemented (again). 

### Feedback.
There is very little mentioned in the documents about using OpenGL's methods. Most of it is confusing to us and still doesn't really make much sense. It just works/magic is usually the explanation.
