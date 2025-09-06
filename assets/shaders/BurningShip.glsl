float fractalFunction(in dvec2 c)
{
    double n = 0.0;
    dvec2 z = dvec2(0.0, 0.0);

    for (int i = 0; i < maxIterations; i++) {
        double zx = abs(z.x);
        double zy = abs(z.y);
        z = dvec2(zx*zx - zy*zy, 2.0*zx*zy) + c;

        if (dot(z, z) > (escapeRadius * escapeRadius))
            break;

        n += 1.0;
    }

    if (n >= double(maxIterations))
        return 0.0;

    if (useSmoothing) {
        double smooth_val = log2_d(log2_d(dot(z, z)));
        return float(n - smooth_val + 4.0);
    }

    return float(n);
}