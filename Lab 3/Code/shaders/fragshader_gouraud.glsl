#version 330 core

// Define constants
#define M_PI 3.141593

/*
********************************************************************************
*                                   Uniforms                                   *
********************************************************************************
*/

// Sampler Uniform:
uniform sampler2D samplerUniform;

/*
********************************************************************************
*                              Imported Variables                              *
********************************************************************************
*/

// Import: The illumination constant.
in float i;

// Import: The texture coordinate.
in vec2 fragmentTextureCoordinate;

/*
********************************************************************************
*                              Exported Variables                              *
********************************************************************************
*/

// Export: Fragment shader color (RGBA).
out vec4 fColor;

/*
********************************************************************************
*                                    Main                                      *
********************************************************************************
*/

void main ()
{

    // Get texture color.
    vec4 textureColor = texture2D(samplerUniform, fragmentTextureCoordinate);

    // Compute the color as a function of the transformed normal vector.
    fColor = textureColor * i;

}
