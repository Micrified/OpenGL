#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the inputs to the fragment shader
// These must have the same type and name!
in vec3 vertexNormal;

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

// UNIFORM: Sampler for extracting color from texture.
uniform sampler2D samplerUniform; // TODO: Store uniform location. Delete when done.

void main()
{

    fColor = vec4(normalize(vertexNormal) * 0.5 + 0.5, 1.0);
}
