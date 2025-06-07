#ifndef FRACTAL_COMMON_GLSL
#define FRACTAL_COMMON_GLSL

#version 430

layout (local_size_x = 16, local_size_y = 16) in;
layout (rgba8, binding = 0) uniform writeonly image2D destImage;

uniform vec2 fullResolution;
uniform vec2 offset;
uniform float zoom; // Represents 1.0 / view_height
uniform int maxIterations;

const float escapeRadius = 4.0;

vec2 pixelToComplex(in vec2 pixelCoord)
{
    vec2 uv = vec2(
        (pixelCoord.x / fullResolution.x) - 0.5,
        0.5 - (pixelCoord.y / fullResolution.y)
    );
    uv.x *= fullResolution.x / fullResolution.y;
    return offset + uv / zoom;
}

#endif // FRACTAL_COMMON_GLSL
