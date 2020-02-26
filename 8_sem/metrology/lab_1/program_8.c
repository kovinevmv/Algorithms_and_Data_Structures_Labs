#include <stdio.h>

#define rmax 9
#define cmax 3


typedef double ary[rmax];
typedef double arys[cmax];
typedef double ary2[rmax][cmax];
typedef double ary2s[cmax][cmax];
typedef double* pointer;
typedef double** dpointer;


void get_data(ary2 x, ary y, int nrow, int ncol) {
	for (int i = 0; i < nrow; i++) {
		x[i][0] = 1;
		for (int j = 1; j < ncol; j++) {
			x[i][j] = (i + 1) * x[i][j - 1];
		}
		y[i] = 2 * (i + 1);
	}
}

void square(ary2 x, double* y, ary2s a, double* g, int nrow, int ncol) {
	for (int k = 0; k < ncol; k++) {
		for (int l = 0; l <= k; l++) {
			a[k][l] = 0;

			for (int i = 0; i < nrow; i++) {
				a[k][l] = a[k][l] + x[i][l] * x[i][k];
				if (k != l)
					a[l][k] = a[k][l];
			}
		}

		g[k] = 0;
		for (int i = 0; i < nrow; i++) {
			g[k] = g[k] + y[i] * x[i][k];
		}
	}
}


void write_data(ary2 x, double* y, ary2s a, double* g, int nrow, int ncol) {
	printf("          X             Y\n");
	for (int i = 0; i < nrow; i++) {
		for (int j = 0; j < ncol; j++) {
			printf("%f ", x[i][j]);
		}
		printf(":%f\n", y[i]);
	}

	printf("          A             G\n");
	for (int i = 0; i < ncol; i++) {
		for (int j = 0; j < ncol; j++) {
			printf("%f ", a[i][j]);
		}
		printf(":%f\n", g[i]);
	}
}



int main() {
	int nrow = 5;
	int ncol = 3;

	ary2 x;
	ary y;

	arys g;
	ary2s a;

	get_data(x, y, nrow, ncol);
	square(x, y, a, g, nrow, ncol);
	write_data(x, y, a, g, nrow, ncol);

	return 0;
}
