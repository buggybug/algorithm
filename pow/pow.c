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

double my_pow(double base, int iexp)
{
    double res;

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
	 : "0"(base), "u"((double)iexp)
	 : "st(1)");

    return res;
}

