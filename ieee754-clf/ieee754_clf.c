#include <stdint.h>
#include <string.h>

#include "ieee754_clf.h"

#define inf 0x7ff0000000000000
#define minf 0xfff0000000000000

float_class_t classify(double x) {
    union {
        double num;
        uint64_t intnum;
    } u;
    u.num = x;
    if (u.intnum == 0) {
        return Zero;
    }
    if (u.intnum << 1 == 0) {
        return MinusZero;
    }
    if (u.intnum == inf) {
        return Inf;
    }
    if (u.intnum == minf) {
        return MinusInf;
    }
    if ((u.intnum & inf) == inf) {
        return NaN;
    }
    if ((u.intnum & inf) == 0) {
        if (u.intnum >> 63 == 0) {
            return Denormal;
        }
        return MinusDenormal;
    }
    if ((u.intnum >> 63) == 0) {
        return Regular;
    }
    return MinusRegular;
}
