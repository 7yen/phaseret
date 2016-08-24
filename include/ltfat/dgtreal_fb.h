typedef struct LTFAT_NAME(dgtreal_fb_plan) LTFAT_NAME(dgtreal_fb_plan);

/** \defgroup dgtrealfb Discrete Gabor Transform for real signals and FIR windows -- dgtreal_fb
 *  \addtogroup dgtrealfb
 * @{
 * For a detailed description see the dedicated page \ref dgttheory
 */

/** Compute Discrete Gabor Transform for real signals using filter bank algorithm
 *
 * \param[in]     f   Input signal, size L x W
 * \param[in]     g   Window, size gl x 1
 * \param[in]     L   Signal length
 * \param[in]    gl   Window length
 * \param[in]     W   Number of channels of the signal
 * \param[in]     a   Time hop factor
 * \param[in]     M   Number of frequency channels
 * \param[in] ptype   Phase convention
 * \param[out]    c   DGT coefficients, size M2 x N x W
 *
 * #### Versions #
 * <tt>
 * ltfat_dgtreal_fb_d(const double f[], const double g[],
 *                    const ltfatInt L, const ltfatInt gl,
 *                    const ltfatInt W,  const ltfatInt a, const ltfatInt M,
 *                    const ltfat_phaseconvention ptype, complex double c[]);
 *
 * ltfat_dgtreal_fb_s(const float f[], const float g[],
 *                    const ltfatInt L, const ltfatInt gl,
 *                    const ltfatInt W,  const ltfatInt a, const ltfatInt M,
 *                    const ltfat_phaseconvention ptype, complex float c[]);
 * </tt>
 * \returns
 * Status code              | Description
 * -------------------------|--------------------------------------------
 * LTFATERR_SUCCESS         | Indicates no error
 * LTFATERR_NULLPOINTER     | At least one of the following was NULL: \a f, \a g, \a c
 * LTFATERR_BADSIZE         | Length of the signal \a L or the length of the window \a gl was less or equal to 0.
 * LTFATERR_NOTPOSARG       | At least one of the following was less or equal to zero: \a W, \a a, \a M
 * LTFATERR_BADTRALEN       | \a L must be bigger of equal to \a gl and must be divisible by \a a
 * LTFATERR_INITFAILED      | FFTW plan creation failed
 * LTFATERR_CANNOTHAPPEN    | \a ptype does not have a valid value from the ltfat_phaseconvention enum
 * LTFATERR_NOMEM           | Indicates that heap allocation failed
 */
LTFAT_API int
LTFAT_NAME(dgtreal_fb)(const LTFAT_REAL f[], const LTFAT_REAL g[],
                       const ltfatInt L, const ltfatInt gl,
                       const ltfatInt W,  const ltfatInt a, const ltfatInt M,
                       const ltfat_phaseconvention ptype, LTFAT_COMPLEX c[]);

/** Initialize plan for Discrete Gabor Transform for real signals for the filter bank algorithm
 *
 * \param[in]     g   Window, size gl x 1
 * \param[in]    gl   Window length
 * \param[in]     a   Time hop factor
 * \param[in]     M   Number of frequency channels
 * \param[in] ptype   Phase convention
 * \param[in] flags   FFTW plan flags
 * \param[out] plan   DGT plan
 *
 * #### Versions #
 * <tt>
 * ltfat_dgtreal_fb_init_d(const double g[], const ltfatInt gl, const ltfatInt a,
 *                         const ltfatInt M, const ltfat_phaseconvention ptype, unsigned flags
 *                         ltfat_dgtreal_fb_plan_d** plan);
 *
 * ltfat_dgtreal_fb_init_s(const float g[], const ltfatInt gl, const ltfatInt a,
 *                         const ltfatInt M, const ltfat_phaseconvention ptype,unsigned flags
 *                         ltfat_dgtreal_fb_plan_s** plan);
 * </tt>
 * \returns
 * Status code              | Description
 * -------------------------|--------------------------------------------
 * LTFATERR_SUCCESS         | Indicates no error
 * LTFATERR_NULLPOINTER     | At least one of the following was NULL: \a g, \a plan
 * LTFATERR_BADSIZE         | Length of the window \a gl was less or equal to 0.
 * LTFATERR_NOTPOSARG       | At least one of the following was less or equal to zero: \a a, \a M
 * LTFATERR_INITFAILED      | FFTW plan creation failed
 * LTFATERR_CANNOTHAPPEN    | \a ptype does not have a valid value from the ltfat_phaseconvention enum
 * LTFATERR_NOMEM           | Indicates that heap allocation failed
 */
LTFAT_API int
LTFAT_NAME(dgtreal_fb_init)(const LTFAT_REAL g[],
                            const ltfatInt gl, const ltfatInt a,
                            const ltfatInt M, const ltfat_phaseconvention ptype,
                            unsigned flags, LTFAT_NAME(dgtreal_fb_plan)** plan);

/** Execute plan for Discrete Gabor Transform for real signals using the filter bank algorithm
 *
 * \param[in]  plan   DGT plan
 * \param[in]     f   Input signal, size L x W
 * \param[in]     L   Signal length
 * \param[in]     W   Number of channels of the signal
 * \param[out]    c   DGT coefficients, size M2 x N x W
 *
 * #### Versions #
 * <tt>
 * ltfat_dgtreal_fb_execute_d(ltfat_dgtreal_fb_plan_d* plan, const double f[],
 *                            const ltfatInt L, const ltfatInt W, complex double c[]);
 *
 * ltfat_dgtreal_fb_execute_s(ltfat_dgtreal_fb_plan_s* plan, const float f[],
 *                            const ltfatInt L, const ltfatInt W, complex float c[]);
 * </tt>
 *
 * \returns
 * Status code              | Description
 * -------------------------|--------------------------------------------
 * LTFATERR_SUCCESS         | Indicates no error
 * LTFATERR_NULLPOINTER     | At least one of the following was NULL: \a f, \a c, \a plan
 * LTFATERR_BADSIZE         | Length of the signal \a L was less or equal to 0.
 * LTFATERR_BADTRALEN       | \a L must be bigger of equal to \a gl and must be divisible by \a a
 * LTFATERR_NOTPOSARG       | \a W was less or equal to 0.
 */
LTFAT_API int
LTFAT_NAME(dgtreal_fb_execute)(LTFAT_NAME(dgtreal_fb_plan)* plan,
                               const LTFAT_REAL f[], const ltfatInt L,
                               const ltfatInt W, LTFAT_COMPLEX c[]);

/** Destroy the plan
 *
 * \param[in]  plan   DGT plan
 *
 * #### Versions #
 * <tt>
 * ltfat_dgtreal_fb_done_d(ltfat_dgtreal_fb_plan_d** plan);
 *
 * ltfat_dgtreal_fb_done_s(ltfat_dgtreal_fb_plan_s** plan);
 * </tt>
 *
 * \returns
 * Status code              | Description
 * -------------------------|--------------------------------------------
 * LTFATERR_SUCCESS         | Indicates no error
 * LTFATERR_NULLPOINTER     | plan or *plan was NULL.
 */
LTFAT_API int
LTFAT_NAME(dgtreal_fb_done)(LTFAT_NAME(dgtreal_fb_plan)** plan);

/** @}*/
