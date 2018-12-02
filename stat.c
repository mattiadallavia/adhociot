#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <math.h>

static struct option long_options[] =
{
    {"seed", required_argument, 0, 's'},
    {0, 0, 0, 0}
};

// usage: ./stat N_MIN   N_MAX   N_STEP
//	             DIM_MIN DIM_MAX DIM_STEP
//               RANGE   ITER
//  -s, --seed S    specify custom seed

int main(int argc, char **argv)
{
	int opt;
	int seed = time(0);
	int n, n_min, n_max, n_step;
	float dim, dim_min, dim_max, dim_step;
	int i, range, iter;
	int conn, t, tx, rx, collisions;
	int conn_sum;
	int t_sum1, t_sum2;
	float t_mean, t_var;
	char command[100];
	FILE *pipe;

	// optional arguments
	while ((opt = getopt_long(argc, argv, "s:", long_options, 0)) != -1)
	{
		switch (opt)
		{
			case 's':
				seed = atoi(optarg);
				break;
			case '?':
				return 1;
		}
	}

	// mandatory arguments
	if (argc - optind < 8) return 1;

	n_min = atoi(argv[optind++]);
	n_max = atoi(argv[optind++]);
	n_step = atoi(argv[optind++]);
	dim_min = atof(argv[optind++]);
	dim_max = atof(argv[optind++]);
	dim_step = atof(argv[optind++]);
	range = atoi(argv[optind++]);
	iter = atoi(argv[optind++]);

	for (dim = dim_min; dim <= dim_max; dim += dim_step)
	{
		for (n = n_min; n <= n_max; n += n_step)
		{
			conn_sum = 0;
			t_sum1 = t_sum2 = 0;

			// random samples
			for (i = 0; i < iter; i++)
			{
				sprintf(command, "./layout %d %d %d --seed %d | ./graph | ./sim",
				        n, (int) dim*range, range, seed++);
				pipe = popen(command, "r");
				fscanf(pipe, "%*d %*d %*d %d\n", &conn);
				fscanf(pipe, "%d %d %d %d\n", &t, &tx, &rx, &collisions);
				pclose(pipe);

				conn_sum += conn;
				t_sum1 += t;
				t_sum2 += t*t;
			}

			// https://en.wikipedia.org/wiki/Standard_deviation#Rapid_calculation_methods
			t_mean = ((float)t_sum1) / iter;
			t_var = ((float)t_sum2) / iter - t_mean*t_mean;
			
			printf("%f %d %f %f %f\n", dim, n, ((float)conn_sum) / (iter*n),
			                           t_mean, t_var);
		}
		printf("\n");
	}
}