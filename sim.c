#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define NODE_WAIT   0 // waiting to be activated by the reception of one packet
#define NODE_ACTIVE 1 // ready to transmit message or to relay other client's messages

#define QUEUE_MAX 100
#define MAX_ATTEMPTS 1000000

struct point
{
	int x;
	int y;
};

struct message
{
	int number;
	int wait;
	int channel_ack;
};

struct node
{
	int state;
	int depth;
	int channel;
	int channel_up;
	struct message queue[QUEUE_MAX];
	int queue_len;
};

int alg2(struct node *nodes, char *net, size_t s);
int alg1(struct node *nodes, char *net, size_t s);
void randtopo(struct point *points, size_t n, int radius);
void printtopo(struct point *points, size_t n, int radius);
void plottopo(struct point *points, char *graph, size_t n, int radius, char *plot_filename);
int topo2graph(struct point *points, char *graph, size_t n, int range);
void printgraph(char *graph, size_t n);
int connectedgraph(char *graph, size_t n);
void visitgraph(char* n, size_t s, int vertex, char* visited);
void printnodes(struct node *nodes, size_t s);
int find(struct node *n, int number);
void enqueue(struct node *n, struct message m);
struct message dequeue(struct node *n, unsigned int pos);

int main(int argc, char *argv[])
{
	int c, tx, nodes_number, alg, conn, radius, range, attempts = 0;
	char* plot_filename = 0;
	struct point *points;
	struct node *nodes;
	char* graph;

	srand(time(0));

	// mandatory arguments
	if (argc < 5)
	{
		fprintf(stderr, "usage: %s <alg.> <number of nodes> <radius of grid> <range> [-p filename.png]\n", argv[0]);
		return 1;
	}

	alg = atoi(argv[1]);
	nodes_number = atoi(argv[2]);
	radius = atoi(argv[3]);
	range = atoi(argv[4]);

	// optional arguments
	while ((c = getopt(argc-4, &argv[4], "p:")) != -1)
	{
		switch(c)
		{
			case 'p':
				plot_filename = optarg;
				break;
			case '?':
				return 1;
		}
	}

	nodes = calloc(nodes_number, sizeof (struct node));
	graph = malloc(nodes_number * nodes_number);
	points = malloc(nodes_number * sizeof (struct point));

	// nodes initialization
	for (int i=1; i<nodes_number; i++)
	{
		nodes[i].queue[0].number = i;
		nodes[i].queue[0].channel_ack = -1;
		nodes[i].queue_len = 1;
		nodes[i].depth = -1;
	}

	// attempt to generate random topology and graph
	do {
		attempts++;
		if (attempts > MAX_ATTEMPTS)
		{
			fprintf(stderr, "couldn't generate connected graph\n");
			return 1;
		}

		randtopo(points, nodes_number, radius);
		topo2graph(points, graph, nodes_number, range);
	}
	while (!connectedgraph(graph, nodes_number));

	printf("topology gen. after %d attempts:\n", attempts);
	printtopo(points, nodes_number, radius);
	if (plot_filename) plottopo(points, graph, nodes_number, radius, plot_filename);

	printf("\ngraph of the network:\n");
	printgraph(graph, nodes_number);

	switch (alg)
	{
		case 0:
			// no alg.
			tx = 0;
			break;

		case 1:
			tx = alg1(nodes, graph, nodes_number);
			break;

		case 2:
			tx = alg2(nodes, graph, nodes_number);
			break;

		default:
			fprintf(stderr, "\nalg. number %d does not exist\n", alg);
			return 1;
	}

	printf("\nnumber of transmissions: %d\n", tx);
}

