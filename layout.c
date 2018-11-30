#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <time.h>
#include <math.h>

#define DIST(X1, Y1, X2, Y2) sqrt(pow(X1 - X2, 2) + pow(Y1 - Y2, 2))

struct point
{
	int x;
	int y;
};

void rand_coord(struct point *coord, int n, int env);
void print_coord(struct point *coord, int n);
void human_layout(struct point *coord, int n, int env);

static int flag_human = 0;

static struct option long_options[] =
{
    {"seed",      required_argument, 0, 's'},
    {"human",     no_argument,       0, 'h'},
    {0, 0, 0, 0}
};

// usage: ./layout NODES ENV RANGE
//  -s, --seed S    specify custom seed
//  -h, --human     print visual layout

int main(int argc, char **argv)
{
	int opt;
	int n, env, range;
	struct point *coord;
	int seed = time(0);

	// optional arguments
	while ((opt = getopt_long(argc, argv, "s:h", long_options, 0)) != -1)
	{
		switch(opt)
		{
			case 's':
				seed = atoi(optarg);
				break;
			case 'h':
				flag_human = 1;
				break;
			case '?':
				return 1;
		}
	}

	// mandatory arguments
	if (argc - optind < 3) return 1;

	n = atoi(argv[optind++]);
	env = atoi(argv[optind++]);
	range = atoi(argv[optind++]);

	coord = malloc((n+1) * sizeof (struct point));
	srand(seed);

	// sink at the center
	coord[0].x = 0;
	coord[0].y = 0;

	rand_coord(coord+1, n, env);

	printf("%d %d %d %d\n", n, env, range, seed);
	print_coord(coord, n+1);

	if (flag_human) human_layout(coord, n+1, env);
}

void rand_coord(struct point *coord, int n, int env)
{
	int i;
	float r, a;

	for (i = 0; i < n; i++)
	{
		r = env * sqrt(((float)rand()) / RAND_MAX);
		a = 2*M_PI * ((float)rand()) / RAND_MAX;

		coord[i].x = round(r * cos(a));
		coord[i].y = round(r * sin(a));
	}
}

void print_coord(struct point *coord, int n)
{
	int i;

	for (i = 0; i < n; i++)
	{
		printf("%d %d\n", coord[i].x, coord[i].y);
	}
}

void human_layout(struct point *coord, int n, int env)
{
	int i, j, k, l = 2*env+1;

	for (i = 0; i < l; i++) // row
	{
		for (j = 0; j < l; j++) // col
		{
			for (k = 0; k < n; k++)
			{
				if ((coord[k].x == (j-env)) && (coord[k].y == -(i-env)))
				{
					printf("%2d", k);
					break;
				}
			}

			// reached the end without finding one
			if (k == n) (round(DIST(i, j, env, env)) > env) ? printf("  ") : printf(" .");
		}
		
		printf("\n");
	}
}