#version 330 core

// Define constants
#define M_PI 3.141593

/*
 ****************************************************
 *                   Attributes                     *
 ****************************************************
*/

// IN: The 3D vertex color vector.
in vec3 vertexColor;

// IN: The 2D texture coordinate.
in vec2 textCoordinates;

/*
 ****************************************************
 *                    Uniforms                      *
 ****************************************************
*/

// UNIFORM: Sampler for extracting color from texture.
uniform sampler2D samplerUniform; // TODO: Store uniform location. Delete when done.

/*
 ****************************************************
 *                     Output                       *
 ****************************************************
*/

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

void main()
{
    // Combines color (texture2D(...)) with light intensity (vertexColor).
    fColor = texture2D(samplerUniform, textCoordinates) * vertexColor;
    // OLD: fColor = vec4(vertexNormal, 1.0);
}
