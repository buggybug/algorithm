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

