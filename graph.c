#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define DIST(X1, Y1, X2, Y2) sqrt(pow(X1 - X2, 2) + pow(Y1 - Y2, 2))
#define IN_RANGE(A, B, GRAPH, N) ((A != B) && (GRAPH[A*N+B] <= 1))

struct point
{
	int x;
	int y;
};

void read_coord(struct point *coord, int n);
void layout2graph(struct point *coord, float *graph, int n, int range);
int visit(float *graph, int n, int vertex, int *visited);
void print_graph(float *graph, int n);
void plot(struct point *coord, float *graph, int n, int env);

FILE *plotout;

static int flag_plot = 0;

static struct option long_options[] =
{
    {"plot", required_argument, 0, 'p'},
    {0, 0, 0, 0}
};

// usage: ./graph
//  -p, --plot FILENAME    write plot data file

int main(int argc, char **argv)
{
	int opt;
	int n, env, range;
	struct point *coord;
	float *graph;
	int conn;
	int *visited;

	// optional arguments
	while ((opt = getopt_long(argc, argv, "p:", long_options, 0)) != -1)
	{
		switch(opt)
		{
			case 'p':
				flag_plot = 1;
				plotout = fopen(optarg, "w");
				break;
			case '?':
				return 1;
		}
	}

	scanf("%d %d %d %*d\n", &n, &env, &range);

	coord = malloc((n+1) * sizeof (struct point));
	graph = malloc((n+1) * (n+1) * sizeof (float));
	visited = calloc(n+1, sizeof (int));

	read_coord(coord, n+1);
	
	layout2graph(coord, graph, n+1, range);
	conn = visit(graph, n+1, 0, visited) - 1;

	printf("%d %d %d %d\n", n, env, range, conn);
	print_graph(graph, n+1);

	if (flag_plot) plot(coord, graph, n+1, env);
}

void read_coord(struct point *coord, int n)
{
	int i;

	for (i = 0; i < n; i++)
	{
		scanf("%d %d\n", &coord[i].x, &coord[i].y);
	}
}

void layout2graph(struct point *coord, float *graph, int n, int range)
{
	int i, j;

	for (i = 0; i < n; i++) for (j = 0; j < n; j++)
	{
		graph[i*n+j] = graph[j*n+i] = DIST(coord[i].x, coord[i].y, coord[j].x, coord[j].y) / range;
	}
}

int visit(float *graph, int n, int vertex, int *visited)
{
	int i, v = 1;
	visited[vertex] = 1;

	for (i = 0; i < n; i++)
	{
		// if i is reachable from vertex and i has not been visited yet
		if (IN_RANGE(vertex, i, graph, n) && (!visited[i])) v += visit(graph, n, i, visited);
	}

	return v;
}

void print_graph(float *graph, int n)
{
	int i, j;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j<n; j++)
		{
			printf("%.2f ", graph[i*n+j]);
		}

		printf("\n");
	}
}

void plot(struct point *coord, float *graph, int n, int env)
{
	int i, j;

	fprintf(plotout, "env = %d\n\n", env);

	fprintf(plotout, "$vertices << EOD\n");
	for (i = 0; i < n; i++)
	{
		fprintf(plotout, "%d %d %d\n", coord[i].x, coord[i].y, i);
	}
	fprintf(plotout, "EOD\n\n");

	fprintf(plotout, "$arcs << EOD\n");
	for (i = 0; i < n; i++) for (j=0; j<i; j++)
	{
		if (IN_RANGE(i, j, graph, n)) fprintf(plotout, "%d %d\n%d %d\n\n", coord[i].x, coord[i].y, coord[j].x, coord[j].y);
	}
	fprintf(plotout, "EOD\n\n");

	fprintf(plotout, "$weights << EOD\n");
	for (i = 0; i < n; i++) for (j=0; j<i; j++)
	{
		if (IN_RANGE(i, j, graph, n)) fprintf(plotout, "%f %f %.1f\n", (coord[i].x + coord[j].x) / 2.0, (coord[i].y + coord[j].y) / 2.0, graph[i*n+j]);
	}
	fprintf(plotout, "EOD\n");
}