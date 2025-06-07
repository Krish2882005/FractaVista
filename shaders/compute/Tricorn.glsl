float fractalFunction(in vec2 c)
{
    float n = 0.0;
    vec2 z = vec2(0.0);
    for (int i = 0; i < maxIterations; i++) {
        z = vec2(z.x * z.x - z.y * z.y, -2.0 * z.x * z.y) + c;

        if (dot(z, z) > (escapeRadius * escapeRadius))
            break;
        n += 1.0;
    }

    if (n >= float(maxIterations))
        return 0.0;

    float smoothCount = n - log2(log2(dot(z, z))) + 4.0;
    return smoothCount;
}