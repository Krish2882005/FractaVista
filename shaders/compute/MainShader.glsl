#version 430

#ifndef AA
    #define AA 1
#endif

#include "FractalCommon.glsl"

#ifdef FRACTAL_MANDELBROT
    #include "Mandelbrot.glsl"
#elif defined(FRACTAL_JULIA)
    #include "Julia.glsl"
#elif defined(FRACTAL_BURNING_SHIP)
    #include "BurningShip.glsl"
#elif defined(FRACTAL_NEWTON)
    #include "Newton.glsl"
#elif defined(FRACTAL_CUBIC_MANDELBROT)
    #include "CubicMandelbrot.glsl"
#elif defined(FRACTAL_TRICORN)
    #include "Tricorn.glsl"
#else
    #error "No fractal algorithm defined"
#endif

void main()
{
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);

    // Boundary check to exit threads outside the image resolution
    if (pixelCoord.x >= fullResolution.x || pixelCoord.y >= fullResolution.y)
        return;

    vec3 col = vec3(0.0);

#if AA > 1
    for (int m = 0; m < AA; m++) {
        for (int n = 0; n < AA; n++) {
            vec2 subPixel = vec2(pixelCoord) + vec2(float(m), float(n)) / float(AA);
            vec2 coord = pixelToComplex(subPixel);
            float l = fractalFunction(coord);

            col += (l < 0.01)
                   ? vec3(0.0)
                   : 0.5 + 0.5 * cos(3.0 + l * 0.15 + vec3(0.0, 0.6, 1.0));
        }
    }
    col /= float(AA * AA);
#else
    vec2 coord = pixelToComplex(vec2(pixelCoord));
    float l = fractalFunction(coord);
    col = (l < 0.01)
          ? vec3(0.0)
          : 0.5 + 0.5 * cos(3.0 + l * 0.15 + vec3(0.0, 0.6, 1.0));
#endif

    imageStore(destImage, pixelCoord, vec4(col, 1.0));
}
