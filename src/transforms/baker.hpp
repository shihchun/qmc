#ifndef QMC_TRANSFORMS_BAKER_H
#define QMC_TRANSFORMS_BAKER_H

namespace integrators
{
    namespace transforms
    {
        template<typename I, typename D>
        struct BakerImpl
        {
            I f; // original function
            const U number_of_integration_variables;

            BakerImpl(I f) : f(f), number_of_integration_variables(f.number_of_integration_variables) {}

#ifdef __CUDACC__
            __host__ __device__
#endif
            auto operator()(D* x) -> decltype(f(x)) const
            {
                D wgt = 1;
                for (U s = 0; s < number_of_integration_variables; s++)
                {
                    x[s] = D(1) - fabs(D(2)*x[s]-D(1)) ;
                    // loss of precision can cause x < 0 or x > 1 must keep in x \elem [0,1]
                    if (x[s] > D(1)) x[s] = D(1);
                    if (x[s] < D(0)) x[s] = D(0);
                }
                return wgt * f(x);
            }
        };
        struct Baker
        {
            template<typename I, typename D, U M> using type = BakerImpl<I, D>;
        };
    };
};

#endif
