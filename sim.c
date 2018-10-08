#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define NODE_WAITING   0 // waiting to be activated by the reception of one packet
#define NODE_ACTIVE    1 // ready to transmit message or to relay other client's messages

#define STACK_MAX 100
#define MAX_ATTEMPTS 1000000

#define FRAME(t) (t / 3)
#define SLOT(t) (t % 3)

struct point
{
	int x;
	int y;
};

struct message
{
	int number;
	int node_confirm;
	int channel_confirm;
};

struct node
{
	int state;
	int depth;
	int channel;
	int wait;
	int attempts;
	int transmitting;
	struct message messages[STACK_MAX];
	int messages_len;
};

struct statistics
{
	int t;
	int tx;
	int rx;
	int collisions;
};

struct statistics alg(struct node *nodes, char *net, size_t s);

struct message peek(struct node *n);
int find(struct node *n, int number);
void push(struct node *n, struct message m);
void delete(struct node *n, unsigned int pos);

void randtopo(struct point *points, size_t n, int radius);
int topo2graph(struct point *points, char *graph, size_t n, int range);
int connectedgraph(char *graph, size_t n);
void visitgraph(char* n, size_t s, int vertex, char* visited);

void printgraph(char *graph, size_t n);
void printtopo(struct point *points, size_t n, int radius);
void printnodes(struct node *nodes, size_t s);
void plottopo(struct point *points, char *graph, size_t n, int radius, char *plot_filename);

int main(int argc, char *argv[])
{
	int c, nodes_number, conn, radius, range, attempts = 0;
	char* plot_filename = 0;
	struct statistics stats;
	struct point *points;
	struct node *nodes;
	char* graph;

	srand(time(0));

	// mandatory arguments
	if (argc < 4)
	{
		fprintf(stderr, "usage: %s <number of nodes> <radius of grid> <range> [-p filename.png]\n", argv[0]);
		return 1;
	}

	nodes_number = atoi(argv[1]);
	radius = atoi(argv[2]);
	range = atoi(argv[3]);

	// optional arguments
	while ((c = getopt(argc-3, &argv[3], "p:")) != -1)
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
		nodes[i].depth = -1;
		nodes[i].messages_len = 1;
		nodes[i].messages[0].number = i;
		nodes[i].messages[0].node_confirm = -1;
		nodes[i].messages[0].channel_confirm = -1;
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

	stats = alg(nodes, graph, nodes_number);

	printf("\nfinal time: t=%d\n", stats.t);
	printf("messages transmitted: %d\n", stats.tx);
	printf("messages received: %d\n", stats.rx);
	printf("collisions: %d\n", stats.collisions);
}

struct statistics alg(struct node *nodes, char *net, size_t s)
{
	int i, weight, pos, coll, disp;
	int i_tx, i_rx;
	struct node *n_tx, *n_rx;
	struct message m_tx, m_rx;
	struct statistics stats;

	memset(&stats, 0, sizeof (struct statistics));

	// the gateway initiates the process sending a void message
	nodes[0].state = NODE_ACTIVE;
	nodes[0].messages_len++;
	nodes[0].messages[0].number = 0;
	nodes[0].messages[0].node_confirm = -1;
	nodes[0].messages[0].channel_confirm = -1;

