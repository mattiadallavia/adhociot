#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>

static struct option long_options[] =
{
    {"seed", required_argument, 0, 's'},
    {0, 0, 0, 0}
};

// usage: ./conn DIM_MIN DIM_MAX DIM_STEP
//	             N_MIN   N_MAX   N_STEP
//               RANGE   ITER
//  --seed S    specify custom seed

int main(int argc, char **argv)
{
	int opt;
	int seed = time(0);
	int n, n_min, n_max, n_step;
	float dim, dim_min, dim_max, dim_step;
	int i, env, range, iter;
	int conn;
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

	dim_min = atof(argv[optind++]);
	dim_max = atof(argv[optind++]);
	dim_step = atof(argv[optind++]);
	n_min = atoi(argv[optind++]);
	n_max = atoi(argv[optind++]);
	n_step = atoi(argv[optind++]);
	range = atoi(argv[optind++]);
	iter = atoi(argv[optind++]);

	for (dim = dim_min; dim <= dim_max; dim += dim_step)
	{
		env = (int) dim*range;

		for (n = n_min; n <= n_max; n += n_step)
		{
			// samples
			for (i = 0; i < iter; i++)
			{
				sprintf(command, "./layout %d %d %d --seed %d | ./graph",
				        env, n, range, seed++);
				pipe = popen(command, "r");
				fscanf(pipe, "%*d\t%*d\t%*d\t%d\n", &conn);
				pclose(pipe);

				printf("%f\t%d\t%d\n", dim, n, conn);
			}
			printf("\n");
		}
		printf("\n");
	}
}