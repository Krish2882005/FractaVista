double fractalFunction(in dvec2 c)
{
    double c2 = dot(c, c);
    if (256.0 * c2 * c2 - 96.0 * c2 + 32.0 * c.x - 3.0 < 0.0)
        return 0.0;
    if (16.0 * (c2 + 2.0 * c.x + 1.0) - 1.0 < 0.0)
        return 0.0;

    double n = 0.0; 
    dvec2 z = dvec2(0.0);

    for (int i = 0; i < maxIterations; i++) {
        z = dvec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        if (dot(z, z) > (escapeRadius * escapeRadius))
            break;
        n += 1.0;
    }

    if (n >= double(maxIterations))
        return 0.0; 

    if (useSmoothing) {
        double smooth_val = log2_d(log2_d(dot(z, z)));
        
        return n - smooth_val + 4.0;
    }

    return n; 
}