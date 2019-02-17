#include "omp.h"
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char const *argv[]) {
    int num_sum = atoi(argv[1]);

    long double res = 0.0;
    int sign;

    #pragma omp parallel private(sign)
    {
        #pragma omp for reduction(+:res)
        for (int i = 1; i < num_sum; i++) {
            sign = (i + 1) % 2 ? -1 : 1;
            res += (long double)(sign * 4) / (long double)(2 * i - 1);
        }
    }

    printf("%.62Lf\n", res);

    return 0;
}