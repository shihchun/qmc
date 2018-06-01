/*
 * Compile without GPU support:
 *   c++ -std=c++11 -I../src 4_cuda_demo_functors.cpp -o 4_cuda_demo_functors.out
 * Compile with GPU support:
 *   nvcc -std=c++11 -x cu -I../src 4_cuda_demo_functors.cpp -o 4_cuda_demo_functors.out
 */

#include <iostream>
#include <cmath> // sin, cos, exp
#include "qmc.hpp"

#ifdef __CUDACC__
#define HOSTDEVICE __host__ __device__
#else
#define HOSTDEVICE
#endif
struct functor1_t  { HOSTDEVICE double operator()(double* x) const { return sin(x[0])*cos(x[1])*exp(x[2]); } } functor1;
struct functor2_t  { HOSTDEVICE double operator()(double* x) const { return 1./( (x[0] + x[1])*(x[0] + x[1]) + .003 )*cos(x[1])*exp(x[2]); } } functor2;
struct functor3_t  { HOSTDEVICE double operator()(double* x) const { return 1./(3.75 - cos(M_PI*x[0]) - cos(M_PI*x[1]) - cos(M_PI*x[2])); } } functor3;
struct functor4_t  { HOSTDEVICE double operator()(double* x) const { return fabs(x[0]*x[0] + x[1]*x[1] + x[2]*x[2] - .125); } } functor4;
struct functor5_t  { HOSTDEVICE double operator()(double* x) const { return exp(-x[0]*x[0] - x[1]*x[1] - x[2]*x[2]); } } functor5;
struct functor6_t  { HOSTDEVICE double operator()(double* x) const { return 1./(1. - x[0]*x[1]*x[2] + 1e-10); } } functor6;
struct functor7_t  { HOSTDEVICE double operator()(double* x) const { return sqrt(fabs(x[0] - x[1] - x[2])); } } functor7;
struct functor8_t  { HOSTDEVICE double operator()(double* x) const { return exp(-x[0]*x[1]*x[2]); } } functor8;
struct functor9_t  { HOSTDEVICE double operator()(double* x) const { return x[0]*x[0]/(cos(x[0] + x[1] + x[2] + 1.) + 5.); } } functor9;
struct functor10_t { HOSTDEVICE double operator()(double* x) const { return ( (x[0] > .5) ? 1./sqrt(x[0]*x[1]*x[2] + 1e-5) : sqrt(x[0]*x[1]*x[2]) ); } } functor10;
struct functor11_t { HOSTDEVICE double operator()(double* x) const { return ( ((x[0]*x[0] + x[1]*x[1] + x[2]*x[2]) < 1.) ? 1. : 0. ); } } functor11;

template<typename F>
void integrate_and_print(integrators::Qmc<double,double>& real_integrator, F& functor)
{
    integrators::result<double> real_result = real_integrator.integrate(functor,3);
    std::cout << real_result.integral << " " << real_result.error << std::endl;
}

int main() {

    integrators::Qmc<double,double> real_integrator;

    // TODO - set parameters similar to CUBA demo?
    real_integrator.minn = 10000;
    real_integrator.maxeval = 1;

    integrate_and_print(real_integrator, functor1);
    integrate_and_print(real_integrator, functor2);
    integrate_and_print(real_integrator, functor3);
    integrate_and_print(real_integrator, functor4);
    integrate_and_print(real_integrator, functor5);
    integrate_and_print(real_integrator, functor6);
    integrate_and_print(real_integrator, functor7);
    integrate_and_print(real_integrator, functor8);
    integrate_and_print(real_integrator, functor9);
    integrate_and_print(real_integrator, functor10);
    integrate_and_print(real_integrator, functor11);

}
