#pragma once
#ifndef QMC_H
#define QMC_H

#include <mutex>
#include <vector>
#include <map>
#include <set>
#include <random> // mt19937_64, uniform_real_distribution
#include <type_traits> // make_signed
#include <iterator>

namespace integrators
{
    
    template <typename T, typename U = unsigned long long int>
    struct result
    {
        T integral;
        T error;
        U n;
        U m;
    };

// TODO - unsigned int MAXDIM = 20,
    template <typename T, typename D, typename U = unsigned long long int, typename G = std::mt19937_64>
    class Qmc
    {

    private:

        std::uniform_real_distribution<D> uniformDistribution = std::uniform_real_distribution<D>(0,1);
        std::mutex work_queue_mutex;
        U work_queue;

        U getNextN(U preferred_n) const;
        
        void initg();
        void initz(std::vector<U>& z, const U n, const U dim) const;
        void initd(std::vector<D>& d, const U m, const U dim);
        void initr(std::vector<T>& r, const U m, const U r_size) const;
        
        result<T,U> reduce(const std::vector<T>& r, const U n, const U m) const;
        template <typename F1, typename F2> void compute(const int i, const std::vector<U>& z, const std::vector<D>& d, T* r_element, const U r_size, const U total_work_packages, const U points_per_package, const U n, const U m, F1& func, const U dim, F2& integralTransform);
        template <typename F1, typename F2> void compute_worker(const U thread_id, const std::vector<U>& z, const std::vector<D>& d, std::vector<T>& r, const U total_work_packages, const U points_per_package, const U n, const U m,  F1& func, const U dim, F2& integralTransform, const int device);
#ifdef __CUDACC__
        template <typename F1, typename F2> void compute_gpu(const U i, const std::vector<U>& z, const std::vector<D>& d, T* r_element, const U r_size, const U work_this_iteration, const U total_work_packages, const U points_per_package, const U n, const U m, F1& func, const U dim, F2& integralTransform, const int device, const U cudablocks, const U cudathreadsperblock);
#endif
        template <typename F1, typename F2> result<T,U> sample(F1& func, const U dim, F2& integralTransform, const U n, const U m);
        void update(result<T,U>& res, U& n, U& m);

    public:

        G randomGenerator;

        U minn;
        U minm;
        D epsrel;
        D epsabs;
        U maxeval;
        U max_work_packages;
        U cputhreads;
        U cudablocks;
        U cudathreadsperblock;
        std::set<int> devices;
        std::map<U,std::vector<U>> generatingVectors;
        U verbosity;

        template <typename F1, typename F2> result<T,U> integrate(F1& func, const U dim, F2& integralTransform);
        template <typename F1> result<T,U> integrate(F1& func, const U dim);

        Qmc();
        virtual ~Qmc() {}

    };
    
};

// Implementation
#include "qmc_mul_mod.hpp"
#include "qmc_real.hpp"
#include "qmc_complex.hpp"
#include "qmc_transform.hpp"
#include "qmc_generating_vectors.hpp"
#include "qmc_core.hpp"

#ifdef __CUDACC__
#include "qmc_core_gpu.hpp"
#endif

#endif