int alg2(struct node *nodes, char *net, size_t s)
{
	int i, j, tx = 0, t = 0, pick, weight, again, pos;
	struct node *n_tx, *n_rx;
	struct message *m;

	// the gateway initiates the process sending a void message
	pick = 0;
	n_tx = &nodes[pick];
	m = &n_tx->queue[0];
	
	n_tx->state = NODE_ACTIVE;
	n_tx->queue_len++;
	m->number = 0;
	m->channel_ack = -1;

	// execute one task from the picked client
	while (pick >= 0)
	{
		printf("\nt=%d\n", t);
		printf("node %d transmits message %d (ch_ack=%d) on channel (%d,%d)\n",
			   pick, m->number, m->channel_ack, (n_tx->depth % 3), n_tx->channel);

		// the node trasmits his message
		for (i=0; i<s; i++)
		{
			weight = net[pick*s+i];
			n_rx = &nodes[i];

			if (weight) // connected nodes receive the message
			{
				// update depths
				if ((n_rx->depth < 0) || ((n_tx->depth + 1) < n_rx->depth)) // better path
				{
					n_rx->depth = n_tx->depth + 1;
					printf("node %d updated depth to %d\n", i, n_rx->depth);
				}

				// activate waiting nodes
				if (n_rx->state == NODE_WAIT)
				{
					n_rx->state = NODE_ACTIVE;
					n_rx->queue[0].wait = t + n_tx->depth % 3 + 1; // wait a time proportional to the distance
					printf("node %d activated, message scheduled for t=%d\n", i, n_rx->queue[0].wait);
				}

				if ((n_tx->depth < n_rx->depth) && ((pos = find(n_rx, m->number)) < 0) && (m->channel_ack == n_rx->channel))
				{
					n_rx->channel = m->channel_ack + 1;
					printf("node %d selects channel %d because it has receaved message %d with ch_ack=%d\n",
						   i, n_rx->channel, m->number, m->channel_ack);
				}

				if ((n_tx->depth > n_rx->depth) && (n_tx->channel >= n_rx->channel_up))
				{
					n_rx->channel_up++;
					printf("node %d update upper channels count to %d\n", i, n_rx->channel_up);
				}

				// remove message from queue if we receive the re-tx (ack) from nearer the destination
				if ((n_tx->depth < n_rx->depth) && ((pos = find(n_rx, m->number)) >= 0))
				{
					dequeue(n_rx, pos);
					printf("message %d dequeued from node %d\n", m->number, i);
				}

				// relay message if we are nearer to the gateway
				if ((n_tx->depth > n_rx->depth) && (find(n_rx, m->number) < 0))
				{
					enqueue(n_rx, *m);
					n_rx->queue[find(n_rx, m->number)].channel_ack = n_tx->channel;
					printf("enqueued by node %d\n", i);
				}
			}
		}

		if (pick == 0) // if sink node
		{
			// dequeue sent message
			dequeue(n_tx, find(n_tx, m->number));
		}
		else {
			// delay message unil it is acknowledged
			// the delay is proportional to the number of nodes
			m->wait = t + s;
			printf("message %d delayed until t=%d by node %d\n", m->number, m->wait, pick);
		}

		tx++;

		printf("\n");
		printnodes(nodes, s);

		// select next node to execute
		pick = -1;
		again = 1;
		while (again && (pick < 0))
		{
			again = 0;
			t++;

			for (i=0; (i < s) && (pick < 0); i++)
			{
				if (nodes[i].state == NODE_ACTIVE)
				{
					for (j=0; (j<nodes[i].queue_len) && (pick < 0); j++)
					{
						again = 1; // there is at least one message in one queue

						if (t >= nodes[i].queue[j].wait)
						{
							pick = i;
							n_tx = &nodes[pick];
							m = &nodes[i].queue[j];
						}
					}
				}
			}
		}
	}
	
	return tx;
}

int alg1(struct node *nodes, char *net, size_t s)
{
	int pick, weight;
	struct node *n_tx, *n_rx;
	struct message *m;
	int tx = 0;
	int t = 0;

	// the gateway initiates the process sending a void message
	pick = 0;
	n_tx = &nodes[pick];
	m = &n_tx->queue[0];
	
	n_tx->state = NODE_ACTIVE;
	n_tx->queue_len++;
	m->number = 0;

	// execute one task from the picked client
	while (pick >= 0)
	{
		printf("\nt=%d\n", t);
		printf("node %d transmits message %d\n", pick, m->number);

		// the node trasmits his message
		for (int i=0; i<s; i++)
		{
			weight = *(net+pick*s+i);
			n_rx = &nodes[i];

			if (weight) // connected nodes receive the message
			{
				// activate waiting nodes
				if (n_rx->state != NODE_ACTIVE)
				{
					n_rx->state = NODE_ACTIVE;
					printf("node %d activated\n", i);
				}

				// update depths
				if ((n_rx->depth < 0) ||
					((n_tx->depth + 1) < n_rx->depth)) // better path
				{
					n_rx->depth = n_tx->depth + 1;
				}

				if (n_rx->depth == 0)
				{
					printf("message %d reached the destination\n", m->number);
				}
				// relay message if we are nearer to the gateway
				else if ((n_rx->depth < n_tx->depth) && (find(n_rx, m->number) < 0))
				{
					enqueue(n_rx, *m);
					printf("enqueued by node %d\n", i);
				}
			}
		}

		dequeue(n_tx, find(n_tx, m->number));
		printf("message %d dequeued from node %d\n", m->number, pick);

		tx++;
		t++;

		printf("\n");
		printnodes(nodes, s);

		// select next node to execute
		pick = -1;
		for (int i=0; (i < s) && (pick < 0); i++)
		{
			if (nodes[i].state == NODE_ACTIVE)
			{
				for (int j=0; (j<nodes[i].queue_len) && (pick < 0); j++)
				{
					pick = i;
					n_tx = &nodes[pick];
					m = &nodes[i].queue[j];
				}
			}
		}
	}
	
	return tx;
}

void randtopo(struct point *points, size_t n, int radius)
{
	int i, j, overlap, l = 2*radius+1;

	// the sink is always in the center
	points[0].x = 0;
	points[0].y = 0;

	for (i=1; i<n; i++)
	{
		do {
			overlap = 0;

			points[i].x = (rand() % l) - radius;
			points[i].y = (rand() % l) - radius;

			for (j=0; j<i; j++) if ((points[i].x == points[j].x) && (points[i].y == points[j].y)) overlap = 1;
		}
		while (overlap);
	}
}

