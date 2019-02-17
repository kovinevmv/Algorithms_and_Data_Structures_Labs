#include "omp.h"
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char const *argv[]) {
    int num_sum = atoi(argv[1]);

    long double res = 0.0;
    long double loc_res = 0.0;
    int sign;

    #pragma omp parallel firstprivate(loc_res) private(sign)
    {
        #pragma omp for
        for (int i = 1; i < num_sum; i++) {
            sign = (i + 1) % 2 ? -1 : 1;
            loc_res += (long double)(sign * 4) / (long double)(2 * i - 1);
        }

        #pragma omp atomic
        res += loc_res;
    }

    printf("%.62Lf\n", res);

    return 0;
}