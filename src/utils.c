#include "ltfat.h"
#include "ltfat/types.h"
#include "ltfat/macros.h"

LTFAT_EXTERN int
LTFAT_NAME_COMPLEX(fftcircshift)( const LTFAT_COMPLEX* in, const ltfatInt L,
                                  const double shift, LTFAT_COMPLEX* out)
{
    double shiftLoc;

    int status = LTFATERR_SUCCESS;
    CHECKNULL(in); CHECKNULL(out);
    CHECK(LTFATERR_BADSIZE, L > 0, "L must be positive");

    shiftLoc = remainder(shift, L);

    if (shiftLoc != 0)
    {
        const ltfat_div_t domod = ltfat_idiv(L, 2);
        double phasefact = -2.0 * M_PI * shiftLoc /  L;
        out[0] = in[0];

        // We do the complex multiplication explicitly, because we want the intermediate
        // results to be doubles until the very end. This function has problems with
        // numerical precision degradation.
        const LTFAT_REAL* inPtr = (const LTFAT_REAL*) in;
        LTFAT_REAL* outPtr = (LTFAT_REAL*) out;

        for (int ii = 1; ii < domod.quot + 1; ii++)
        {
            LTFAT_REAL ar = inPtr[2 * ii];
            LTFAT_REAL ai = inPtr[2 * ii + 1];
            double br = cos(ii * phasefact);
            double bi = sin(ii * phasefact);

            outPtr[2 * ii] = ar * br - ai * bi;
            outPtr[2 * ii + 1] = ar * bi + ai * br;
        }

        for (int ii = 1; ii < domod.quot + domod.rem; ii++)
        {
            LTFAT_REAL ar = inPtr[2 * (L - ii)];
            LTFAT_REAL ai = inPtr[2 * (L - ii) + 1];
            double br = cos(-ii * phasefact);
            double bi = sin(-ii * phasefact);

            outPtr[2 * (L - ii)] =  ar * br - ai * bi;
            outPtr[2 * (L - ii) + 1] = ar * bi + ai * br;
        }
    }
    else
    {
        if (in != out)
            memcpy(out, in, L * sizeof * out);
    }
error:
    return status;
}


LTFAT_EXTERN int
LTFAT_NAME_COMPLEX(fftfftshift)(const LTFAT_COMPLEX* in, const ltfatInt L,
                                LTFAT_COMPLEX* out)
{
    ltfat_div_t domod;
    int status = LTFATERR_SUCCESS;
    domod = ltfat_idiv(L, 2);

    if (domod.rem)
    {
        // There is no Nyquist sample, modulation is by (L-1/L)*pi
        status = LTFAT_NAME_COMPLEX(fftcircshift)(in, L, domod.quot, out);
    }
    else
    {
        CHECKNULL(in); CHECKNULL(out);
        CHECK(LTFATERR_BADSIZE, L > 0, "L must be positive");

        if (in != out)
            for (int ii = 0; ii < L; ii += 2)
                out[ii] = in[ii];

        // There is Nyquist sample. Modulation is exactly by pi i.e. no complex
        // multiplication
        for (int ii = 1; ii < L; ii += 2)
            out[ii] = -in[ii];
    }
error:
    return status;
}

LTFAT_EXTERN int
LTFAT_NAME_COMPLEX(fftifftshift)(const LTFAT_COMPLEX* in, const ltfatInt L,
                                 LTFAT_COMPLEX* out)
{
    ltfat_div_t domod;
    int status = LTFATERR_SUCCESS;
    CHECK(LTFATERR_BADSIZE, L > 0, "L must be positive");
    domod = ltfat_idiv(L, 2);

    if (domod.rem)
        // There is no Nyquist sample, modulation is by -(L-1/L)*pi
        status = LTFAT_NAME_COMPLEX(fftcircshift)(in, L, -domod.quot, out);
    else
        status = LTFAT_NAME_COMPLEX(fftfftshift)(in, L, out);
error:
    return status;
}

LTFAT_EXTERN int
LTFAT_NAME_COMPLEX(fftrealcircshift)( const LTFAT_COMPLEX* in, const ltfatInt L,
                                      const double shift, LTFAT_COMPLEX* out)
{
    ltfat_div_t domod;
    double shiftLoc;
    int status = LTFATERR_SUCCESS;
    CHECKNULL(in); CHECKNULL(out);
    CHECK(LTFATERR_BADSIZE, L > 0, "L must be positive");

    domod = ltfat_idiv(L, 2);
    shiftLoc = remainder(shift, L);

    if (shiftLoc != 0)
    {
        LTFAT_COMPLEX phasefact = -I * (LTFAT_REAL)(2.0 * M_PI * shiftLoc) / ((
                                      LTFAT_REAL) L);

        out[0] = in[0];

        for (int ii = 1; ii < domod.quot + 1; ii++)
            out[ii] = exp((LTFAT_REAL)ii * phasefact) * in[ii];
    }
    else
    {
        if (in != out)
            memcpy(out, in, (domod.quot + 1) * sizeof * out);
    }

error:
    return status;
}

