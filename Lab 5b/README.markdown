Open_GL 2

### Introduction.

For the final OpenGL assignment, we were required to:
1. Add common animation to the scene.
2. Animate a complete scene with at least two different meshes, two different texture images, and two instances of each mesh each animated in their own way.
3. Produce a basic water shader using sin waves.
4. Animate water shader.

This particular README is only concerned with objectives 3 and 4.

### Height Map

We completed the height map assignment as instructed.
1. The `grid.obj` file was downloaded and loaded in.
2. A new shader was created and configured to output it's color as the UV coordinates of the grid. We used a sin function to determine the vertex points.
3. We made sure to apply our other transformations.

The resulting screenshot is titled `height_map.png`.

### Normals

To derive correct normals, we:
1. Updated our vertex shader to send the normals to the fragment shader.
2. Computed the derivative of our function to send to the fragment shader as the new normal.

We had some trouble when we forgot to apply the normal transformation matrix as we rotated the scene. But caught that eventually.
The resulting screenshot is titled `normal_map.png`.

### Multiple Waves

Implementing multiple waves was fairly easy to do.

1. We added a constant `N_WAVES` in both our `MainView` class and vertex shader to describe the number of waves to account for.
2. We added three 2D `float` arrays to our `MainView` header. One for amplitude (`amplitudes`), frequencies (`frequencies`), and phases (`phases`).
3. We added a corresponding `GLuint` pointers to our `ShaderLocationSet` structs to represent them in the vertex shaders, and added corresponding uniforms in the shaders to receive them.
4. We made sure our `setupShaderProgram` method initialized our new uniforms.
5. We added instructions to upload our frequency, amplitude, and phase array data to the vertex shaders in `paintGL`.
6. We implemented `waveHeight(int waveIdx, float uvalue)` and `waveDU(int waveIdx, float uvalue)` to compute the height and derivative of each wave respectively.
7. We added a loop to go though and compute our `currentVertexPosition` as the sum of all wave heights. The derivative of each wave was also summed (and is mathematically correct to do so) to be used later.
8. We applied our transformations to the new vertex position (with z height altered previously) and compute the adjusted normal vector with our derivative sum.

Our result for multiple waves (with normals for colors) is show in the screenshot titled `multiple_normal_waves.png`.
We then reimplemented phong shading with the following decisions:

9. We decided we'd use the `smoothstep` function to produce a "degree" of color within our wave range (we chose a range slightly smaller than the maximum and minimum wave amplitude possible. That was computed by summing all amplitudes).
10. Using the output from the `smoothstep` function, we used `mix` to pick a color between pure blue and pure white.
11. Our final color output was the color vector determined using `mix` multiplied by our illumination (phong shading) value.

We eventually adjusted our `smoothstep` function so that the range was larger than that of our waves. This was to avoid having to show the extreme whites and blues and helped achieve a result more like that shown in the assignment.

The resulting screenshot is titled `multiple_phong_waves.ong`.

### Animating Water

To animate water, we added a `float` uniform (`elapsedTimeUniform`) to our vertex shaders, and used it to provide a time value which was then used to compute the height and derivative of our vertex points within the `waveHeight` and `waveDU` functions.
The `elapsedTimeUniform` was represented by: `static double elapsedTime` within `paintGL` which persists between function calls.
During every call to `paintGL`, the time was incremented by one-sixtith. This was done in order to match the timer's refresh rate of 60 FPS.

### Issues.
None.

### Key Shortcuts.
None.

### Bonus Assignments.
None.
