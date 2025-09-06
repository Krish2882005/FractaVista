float fractalFunction(in vec2 c)
{
    float n = 0.0; 

    vec2 z = c;

    for (int i = 0; i < maxIterations; i++) {
        
        if (dot(z, z) > 1000.0) {
            break;
        }

        vec2 z2 = z * z;
        vec2 z3 = z * (vec2(z2.x - z2.y) + vec2(-2.0, 2.0) * z2.yx);
        vec2 w = vec2(1.0 + z2.x, z2.y);
        z = (w.x * z3 + w.y * vec2(z3.y, -z3.x)) / dot(w, w) + c;
        
        n += 1.0;
    }

    if (n >= float(maxIterations)) {
        return 0.0;
    }
    
    float smoothCount = n - log2(log2(dot(z, z))) + 4.0;
    return smoothCount;
}