LTFAT_EXTERN int
LTFAT_NAME_COMPLEX(fftrealfftshift)(const LTFAT_COMPLEX* in, const ltfatInt L,
                                    LTFAT_COMPLEX* out)
{
    ltfat_div_t domod;
    int status = LTFATERR_SUCCESS;
    CHECK(LTFATERR_BADSIZE, L > 0, "L must be positive");
    domod = ltfat_idiv(L, 2);

    if (domod.rem)
    {
        // There is no Nyquist sample, modulation is by (L-1/L)*pi
        status = LTFAT_NAME_COMPLEX(fftrealcircshift)(in, L, domod.quot, out);
    }
    else
    {
        CHECKNULL(in); CHECKNULL(out);
        CHECK(LTFATERR_BADSIZE, L > 0, "L must be positive");

        if (in != out)
            for (int ii = 0; ii < domod.quot + 1; ii += 2)
                out[ii] = in[ii];

        // There is Nyquist sample. Modulation is exactly by pi i.e. no complex
        // multiplication
        for (int ii = 1; ii < domod.quot + 1; ii += 2)
            out[ii] = -in[ii];
    }

error:
    return status;
}

LTFAT_EXTERN int
LTFAT_NAME_COMPLEX(fftrealifftshift)(const LTFAT_COMPLEX* in, const ltfatInt L,
                                     LTFAT_COMPLEX* out)
{
    ltfat_div_t domod;
    int status = LTFATERR_SUCCESS;
    CHECK(LTFATERR_BADSIZE, L > 0, "L must be positive");
    domod = ltfat_idiv(L, 2);

    if (domod.rem)
        // There is no Nyquist sample, modulation is by -(L-1/L)*pi
        status = LTFAT_NAME_COMPLEX(fftrealcircshift)(in, L, -domod.quot, out);
    else
        status = LTFAT_NAME_COMPLEX(fftrealfftshift)(in, L, out);

error:
    return status;
}

LTFAT_EXTERN int
LTFAT_NAME(real2complex_array)(const LTFAT_REAL* in, const ltfatInt L,
                               LTFAT_COMPLEX* out)
{
    LTFAT_REAL (*outTmp)[2];
    int status = LTFATERR_SUCCESS;
    CHECKNULL(in); CHECKNULL(out);
    CHECK(LTFATERR_BADSIZE, L > 0, "L must be positive");

    outTmp = (LTFAT_REAL(*)[2])  out;

    if (in == (LTFAT_REAL*)out)
    {
        // Go from the back to avoid overwriting input
        for (ltfatInt ii = L - 1; ii >= 0; ii--)
        {
            outTmp[ii][0] = in[ii];
            outTmp[ii][1] = 0.0;
        }
    }
    else
    {
        for (ltfatInt ii = 0; ii < L; ii++)
        {
            outTmp[ii][0] = in[ii];
            outTmp[ii][1] = 0.0;
        }
    }

error:
    return status;
}

LTFAT_EXTERN int
LTFAT_NAME(complex2real_array)(const LTFAT_COMPLEX* in, const ltfatInt L,
                               LTFAT_REAL* out)
{
    const LTFAT_REAL (*inTmp)[2];
    int status = LTFATERR_SUCCESS;
    CHECKNULL(in); CHECKNULL(out);
    CHECK(LTFATERR_BADSIZE, L > 0, "L must be positive");

    inTmp = (const LTFAT_REAL(*)[2]) in;

    for (ltfatInt ii = 0; ii < L; ii++)
        out[ii] = inTmp[ii][0];

error:
    return status;
}


LTFAT_EXTERN int
LTFAT_NAME_COMPLEX(dgt_phaselock)(const LTFAT_COMPLEX* cFreqinv,
                                  const ltfatInt L, const ltfatInt W, const ltfatInt a, const ltfatInt M,
                                  LTFAT_COMPLEX* cTimeinv)
{
    ltfatInt N;
    int status = LTFATERR_SUCCESS;
    CHECKNULL(cFreqinv); CHECKNULL(cTimeinv);
    CHECK(LTFATERR_NOTPOSARG, L > 0, "L must be positive");
    CHECK(LTFATERR_NOTPOSARG, W > 0, "W must be positive");
    CHECK(LTFATERR_NOTPOSARG, a > 0, "a must be positive");
    CHECK(LTFATERR_NOTPOSARG, M > 0, "M must be positive");

    N = L / a;

    for (ltfatInt w = 0; w < W; w++)
    {
        for (ltfatInt n = 0; n < N; n++)
        {
            const LTFAT_COMPLEX* inCol = cFreqinv + n * M + w * M * N;
            LTFAT_COMPLEX* outCol = cTimeinv + n * M + w * M * N;
            LTFAT_NAME_COMPLEX(fftcircshift)( inCol, M, -n * a, outCol);
        }
    }

error:
    return status;
}

