#undef NDEBUG
#ifndef _minunit_h
#define _minunit_h
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int tests_run;

typedef struct
{
    char** failedTests;
    int failedTestsLength;
    int noOfFailedTests;
} failedTests;

failedTests ft = {0};

// char** failedTests = NULL;
// int failedTestsLength = 0;
// int noOfFailedTests = 0;

void test_error_handler (int ltfat_errno, const char* file, int line,
                         const char* funcname, const char* reason)
{
    fprintf (stderr, "    [ERROR %d]: (%s:%d:): [%s]: %s\n", -ltfat_errno, file, line,
             funcname, reason);

    fflush (stderr);
}

#define ARRAYLEN(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#define mu_suite_start() int message = 0; do { ltfat_set_error_handler(test_error_handler); } while(0)

#define mu_suite_stop() do{ fftw_cleanup(); fftwf_cleanup(); } while(0)

#define mu_assert(test, ...) do{ printf("    "); printf(__VA_ARGS__);  if (!(test)) { printf("    <--- FAILED"); return 1; }  printf("\n"); }while(0)


#define mu_run_test_singledouble(test) mu_run_test(test##_d); mu_run_test(test##_s);
#define mu_run_test_singledoublecomplex(test) mu_run_test(test##_d); mu_run_test(test##_s); mu_run_test(test##_dc); mu_run_test(test##_sc);
#define mu_run_test(test) printf("\n--- %s ---\n", #test); \
    message = test(); tests_run++; if (message) { ft.noOfFailedTests++;  ADDTOFAILEDTESTS(#test); message = 0; }

#define RUN_TESTS(name) int main() {\
    printf("---------------- RUNNING: " __FILE__ " ----------------\n");\
        name();\
        if (ft.noOfFailedTests>0){\
            printf("\n----------------\nFAILED TESTS %d: \n\n", ft.noOfFailedTests);\
            for(int ii=0;ii< ft.noOfFailedTests;ii++) { printf("    %s\n", ft.failedTests[ii]); }\
            ltfat_free(ft.failedTests); }\
        else { printf("\n----------------\nALL TESTS PASSED\n"); }}




#define ADDTOFAILEDTESTS(string) \
do{ \
    if( ft.noOfFailedTests > ft.failedTestsLength){ \
        ft.failedTests = ltfat_realloc( ft.failedTests, ft.failedTestsLength*sizeof(char*), 2*ft.noOfFailedTests*sizeof(char*));\
        ft.failedTestsLength = 2*ft.noOfFailedTests; } \
    ft.failedTests[ft.noOfFailedTests-1] = #string; \
}while(0)


void fillRand_d(double *in, int L);
void fillRand_s(float *in, int L);
void fillRand_dc(double _Complex *in, int L);
void fillRand_sc(float _Complex *in, int L);


/*
Fills array with pseudorandom values in range [0-1]
*/
void fillRand_d(double* f, int L)
{
    srand (time(NULL));

    for (int m = 0; m < L; m++)
    {
        f[m] = ((double) (rand())) / RAND_MAX;
    }
}

void fillRand_s(float* f, int L)
{
    srand (time(NULL));

    for (int m = 0; m < L; m++)
    {
        f[m] = ((float) (rand())) / RAND_MAX;
    }
}

void fillRand_dc(double _Complex* f, int L)
{
    double (*fTmp)[2]  = (double (*)[2]) f;
    srand (time(NULL));

    for (int m = 0; m < L; m++)
    {
        fTmp[m][0] = ((double) (rand())) / RAND_MAX;
        fTmp[m][1] = ((double) (rand())) / RAND_MAX;
    }
}

void fillRand_sc(float _Complex* f, int L)
{
    float (*fTmp)[2] = (float (*)[2]) f;
    srand (time(NULL));

    for (int m = 0; m < L; m++)
    {
        fTmp[m][0] = ((float) (rand())) / RAND_MAX;
        fTmp[m][1] = ((float) (rand())) / RAND_MAX;
    }
}



#define LTFAT_DOUBLE
#include "ltfat/types.h"
#define TEST_NAME(name) name##_d
#define TEST_NAME_COMPLEX(name) name##_dc

#include "test_typecomplexindependent.c"
#include "test_typeindependent.c"

#undef TEST_NAME
#undef TEST_NAME_COMPLEX
#undef LTFAT_DOUBLE

#define LTFAT_SINGLE
#include "ltfat/types.h"
#define TEST_NAME(name) name##_s
#define TEST_NAME_COMPLEX(name) name##_sc

#include "test_typecomplexindependent.c"
#include "test_typeindependent.c"


#undef TEST_NAME
#undef TEST_NAME_COMPLEX
#undef LTFAT_SINGLE

#define LTFAT_COMPLEXTYPE

#define LTFAT_DOUBLE
#include "ltfat/types.h"
#define TEST_NAME(name) name##_dc
#define TEST_NAME_COMPLEX(name) name##_dc

#include "test_typecomplexindependent.c"

#undef TEST_NAME
#undef TEST_NAME_COMPLEX
#undef LTFAT_DOUBLE

#define LTFAT_SINGLE
#include "ltfat/types.h"
#define TEST_NAME(name) name##_sc
#define TEST_NAME_COMPLEX(name) name##_sc


#include "test_typecomplexindependent.c"

#undef TEST_NAME
#undef TEST_NAME_COMPLEX
#undef LTFAT_SINGLE
#undef LTFAT_COMPLEXTYPE

// Unsets all the macros 
#include "ltfat/types.h"




#endif
