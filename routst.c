#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <string.h>

static int flag_coll = 0;
static int flag_wfac = 0;
static int flag_bfac = 0;
static int flag_rfac = 0;

static struct option long_options[] =
{
    {"seed",       required_argument, 0,        's'},
    {"wfactor",    required_argument, 0,        'w'},
    {"bfactor",    required_argument, 0,        'b'},
    {"rfactor",    required_argument, 0,        'r'},
    {"collisions", no_argument,       &flag_coll, 1},
    {0, 0, 0, 0}
};

// usage: ./routst DIM_MIN DIM_MAX DIM_STEP
//	               N_MIN   N_MAX   N_STEP
//                 RANGE   ITER
//  --seed S                          set custom seed
//  --collisions                      enable collision detection and avoidance
//  --wfactor 'W_MIN W_MAX W_STEP'    set wait factor range
//  --bfactor 'W_MIN W_MAX W_STEP'    set branching factor range

int main(int argc, char **argv)
{
	int opt;
	int seed = time(0);
	int n, n_min, n_max, n_step;
	float dim, dim_min, dim_max, dim_step;
	int w, wr, w_min = 1, w_max = 1, w_step = 1;
	int b, br, b_min = 1, b_max = 1, b_step = 1;
	int r, rr, r_min = 1, r_max = 1, r_step = 1;
	int i, env, range, iter;
	int conn, t, tx, retx, rx, coll;
	char comm[200];
	FILE *pipe;

	// optional arguments
	while ((opt = getopt_long(argc, argv, "s:w:b:", long_options, 0)) != -1)
	{
		switch (opt)
		{
			case 's':
				seed = atoi(optarg);
				break;
			case 'w':
				flag_wfac = 1;
				sscanf(optarg, "%d %d %d", &w_min, &w_max, &w_step);
				break;
			case 'b':
				flag_bfac = 1;
				sscanf(optarg, "%d %d %d", &b_min, &b_max, &b_step);
				break;
			case 'r':
				flag_rfac = 1;
				sscanf(optarg, "%d %d %d", &r_min, &r_max, &r_step);
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
			for (w = w_min; w <= w_max; w += w_step)
			{
				for (b = b_min; b <= b_max; b += b_step)
				{
					for (r = r_min; r <= r_max; r += r_step)
					{
						// samples
						for (i = 0; i < iter; i++)
						{
							sprintf(comm, "./layout %d %d %d --s %d | ./graph | ./rout",
							        env, n, range, seed++);
							if (flag_coll) strcat(comm, " --c");
							if (flag_wfac) sprintf(comm+strlen(comm), " --w %d", w);
							if (flag_bfac) sprintf(comm+strlen(comm), " --b %d", b);
							if (flag_rfac) sprintf(comm+strlen(comm), " --r %d", r);

							pipe = popen(comm, "r");
							fscanf(pipe, "%*d\t%*d\t%*d\t%*d\t%d\t%d\t%d\t%d\n", &conn, &wr, &br, &rr);
							fscanf(pipe, "%d\t%d\t%d\t%d\t%d\n", &t, &tx, &retx, &rx, &coll);
							pclose(pipe);

							printf("%f\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", dim, n, conn, wr, br, rr, t, tx, retx, rx, coll);
						}
						if (r_max-r_min) printf("\n");
					}
					if (b_max-b_min) printf("\n");
				}
				if (w_max-w_min) printf("\n");
			}
			printf("\n");
		}
		printf("\n");
	}
}