LTFAT_EXTERN int
LTFAT_NAME_COMPLEX(dgtreal_phaselock)(const LTFAT_COMPLEX* cFreqinv,
                                      const ltfatInt L, const ltfatInt W, const ltfatInt a, const ltfatInt M,
                                      LTFAT_COMPLEX* cTimeinv)
{
    ltfatInt N, M2;
    int status = LTFATERR_SUCCESS;
    CHECKNULL(cFreqinv); CHECKNULL(cTimeinv);
    CHECK(LTFATERR_NOTPOSARG, L > 0, "L must be positive");
    CHECK(LTFATERR_NOTPOSARG, W > 0, "W must be positive");
    CHECK(LTFATERR_NOTPOSARG, a > 0, "a must be positive");
    CHECK(LTFATERR_NOTPOSARG, M > 0, "M must be positive");

    N = L / a;
    M2 = M / 2 + 1;

    for (ltfatInt w = 0; w < W; w++)
    {
        for (ltfatInt n = 0; n < N; n++)
        {
            const LTFAT_COMPLEX* inCol = cFreqinv + n * M2 + w * M2 * N;
            LTFAT_COMPLEX* outCol = cTimeinv + n * M2 + w * M2 * N;
            LTFAT_NAME_COMPLEX(fftrealcircshift)( inCol, M, -n * a, outCol);
        }
    }

error:
    return status;
}

LTFAT_EXTERN int
LTFAT_NAME_COMPLEX(dgt_phaseunlock)(const LTFAT_COMPLEX* cTimeinv,
                                    const ltfatInt L, const ltfatInt W, const ltfatInt a, const ltfatInt M,
                                    LTFAT_COMPLEX* cFreqinv)
{
    ltfatInt N;
    int status = LTFATERR_SUCCESS;
    CHECKNULL(cFreqinv); CHECKNULL(cTimeinv);
    CHECK(LTFATERR_NOTPOSARG, L > 0, "L must be positive");
    CHECK(LTFATERR_NOTPOSARG, W > 0, "W must be positive");
    CHECK(LTFATERR_NOTPOSARG, a > 0, "a must be positive");
    CHECK(LTFATERR_NOTPOSARG, M > 0, "M must be positive");

    N = L / a;

    for (ltfatInt w = 0; w < W; w++)
    {
        for (ltfatInt n = 0; n < N; n++)
        {
            const LTFAT_COMPLEX* inCol = cTimeinv + n * M + w * M * N;
            LTFAT_COMPLEX* outCol = cFreqinv + n * M + w * M * N;
            LTFAT_NAME_COMPLEX(fftcircshift)( inCol, M, n * a, outCol);
        }
    }

error:
    return status;
}

LTFAT_EXTERN int
LTFAT_NAME_COMPLEX(dgtreal_phaseunlock)(const LTFAT_COMPLEX* cTimeinv,
                                        const ltfatInt L, const ltfatInt W, const ltfatInt a, const ltfatInt M,
                                        LTFAT_COMPLEX* cFreqinv)
{
    ltfatInt N, M2;
    int status = LTFATERR_SUCCESS;
    CHECKNULL(cFreqinv); CHECKNULL(cTimeinv);
    CHECK(LTFATERR_NOTPOSARG, L > 0, "L must be positive");
    CHECK(LTFATERR_NOTPOSARG, W > 0, "W must be positive");
    CHECK(LTFATERR_NOTPOSARG, a > 0, "a must be positive");
    CHECK(LTFATERR_NOTPOSARG, M > 0, "M must be positive");

    N = L / a;
    M2 = M / 2 + 1;

    for (ltfatInt w = 0; w < W; w++)
    {
        for (ltfatInt n = 0; n < N; n++)
        {
            const LTFAT_COMPLEX* inCol = cTimeinv + n * M2 + w * M2 * N;
            LTFAT_COMPLEX* outCol = cFreqinv + n * M2 + w * M2 * N;
            LTFAT_NAME_COMPLEX(fftrealcircshift)( inCol, M, n * a, outCol);
        }
    }

error:
    return status;
}
