#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define DIST(X1, Y1, X2, Y2) sqrt(pow(X1 - X2, 2) + pow(Y1 - Y2, 2))
#define IN_RANGE(A, B, GRAPH, N) ((A != B) && (GRAPH[A*N+B] <= 1))

struct point
{
	int x;
	int y;
};

void rand_layout(struct point *layout, int n, int env);
int layout2graph(struct point *layout, float *graph, int n, int range);
int visit(float *graph, int n, int range, int vertex, int *visited);

void print_layout(struct point *layout, int n, int env);
void print_graph(float *graph, int n);
void plot_net(struct point *layout, float *graph, int n, int env);

FILE *plotout;

int main(int argc, char *argv[])
{
	int n, env, range;
	struct point *layout;
	float *graph;
	int c;
	int f_plotnet = 0;

	srand(time(0));

	// mandatory arguments
	if (argc < 4)
	{
		fprintf(stderr, "usage: %s <number of nodes> <radius of env.> <range>\n"
		                "                 [-p filename]\n", argv[0]);
		return 1;
	}

	n = atoi(argv[1]);
	env = atoi(argv[2]);
	range = atoi(argv[3]);

	// optional arguments
	while ((c = getopt(argc-3, &argv[3], "p:")) != -1)
	{
		switch(c)
		{
			case 'p':
				f_plotnet = 1;
				plotout = fopen(optarg, "w");
				break;
			case '?':
				return 1;
		}
	}

	layout = malloc(n * sizeof (struct point));
	graph = malloc(n * n * sizeof (float));
	// visited = malloc(n * sizeof (int));

	rand_layout(layout+1, n-1, env);
	layout2graph(layout, graph, n, range);

	// memset(visited, 0, n * sizeof (int));
	// visit(graph, n, range, 0, visited) != n); // not all n are reachable

	print_layout(layout, n, env);
	print_graph(graph, n);

	if (f_plotnet) plot_net(layout, graph, n, env);
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

int layout2graph(struct point *layout, float *graph, int n, int range)
{
	int i, j;

	for (i = 0; i < n; i++) for (j = 0; j < n; j++)
	{
		graph[i*n+j] = graph[j*n+i] = DIST(layout[i].x, layout[i].y, layout[j].x, layout[j].y) / range;
	}
}

int visit(float *graph, int n, int range, int vertex, int *visited)
{
	int i, v = 1;
	visited[vertex] = 1;

	for (i = 0; i < n; i++)
	{
		// if i is reachable from vertex and i has not been visited yet
		if (IN_RANGE(vertex, i, graph, n) && (!visited[i])) v += visit(graph, n, range, i, visited);
	}

	return v;
}

void print_layout(struct point *layout, int n, int env)
{
	int i, j, k, l=2*env+1;

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
			if (k == n) (round(DIST(i, j, env, env)) > env) ? printf("  ") : printf(" -");
		}
		
		printf("\n");
	}
}

void plot_net(struct point *layout, float *graph, int n, int env)
{
	int i, j;

	fprintf(plotout, "env = %d\n\n", env);

	fprintf(plotout, "$vertices << EOD\n");
	for (i = 0; i < n; i++)
	{
		fprintf(plotout, "%d %d %d\n", layout[i].x, layout[i].y, i);
	}
	fprintf(plotout, "EOD\n\n");

	fprintf(plotout, "$arcs << EOD\n");
	for (i = 0; i < n; i++) for (j=0; j<i; j++)
	{
		if (IN_RANGE(i, j, graph, n)) fprintf(plotout, "%d %d\n%d %d\n\n", layout[i].x, layout[i].y, layout[j].x, layout[j].y);
	}
	fprintf(plotout, "EOD\n\n");

	fprintf(plotout, "$weights << EOD\n");
	for (i = 0; i < n; i++) for (j=0; j<i; j++)
	{
		if (IN_RANGE(i, j, graph, n)) fprintf(plotout, "%f %f %f\n", (layout[i].x + layout[j].x) / 2.0, (layout[i].y + layout[j].y) / 2.0, graph[i*n+j]);
	}
	fprintf(plotout, "EOD\n");
}

void print_graph(float *graph, int n)
{
	int i, j;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j<n; j++)
		{
			if (IN_RANGE(i, j, graph, n)) printf(" %.1f", graph[i*n+j]);
			else printf(" ---");
		}

		printf("\n");
	}
}