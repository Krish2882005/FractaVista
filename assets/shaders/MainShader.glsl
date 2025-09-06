#version 430

#extension GL_ARB_gpu_shader_fp64 : enable

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

// This palette function linearly interpolates between the color stops
// provided in the Palette UBO.
vec3 getPaletteColor(float t) {
    if (paletteData.numStops == 0) return vec3(0.0);
    if (t <= paletteData.stops[0].position) return paletteData.stops[0].color;
    if (t >= paletteData.stops[paletteData.numStops - 1].position) return paletteData.stops[paletteData.numStops - 1].color;

    for (int i = 0; i < paletteData.numStops - 1; i++) {
        ColorStop current = paletteData.stops[i];
        ColorStop next = paletteData.stops[i+1];
        if (t >= current.position && t <= next.position) {
            float range = next.position - current.position;
            float segmentT = (range < 1e-6) ? 0.0 : (t - current.position) / range;
            return mix(current.color, next.color, segmentT);
        }
    }
    return paletteData.stops[paletteData.numStops - 1].color;
}


void main()
{
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);

    if (pixelCoord.x >= int(fullResolution.x) || pixelCoord.y >= int(fullResolution.y))
        return;

    dvec2 coord = pixelToComplex(pixelCoord);
    
    double iter = fractalFunction(coord); 

    vec3 finalColor = vec3(0.0);

    if (iter > 0.0)
    {
        double v = iter * paletteFrequency;

        double t_double = (mod(floor(v), 2.0) == 0.0) ? fract(v) : 1.0 - fract(v);
        
        float t_float = float(t_double);
        
        finalColor = getPaletteColor(t_float);
    }

    imageStore(destImage, pixelCoord, vec4(finalColor, 1.0));
}

/*
if (iter > 0.0)
{
    float v = iter * paletteFrequency;

    // Use a sine wave for smooth, cyclical color
    // The 0.5 * ... + 0.5 maps the [-1, 1] range of sin() to [0, 1]
    float t = 0.5 * sin(v * 2.0 * 3.14159265) + 0.5;

    finalColor = getPaletteColor(t);
}
*/