void printtopo(struct point *points, size_t n, int radius)
{
	int i, j, k, found, l=2*radius+1;

	for (i=0; i<l; i++) // row
	{
		for (j=0; j<l; j++) // col
		{
			found = 0;
			for (k=0; k<n; k++)
			{
				if (points[k].x == (j-radius) && points[k].y == -(i-radius))
				{
					printf("%2d", k);
					found = 1;
				}
			}

			if (!found) printf(" -");
		}
		printf("\n");
	}
}

void plottopo(struct point *points, char *graph, size_t n, int r, char* filename)
{
	int i, j;
	FILE *p;

	p = popen("gnuplot", "w");

	fprintf(p, "set terminal pngcairo size 640, 640\n");
	fprintf(p, "set output '%s'\n", filename);
	fprintf(p, "set size square\n");
	fprintf(p, "set xr [%f:%f]\n", -r*1.1, r*1.1);
	fprintf(p, "set yr [%f:%f]\n", -r*1.1, r*1.1);
	fprintf(p, "set xtics %d\n", r/5);
	fprintf(p, "set ytics %d\n", r/5);

	fprintf(p, "$vertices << EOD\n");
	for (i=0; i<n; i++)
	{
		fprintf(p, "%d %d %d\n", i, points[i].x, points[i].y);
	}
	fprintf(p, "EOD\n");

	fprintf(p, "$arcs << EOD\n");
	for (i=0; i<n; i++) for (j=0; j<i; j++)
	{
		if (graph[i*n+j]) fprintf(p, "%d %d\n%d %d\n\n", points[i].x, points[i].y, points[j].x, points[j].y);
	}
	fprintf(p, "EOD\n");

	fprintf(p, "$weights << EOD\n");
	for (i=0; i<n; i++) for (j=0; j<i; j++)
	{
		if (graph[i*n+j]) fprintf(p, "%d %f %f\n", graph[i*n+j], (points[i].x + points[j].x) / 2.0, (points[i].y + points[j].y) / 2.0);
	}
	fprintf(p, "EOD\n");

	fprintf(p, "plot ");
	fprintf(p, "'$arcs' using 1:2 with lines lc rgb 'black' notitle, ");
	fprintf(p, "'$weights' using 2:3 with points ps 3.5 pt 7 lc rgb 'white' notitle, ");
	fprintf(p, "'$weights' using 2:3:1 with labels tc rgb 'black' notitle, ");
	fprintf(p, "'$vertices' using 2:3 with points ps 3.5 pt 7 lc rgb 'black' notitle, ");
	fprintf(p, "'$vertices' using 2:3:1 with labels tc rgb 'white' notitle\n");

	pclose(p);
}

int topo2graph(struct point *points, char *graph, size_t n, int range)
{
	int i, j, dist;

	for (i=0; i<n; i++) for (j=0; j<n; j++)
	{
		dist = sqrt(pow(points[i].x - points[j].x, 2) + pow(points[i].y - points[j].y, 2));
		graph[i*n+j] = graph[j*n+i] = (dist <= range) ? dist : 0;
	}
}

int connectedgraph(char *graph, size_t n)
{
	int i, connected = 1;
	char* visited = calloc(n, 1);

	visitgraph(graph, n, 0, visited);
	for (i=0; i<n; i++) if(!visited[i]) connected = 0;

	return connected;
}

void visitgraph(char* n, size_t s, int vertex, char* visited)
{
	visited[vertex] = 1;

	for (int i=0; i<s; i++)
	{
		// if vertex is connected to i and i has not been visited yet
		if (n[vertex*s+i] && (visited[i] == 0)) visitgraph(n, s, i, visited);
	}
}

void printgraph(char *n, size_t s)
{
	int v;

	for (int i=0; i<s; i++)
	{
		for (int j=0; j<s; j++)
		{
			v = *(n+i*s+j);

			if (v == 0) printf(" -");
			else printf("%2d", v);
		}

		printf("\n");
	}
}

void printnodes(struct node *nodes, size_t s)
{
	printf("     # ");
	for (int i=0; i<s; i++) printf("%2d ", i);

	printf("\ndepth: ");
	for (int i=0; i<s; i++)
	{
		if (nodes[i].depth < 0) printf(" ? ");
		else printf("%2d ", nodes[i].depth);
	}

	printf("\nchan.: ");
	for (int i=0; i<s; i++) printf("%2d ", nodes[i].channel);
	printf("\n");

	printf("\nqueue: ");
	for (int i=0; i<s; i++) printf("%2d ", nodes[i].queue_len);
	printf("\n");
}

int find(struct node *n, int number)
{
	for (int i=0; i < n->queue_len; i++)
	{
		if (n->queue[i].number == number) return i;
	}

	return -1;
}

void enqueue(struct node *n, struct message m)
{
	n->queue[n->queue_len] = m;
	n->queue_len++;
}

struct message dequeue(struct node *n, unsigned int pos)
{
	struct message m;

	m = n->queue[pos];
	n->queue_len--;
	for (int i=pos; i<n->queue_len; i++) n->queue[i] = n->queue[i+1];
	return m;
}