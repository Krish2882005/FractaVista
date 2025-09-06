float fractalFunction(in vec2 c)
{
    float n = 0.0;
    vec2 z = vec2(0.0);
    for (int i = 0; i < maxIterations; i++) {
        float zx_squared = z.x * z.x;
        float zy_squared = z.y * z.y;

        float next_zx = z.x * zx_squared - 3.0 * z.x * zy_squared;
        float next_zy = 3.0 * zx_squared * z.y - z.y * zy_squared;

        z = vec2(next_zx, next_zy) + c;

        if (dot(z, z) > (escapeRadius * escapeRadius))
            break;
        n += 1.0;
    }

    if (n >= float(maxIterations))
        return 0.0;

    float smoothCount = n - log2(log2(dot(z, z))) + 4.0;
    return smoothCount;
}