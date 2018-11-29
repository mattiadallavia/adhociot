#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define DIST(X1, Y1, X2, Y2) sqrt(pow(X1 - X2, 2) + pow(Y1 - Y2, 2))
#define IN_RANGE(A, B, GRAPH, N, RANGE) ((A != B) && (GRAPH[A*N+B] <= RANGE))

struct point
{
	int x;
	int y;
};

void rand_points(struct point *points, int n, int radius);
int points2graph(struct point *points, int *graph, int n);
int visit(int *graph, int n, int range, int vertex, int *visited);

void print_points(struct point *points, int n, int radius);
void print_graph(int *graph, int n, int range);
void plot_net(struct point *points, int *graph, int n, int radius, int range);

FILE *netout;

int main(int argc, char *argv[])
{
	int c, n, conn, radius, range;
	int f_printnet = 0;
	struct point *points;
	int *graph;
	// int *visited;

	srand(time(0));

	// mandatory arguments
	if (argc < 4)
	{
		fprintf(stderr, "usage: %s <number of nodes> <radius of grid> <range> [-p filename.png]\n", argv[0]);
		return 1;
	}

	n = atoi(argv[1]);
	radius = atoi(argv[2]);
	range = atoi(argv[3]);

	// optional arguments
	while ((c = getopt(argc-3, &argv[3], "p:")) != -1)
	{
		switch(c)
		{
			case 'p':
				f_printnet = 1;
				netout = fopen(optarg, "w");
				break;
			case '?':
				return 1;
		}
	}

	points = malloc(n * sizeof (struct point));
	graph = malloc(n * n * sizeof (int));
	// visited = malloc(n * sizeof (int));

	rand_points(points+1, n-1, radius);
	points2graph(points, graph, n);

	// memset(visited, 0, n * sizeof (int));
	// visit(graph, n, range, 0, visited) != n); // not all nodes are reachable

	print_points(points, n, radius);

	printf("\ngraph of the network:\n");
	print_graph(graph, n, range);

	if (f_printnet) plot_net(points, graph, n, radius, range);
}

void rand_points(struct point *points, int n, int radius)
{
	int i;
	float r, a;

	for (i = 0; i < n; i++)
	{
		r = radius * sqrt(((float)rand()) / RAND_MAX);
		a = 2*M_PI * ((float)rand()) / RAND_MAX;

		points[i].x = round(r * cos(a));
		points[i].y = round(r * sin(a));
	}
}

int points2graph(struct point *points, int *graph, int n)
{
	int i, j;

	for (i = 0; i < n; i++) for (j = 0; j < n; j++)
	{
		graph[i*n+j] = graph[j*n+i] = round(DIST(points[i].x, points[i].y, points[j].x, points[j].y));
	}
}

int visit(int *graph, int n, int range, int vertex, int *visited)
{
	int i, v = 1;
	visited[vertex] = 1;

	for (i = 0; i < n; i++)
	{
		// if i is reachable from vertex and i has not been visited yet
		if (IN_RANGE(vertex, i, graph, n, range) && (!visited[i])) v += visit(graph, n, range, i, visited);
	}

	return v;
}

void print_points(struct point *points, int n, int radius)
{
	int i, j, k, l=2*radius+1;

	for (i = 0; i < l; i++) // row
	{
		for (j = 0; j < l; j++) // col
		{
			for (k = 0; k < n; k++)
			{
				if ((points[k].x == (j-radius)) && (points[k].y == -(i-radius)))
				{
					printf("%2d", k);
					break;
				}
			}

			// reached the end without finding one
			if (k == n) (round(DIST(i, j, radius, radius)) > radius) ? printf("  ") : printf(" -");
		}
		
		printf("\n");
	}
}

void plot_net(struct point *points, int *graph, int n, int radius, int range)
{
	int i, j;

	fprintf(netout, "radius = %d\n\n", radius);

	fprintf(netout, "$vertices << EOD\n");
	for (i = 0; i < n; i++)
	{
		fprintf(netout, "%d %d %d\n", points[i].x, points[i].y, i);
	}
	fprintf(netout, "EOD\n\n");

	fprintf(netout, "$arcs << EOD\n");
	for (i = 0; i < n; i++) for (j=0; j<i; j++)
	{
		if (IN_RANGE(i, j, graph, n, range)) fprintf(netout, "%d %d\n%d %d\n\n", points[i].x, points[i].y, points[j].x, points[j].y);
	}
	fprintf(netout, "EOD\n\n");

	fprintf(netout, "$weights << EOD\n");
	for (i = 0; i < n; i++) for (j=0; j<i; j++)
	{
		if (IN_RANGE(i, j, graph, n, range)) fprintf(netout, "%f %f %d\n", (points[i].x + points[j].x) / 2.0, (points[i].y + points[j].y) / 2.0, graph[i*n+j]);
	}
	fprintf(netout, "EOD\n");
}

void print_graph(int *graph, int n, int range)
{
	int i, j;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j<n; j++)
		{
			if (IN_RANGE(i, j, graph, n, range)) printf("%2d", graph[i*n+j]);
			else printf(" -");
		}

		printf("\n");
	}
}