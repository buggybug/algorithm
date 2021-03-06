// Algorithm description:
// Function pow(a, b) can be interpreted as pow(2, b*log(a)), where log stands
// for binary logarithm. The only problem is that the exponent can be non-
// integral, so it should be split into two parts: integral part and float part
// i.e. the number that varies in range [-1; 1].
//
// In terms of FPU available commands, the solution is the next:
// 1) calculate b*log(a) using integrated FPU command
// 2) split previos operation's result into integral and float parts
// 3) perform a fast scale for the integral part
// 4) execute low-level FPU function pow(2, x) - 1, where operand x lies in the
// range [-1; 1]
// 5) multiply the results
//
// Also we make sure that the FPU stack is correctly cleaned-up, i.e. we popped
// input parameters and just a top of the stack is used (to store the result
// value)

#include <math.h>

// Assembly implementation provided for GCC only
#ifdef __GNUC__
#if defined(__i386__) || defined(__amd64__)
#define MY_POW_FPU_IMPL 1
#endif // defined(__i386__) || defined(__amd64__)
#endif // __GNUC__

double my_pow(double base, int iexp)
{
    const double pos_base = fabs(base);
    double res;

#ifdef MY_POW_FPU_IMPL

    asm ("fyl2x;"
         "fld1;"
         "fscale;"
         "fxch %%st(1);"
         "fld1;"
         "fxch %%st(1);"
         "fprem;"
         "f2xm1;"
         "faddp;"
         "fmulp;"
         : "=t"(res)
         : "0"(pos_base), "u"((double)iexp)
         : "st(1)", "st(7)");

#else

    // C library does not provide fyl2x equivalent
    const long double exp_arg = iexp * log2l(pos_base);

    res = (double)(ldexpl(1.0, (int)floorl(exp_arg)) *
                   exp2l(fmodl(exp_arg, 1.0)));

#endif // MY_POW_FPU_IMPL

    if ((base < 0) && (iexp & 1))
        res *= -1;

    return res;
}

