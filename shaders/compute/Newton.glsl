vec2 complexAdd(vec2 a, vec2 b) { return a + b; }
vec2 complexSub(vec2 a, vec2 b) { return a - b; }
vec2 complexMul(vec2 a, vec2 b) {
    return vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}
vec2 complexDiv(vec2 a, vec2 b) {
    float den = dot(b, b); 
    if (den == 0.0) return vec2(0.0); 
    return vec2(
        (a.x * b.x + a.y * b.y) / den,
        (a.y * b.x - a.x * b.y) / den
    );
}
vec2 complexSq(vec2 z) { return complexMul(z, z); }
vec2 complexCube(vec2 z) { return complexMul(complexSq(z), z); }

float fractalFunction(in vec2 c)
{
    float n = 0.0;
    vec2 z = c; 
    vec2 z_prev;

    const float CONVERGENCE_THRESHOLD_SQ = 0.000001; 

    for (int i = 0; i < maxIterations; i++) {
        z_prev = z; 

        vec2 f_z = complexSub(complexCube(z), vec2(1.0, 0.0));

        vec2 f_prime_z = complexMul(vec2(3.0, 0.0), complexSq(z));

        if (dot(f_prime_z, f_prime_z) < CONVERGENCE_THRESHOLD_SQ * 100.0) { 
            n = float(maxIterations); 
            break;
        }

        vec2 delta_z = complexDiv(f_z, f_prime_z);

        z = complexSub(z, delta_z);

        if (dot(z - z_prev, z - z_prev) < CONVERGENCE_THRESHOLD_SQ) {
            n += 1.0; 
            break;
        }

        n += 1.0;
    }

    if (n >= float(maxIterations)) {
        return 0.0; 
    } else {
        return n; 
    }
}