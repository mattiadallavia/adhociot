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

void usage(char* name);
void rand_layout(struct point *layout, int n, int env);
void print_layout(struct point *layout, int n);
void human_layout(struct point *layout, int n, int env);

static int flag_human = 0;

static struct option long_options[] =
{
    {"seed",      required_argument, 0, 's'},
    {"human",     no_argument,       0, 'h'},
    {0, 0, 0, 0}
};

int main(int argc, char **argv)
{
	int opt;
	int n, env, range;
	struct point *layout;
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
	if (argc - optind < 3)
	{
		usage(argv[0]);
		return 1;
	}

	n = atoi(argv[optind++]);
	env = atoi(argv[optind++]);
	range = atoi(argv[optind++]);

	layout = malloc(n * sizeof (struct point));
	srand(seed);

	rand_layout(layout+1, n-1, env);

	printf("%d %d %d %d\n", n, env, range, seed);
	print_layout(layout, n);

	if (flag_human) human_layout(layout, n, env);
}

void usage(char* name)
{
	fprintf(stderr, "usage: %s NODES ENV RANGE\n", name);
	fprintf(stderr, " -s, --seed S    specify custom seed\n");
	fprintf(stderr, " -h, --human     print visual layout\n");
}

void rand_layout(struct point *layout, int n, int env)
{
	int i;
	float r, a;

	for (i = 0; i < n; i++)
	{
		r = env * sqrt(((float)rand()) / RAND_MAX);
		a = 2*M_PI * ((float)rand()) / RAND_MAX;

		layout[i].x = round(r * cos(a));
		layout[i].y = round(r * sin(a));
	}
}

void print_layout(struct point *layout, int n)
{
	int i;

	for (i = 0; i < n; i++)
	{
		printf("%d %d\n", layout[i].x, layout[i].y);
	}
}

void human_layout(struct point *layout, int n, int env)
{
	int i, j, k, l = 2*env+1;

	for (i = 0; i < l; i++) // row
	{
		for (j = 0; j < l; j++) // col
		{
			for (k = 0; k < n; k++)
			{
				if ((layout[k].x == (j-env)) && (layout[k].y == -(i-env)))
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