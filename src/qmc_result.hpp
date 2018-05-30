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
};
