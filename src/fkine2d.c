#include <rk/kinematics.h>

#include <stdarg.h>


#if defined __cplusplus
extern "C" {
#endif


rkMat3 _rkForwardKinematics2D(rkLink2D *root, ...) {
    va_list args;
    va_start(args, root);
    
    rkLink2D *link = root;
    rkMat3 T = link->transform;
    
    if(link->startOfChain)
        link++;
    
    do {
        float q = va_arg(args, double);
        if(isnan(q))
            break;
        link->q = q;
        float c = cos(q);
        float s = sin(q);
        link->transform = rkMat3Multiply(
            T,
            (rkMat3) {{
                { c, -s,  0 },
                { s,  c,  0 }
            }}
        );
        T = rkMat3Multiply(
            link->transform,
            (rkMat3) {{
                { 1,  0, link->r },
                { 0,  1,    0    }
            }}
        );
    } while(!(link++)->endOfChain);
    
    if(!link->endOfChain)
        (++link)->transform = T;
    
    va_end(args);
    return T;
}

#if defined __cplusplus
}
#endif
