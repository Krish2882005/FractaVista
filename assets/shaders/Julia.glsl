float fractalFunction(in vec2 z)
{
    float n = 0.0;
    for (int i = 0; i < maxIterations; i++) {
        // Julia iteration: z = z^2 + juliaC
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + juliaC;
        if (dot(z, z) > (escapeRadius * escapeRadius))
            break;
        n += 1.0;
    }
    
    if (n >= float(maxIterations))
        return 0.0;
    
    float smoothCount = n - log2(log2(dot(z, z))) + 4.0;
    return smoothCount;
}
