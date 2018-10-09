#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define DIST(X1, Y1, X2, Y2) sqrt(pow(X1 - X2, 2) + pow(Y1 - Y2, 2))

struct point
{
	int x;
	int y;
};

void rand_points(struct point *points, int n, int radius);
int points2graph(struct point *points, int *graph, int n);
int visit(int *graph, int n, int range, int vertex, int *visited);

int main(int argc, char *argv[])
{
	int n, i, range, iter, conn, conn_sum, compl;
	int nodes_min, nodes_max, nodes_step;
	float g, gain_min, gain_max, gain_step;
	struct point *points;
	int *graph;
	int *visited;

	srand(time(0));

	// mandatory arguments
	if (argc < 9)
	{
		fprintf(stderr, "usage: %s <min. nodes> <max. nodes> <n. step> "
			                      "<min. gain> <max. gain> <g. step> "
		                          "<range> <iter.>\n", argv[0]);
		return 1;
	}

	nodes_min = atoi(argv[1]);
	nodes_max = atoi(argv[2]);
	nodes_step = atoi(argv[3]);
	gain_min = atof(argv[4]);
	gain_max = atof(argv[5]);
	gain_step = atof(argv[6]);
	range = atoi(argv[7]);
	iter = atoi(argv[8]);

	points = malloc(nodes_max * sizeof (struct point));
	graph = malloc(nodes_max * nodes_max * sizeof (int));
	visited = malloc(nodes_max * sizeof (int));

	// the sink is always in the center
	points[0].x = 0;
	points[0].y = 0;

	for (g = gain_min; g <= gain_max; g += gain_step)
	{
		for (n = nodes_min; n <= nodes_max; n += nodes_step)
		{
			conn_sum = 0;
			compl = 0;

			// generate random samples
			for (i = 0; i < iter; i++)
			{
				memset(visited, 0, n * sizeof (int));

				rand_points(points+1, n-1, g*range);
				points2graph(points, graph, n);
				conn = visit(graph, n, range, 0, visited);

				conn_sum += conn;
				compl += (conn == n);
			}

			printf("%f %d %f %f\n", g, n, ((float)conn_sum) / (iter*n), ((float)compl) / iter);
		}
		printf("\n");
	}
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
		if ((graph[vertex*n+i] <= range) && (!visited[i])) v += visit(graph, n, range, i, visited);
	}

	return v;
}