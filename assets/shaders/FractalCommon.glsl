#ifndef FRACTAL_COMMON_GLSL
#define FRACTAL_COMMON_GLSL

#define MAX_PALETTE_STOPS 16

layout (local_size_x = 16, local_size_y = 16) in;
layout (rgba8, binding = 0) uniform writeonly image2D destImage;

uniform dvec2 fullResolution;
uniform dvec2 offset;
uniform double zoom;
uniform int maxIterations;
uniform bool useSmoothing;
uniform double paletteFrequency;

#if defined(FRACTAL_JULIA)
uniform dvec2 juliaC;
#endif

struct ColorStop {
    vec3 color;
    float position;
};

layout(std140, binding = 0) uniform Palette {
    int numStops;
    ColorStop stops[MAX_PALETTE_STOPS];
} paletteData;

const double escapeRadius = 4.0;

dvec2 pixelToComplex(in ivec2 pixelCoord)
{
    dvec2 uv = dvec2(
        (double(pixelCoord.x) / fullResolution.x) - 0.5,
        0.5 - (double(pixelCoord.y) / fullResolution.y)
    );
    uv.x *= fullResolution.x / fullResolution.y;
    return offset + uv / zoom;
}

const double LN2_D = 0.693147180559945309417;

double log_d(double x) {
    if (x <= 0.0) return -1.0/0.0; 
    if (x == 1.0) return 0.0;

    int exponent;
    double mantissa = frexp(x, exponent);

    double y = (mantissa - 1.0) / (mantissa + 1.0);
    double y2 = y * y;

    double series_sum = y;
    double term = y;
    term *= y2; series_sum += term / 3.0;
    term *= y2; series_sum += term / 5.0;
    term *= y2; series_sum += term / 7.0;
    term *= y2; series_sum += term / 9.0;

    return double(exponent) * LN2_D + 2.0 * series_sum;
}

double log2_d(double x) {
    return log_d(x) / LN2_D;
}

double fractalFunction(in dvec2 c);

#endif