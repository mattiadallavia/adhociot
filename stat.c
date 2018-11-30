#include <stdio.h>
#include <stdlib.h>

// usage: ./stat N_MIN   N_MAX   N_STEP
//	             DIM_MIN DIM_MAX DIM_STEP
//               RANGE   ITER

int main(int argc, char **argv)
{
	int n, n_min, n_max, n_step;
	float dim, dim_min, dim_max, dim_step;
	int i, range, iter;
	int conn, conn_sum, compl;
	char command[100];
	FILE *pipe;

	// mandatory arguments
	if (argc < 9) return 1;

	n_min = atoi(argv[1]);
	n_max = atoi(argv[2]);
	n_step = atoi(argv[3]);
	dim_min = atof(argv[4]);
	dim_max = atof(argv[5]);
	dim_step = atof(argv[6]);
	range = atoi(argv[7]);
	iter = atoi(argv[8]);

	for (dim = dim_min; dim <= dim_max; dim += dim_step)
	{
		for (n = n_min; n <= n_max; n += n_step)
		{
			conn_sum = 0;
			compl = 0;

			// random samples
			for (i = 0; i < iter; i++)
			{
				sprintf(command, "./layout %d %d %d | ./graph",
				                 n, (int) dim*range, range);
				pipe = popen(command, "r");
				fscanf(pipe, "%*d %*d %*d %d\n", &conn);
				pclose(pipe);

				conn_sum += conn;
				compl += (conn == n);
			}

			printf("%f %d %f %f\n", dim, n, ((float)conn_sum) / (iter*n),
			                                ((float)compl) / iter);
		}
		printf("\n");
	}
}