	while (1) // time loop
	{
		disp = 0;

		// check every active node for messages to dispatch
		for (i = 0; i < s; i++) if ((nodes[i].state == NODE_ACTIVE) && (nodes[i].messages_len > 0))
		{
			// count nodes with messages to dispatch, now or in the future
			disp++;

			// if it is the right slot for this node
			// and is scheduled for this (or a past) frame
			if ((SLOT(stats.t) == SLOT(nodes[i].depth)) && (FRAME(stats.t) >= nodes[i].wait))
			{
				nodes[i].transmitting = 1;
			}
		}

		// there are no more messages to dispatch
		if (disp == 0) return stats;

		printf("\nt=%d (frame %d, slot %d)\n\n", stats.t, FRAME(stats.t), SLOT(stats.t));
		printnodes(nodes, s);

		// dispatch all transmissions in t
		for (i_tx = 0; i_tx < s; i_tx++) if (nodes[i_tx].transmitting)
		{
			n_tx = &nodes[i_tx];
			m_tx = peek(n_tx);

			m_rx.number = m_tx.number;
			m_rx.node_confirm = i_tx;
			m_rx.channel_confirm = n_tx->channel;

			printf("\nnode %d transmits message %d on channel %d\n", i_tx, m_tx.number, n_tx->channel);

			// transmit to all connected nodes
			// node who are transmitting cannot receive at the same time
			for (i_rx = 0; i_rx < s; i_rx++) if ((weight = net[i_tx*s+i_rx]) && !nodes[i_rx].transmitting)
			{
				n_rx = &nodes[i_rx];

				// collision detection
				coll = 0;
				for (i = 0; i < s; i++)
				{
					// is there anyone else connected to me transmitting on the same channel at the same time?
					if ((i != i_tx) && net[i_rx*s+i] && nodes[i].transmitting && (nodes[i].channel == n_tx->channel))
					{
						coll = 1;
						stats.collisions++;
						printf("collision with message %d from node %d at node %d\n", peek(&nodes[i]).number, i, i_rx);
					}
				}

				// if a collision happened the message is not received
				if (coll) continue;

				// update depths
				if ((n_rx->depth < 0) || ((n_tx->depth + 1) < n_rx->depth)) // better path
				{
					n_rx->depth = n_tx->depth + 1;
				}

				// activate waiting nodes
				if (n_rx->state == NODE_WAITING)
				{
					n_rx->state = NODE_ACTIVE;
					n_rx->wait = FRAME(stats.t); // todo: wait a frame proportional to the distance
					printf("node %d activated at depth %d, transmission scheduled for frame %d\n", i_rx, n_rx->depth, n_rx->wait);
				}

				// select new channel if we receive an ack for another node transmitted on our channel
				if ((n_tx->depth < n_rx->depth) && (m_tx.node_confirm != i_rx) && (m_tx.channel_confirm == n_rx->channel))
				{
					n_rx->channel++;
					printf("channel %d in use by node %d, node %d selects channel %d\n", (n_rx->channel-1), m_tx.node_confirm, i_rx, n_rx->channel);
				}

				// ack for a message we sent
				if ((n_tx->depth < n_rx->depth) && (m_tx.node_confirm == i_rx))
				{
					// reset the exp. backoff
					n_rx->attempts = 0;
					n_rx->wait = FRAME(stats.t);
					printf("node %d received an ack, transmission scheduled for frame %d\n", i_rx, n_rx->wait);
				}

				// remove message from stack if we receive an ack from nearer the destination
				// from us or another node, doesn't matter
				if ((n_tx->depth < n_rx->depth) && ((pos = find(n_rx, m_rx.number)) >= 0))
				{
					delete(n_rx, pos);
					printf("message %d removed from node %d\n", m_rx.number, i_rx);
				}

				// relay message if we are nearer the gateway
				if ((n_tx->depth > n_rx->depth))
				{
					// remove old instances of the same message
					if ((pos = find(n_rx, m_rx.number)) >= 0) delete(n_rx, pos);

					push(n_rx, m_rx);
					printf("added by node %d\n", i_rx);
				}

				stats.rx++;
			}

			// after transmitting
			if (i_tx == 0) // if sink node
			{
				// remove sent message
				delete(n_tx, find(n_tx, m_tx.number));
			}
			else
			{
				// delay transmissions unil sent message is acknowledged
				n_tx->attempts++;
				n_tx->wait = FRAME(stats.t) + (rand() % (int)pow(2, n_tx->attempts)) + 1; // exponential backoff
				printf("node %d scheduled attempt n. %d at frame %d\n", i_tx, (n_tx->attempts+1), n_tx->wait);
			}

			stats.tx++;
		}
		// end current transmissions
		for (i = 0; i < s; i++) nodes[i].transmitting = 0;

		stats.t++; // all messages dispatched for t, go to t+1
	}
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

	printf("\ntran.: ");
	for (int i=0; i<s; i++)
	{
		if (nodes[i].transmitting) printf(" * ");
		else printf("   ");
	}

	printf("\ndepth: ");
	for (int i=0; i<s; i++)
	{
		if (nodes[i].depth < 0) printf("   ");
		else printf("%2d ", nodes[i].depth);
	}

	printf("\nchan.: ");
	for (int i=0; i<s; i++) printf("%2d ", nodes[i].channel);

	printf("\nmess.: ");
	for (int i=0; i<s; i++) printf("%2d ", nodes[i].messages_len);
	printf("\n");
}

struct message peek(struct node *n)
{
	return n->messages[n->messages_len - 1];
}

int find(struct node *n, int number)
{
	int i;

	for (i = 0; i < n->messages_len; i++)
	{
		if (n->messages[i].number == number) return i;
	}

	return -1;
}

void push(struct node *n, struct message m)
{
	n->messages[n->messages_len] = m;
	n->messages_len++;
}

void delete(struct node *n, unsigned int pos)
{
	int i;

	n->messages_len--;
	for (i = pos; i < n->messages_len; i++) n->messages[i] = n->messages[i+1];
}