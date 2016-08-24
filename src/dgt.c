#include "ltfat.h"
#include "ltfat/types.h"
#include "ltfat/macros.h"


LTFAT_API void
LTFAT_NAME(dgt_ola)(const LTFAT_COMPLEX* f, const LTFAT_COMPLEX* g,
                    const ltfatInt L, const ltfatInt gl,
                    const ltfatInt W, const ltfatInt a, const ltfatInt M,
                    const ltfatInt bl, const ltfat_phaseconvention ptype,
                    LTFAT_COMPLEX* cout)
{
    LTFAT_NAME(dgt_ola_plan) plan =
        LTFAT_NAME(dgt_ola_init)(g, gl, W, a, M, bl, ptype, FFTW_ESTIMATE);

    LTFAT_NAME(dgt_ola_execute)(plan, f, L, cout);

    LTFAT_NAME(dgt_ola_done)(plan);

}


LTFAT_API void
LTFAT_NAME(dgtreal_ola)(const LTFAT_REAL* f, const LTFAT_REAL* g,
                        const ltfatInt L, const ltfatInt gl,
                        const ltfatInt W, const ltfatInt a, const ltfatInt M,
                        const ltfatInt bl, const ltfat_phaseconvention ptype,
                        LTFAT_COMPLEX* cout)
{
    LTFAT_NAME(dgtreal_ola_plan) plan =
        LTFAT_NAME(dgtreal_ola_init)(g, gl, W, a, M, bl, ptype, FFTW_ESTIMATE);

    LTFAT_NAME(dgtreal_ola_execute)(plan, f, L, cout);

    LTFAT_NAME(dgtreal_ola_done)(plan);

}
