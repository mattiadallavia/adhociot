#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define NODE_WAITING   0 // waiting to be activated by the reception of one packet
#define NODE_ACTIVE    1 // ready to transmit message or to relay other client's messages

#define FRAME(T) (T / 3)
#define SLOT(T) (T % 3)
#define IN_RANGE(A, B, GRAPH, N) ((A != B) && (GRAPH[A*N+B] <= 1))

struct message
{
	int number;
	int node_confirm;
	long channel_confirm;
};

struct node
{
	int state;
	int depth;
	long channel;
	long wait;
	int attempts;
	int transmitting;
	struct message *messages;
	int messages_len;
};

struct statistics
{
	long t;
	long tx;
	long rx;
	long collisions;
};

struct statistics alg(struct node *nodes, float *graph, int n);
struct message peek(struct node *n);
int find(struct node *n, int number);
void push(struct node *n, struct message m);
void delete(struct node *n, unsigned int pos);
void read_graph(float *graph, int n);
void print_nodes(struct node *nodes, int n);

static int flag_steps = 0;
static int flag_coll = 0;
static int bfac = 5;
static float wfac = 3;

static struct option long_options[] =
{
    {"steps",      no_argument,       &flag_steps, 1},
    {"collisions", no_argument,       &flag_coll,  1},
    {"wfactor",    required_argument, 0,         'w'},
    {"bfactor",    required_argument, 0,         'b'},
    {0, 0, 0, 0}
};

// usage: ./sim
//  --steps         print intermediate steps
//  --collisions    enable collision detection and avoidance

int main(int argc, char **argv)
{
	int i, opt;
	int n, env, range, seed, conn;
	float *graph;
	struct node *nodes;
	struct statistics stats;

	// optional arguments
	while ((opt = getopt_long(argc, argv, "w:b:", long_options, 0)) != -1)
	{
		switch (opt)
		{
			case 'w':
				wfac = atof(optarg);
				break;
			case 'b':
				bfac = atoi(optarg);
				break;
			case '?':
				return 1;
		}
	}

	scanf("%d\t%d\t%d\t%d\t%d\n", &env, &n, &range, &seed, &conn);
	printf("%d\t%d\t%d\t%d\t%d\t%f\n", env, n, range, seed, conn, wfac);

	srand(seed);
	graph = malloc((n+1) * (n+1) * sizeof (float));
	nodes = calloc((n+1), sizeof (struct node));

	read_graph(graph, n+1);

	// satellites initialization
	for (i = 0; i < (n+1); i++)
	{
		nodes[i].depth = -1;
		nodes[i].channel = -1;
		nodes[i].messages = malloc((n+1) * sizeof (struct message));
		nodes[i].messages_len = 1;
		nodes[i].messages[0].number = i;
		nodes[i].messages[0].node_confirm = -1;
		nodes[i].messages[0].channel_confirm = -1;
	}
	// the gateway initiates the process sending a void message
	nodes[0].state = NODE_ACTIVE;
	nodes[0].depth = 0;
	nodes[0].channel = 0;

	stats = alg(nodes, graph, n+1);

	printf("%ld\t%ld\t%ld\t%ld\n", stats.t, stats.tx, stats.rx, stats.collisions);
}

struct statistics alg(struct node *nodes, float *graph, int n)
{
	int i, pos, coll, disp, disp_t;
	int i_tx, i_rx;
	float dist;
	struct node *n_tx, *n_rx;
	struct message m_tx, m_rx;
	struct statistics stats;
	memset(&stats, 0, sizeof (struct statistics));

	while (1) // time loop
	{
		disp = 0;
		disp_t = 0;

		// check every active node for messages to dispatch
		for (i = 0; i < n; i++) if ((nodes[i].state == NODE_ACTIVE) && (nodes[i].messages_len > 0))
		{
			disp++; // count nodes with messages to dispatch, now or in the future

			// if it is the right slot for this node
			// and is scheduled for this (or a past) time
			if (((!flag_coll) || (SLOT(stats.t) == SLOT(nodes[i].depth))) &&
				(stats.t >= nodes[i].wait))
			{
				disp_t++; // messages to dispatch in this t
				nodes[i].transmitting = 1;
			}
		}

		if (disp == 0) return stats; // there are no more messages to dispatch
		if (disp_t == 0) // no mess. to disp. in this t
		{
			stats.t++;
			continue;
		}

		if (flag_steps)
		{
			printf("t=%ld (frame %ld, slot %ld)\n", stats.t, FRAME(stats.t), SLOT(stats.t));
			print_nodes(nodes, n);
		}

		// dispatch all transmissions in t
		for (i_tx = 0; i_tx < n; i_tx++) if (nodes[i_tx].transmitting)
		{
			n_tx = &nodes[i_tx];
			m_tx = peek(n_tx);

			m_rx.number = m_tx.number;
			m_rx.node_confirm = i_tx;
			m_rx.channel_confirm = n_tx->channel;

			if (flag_steps) printf("node %d transmits message %d on channel %ld\n", i_tx, m_tx.number, n_tx->channel);

			// transmit to all connected nodes
			// node who are transmitting cannot receive at the same time
			for (i_rx = 0; i_rx < n; i_rx++) if (IN_RANGE(i_tx, i_rx, graph, n) && !nodes[i_rx].transmitting)
			{
				n_rx = &nodes[i_rx];
				dist = graph[i_tx+n*i_rx];

				// collision detection
				coll = 0;
				if (flag_coll) for (i = 0; i < n; i++)
				{
					// is there anyone else connected to me transmitting on the same channel at the same time?
					if (i != i_tx &&
						nodes[i].transmitting &&
						nodes[i].channel == n_tx->channel &&
						IN_RANGE(i, i_rx, graph, n))
					{
						coll++;
						stats.collisions++;
						if (flag_steps) printf("collision with message %d from node %d at node %d\n", peek(&nodes[i]).number, i, i_rx);
					}
				}
				// if a collision happened the message is not received
				if (coll) continue;

				// activate waiting nodes
				if (n_rx->state == NODE_WAITING)
				{
					n_rx->state = NODE_ACTIVE;
					if (flag_coll) n_rx->wait = stats.t + (int) (wfac*dist*10); // wait a frame proportional to the distance
					if (flag_steps) printf("node %d activated, transmission scheduled for t=%ld\n", i_rx, n_rx->wait);
				}

				// update depths
				if ((n_rx->depth < 0) || ((n_tx->depth + 1) < n_rx->depth)) // better path
				{
					n_rx->depth = n_tx->depth + 1;
					n_rx->channel = -1;
				}

				// choose channel
				if (flag_coll &&
					n_tx->depth < n_rx->depth &&
					(n_rx->channel < 0 || n_rx->channel / bfac != n_tx->channel)) // p-1 node changed channel (or overflow)
				{
					n_rx->channel = bfac*n_tx->channel;
					if (flag_steps) printf("node %d selects channel %ld\n", i_rx, n_rx->channel);
				}

				// select new channel if we receive an ack for another node transmitted on our channel
				if (flag_coll && (n_tx->depth < n_rx->depth) && (m_tx.node_confirm != i_rx) && (m_tx.channel_confirm == n_rx->channel))
				{
					n_rx->channel++;
					if (flag_steps) printf("channel %ld in use by node %d, node %d selects channel %ld\n", (n_rx->channel-1), m_tx.node_confirm, i_rx, n_rx->channel);
				}

				// ack for a message we sent
				if ((n_tx->depth < n_rx->depth) && (m_tx.node_confirm == i_rx))
				{
					// reset the exp. backoff
					n_rx->attempts = 0;
					n_rx->wait = 0;
					if (flag_steps) printf("node %d received an ack, transmission scheduled for t=%ld\n", i_rx, n_rx->wait);
				}

				// remove message from stack if we receive an ack from nearer the destination
				// from us or another node, doesn't matter
				if ((n_tx->depth < n_rx->depth) && ((pos = find(n_rx, m_rx.number)) >= 0))
				{
					delete(n_rx, pos);
					if (flag_steps) printf("message %d removed from node %d\n", m_rx.number, i_rx);
				}

				// relay message if we are nearer the gateway
				if ((n_tx->depth > n_rx->depth))
				{
					// remove old instances of the same message
					if ((pos = find(n_rx, m_rx.number)) >= 0) delete(n_rx, pos);

					push(n_rx, m_rx);
					if (flag_steps) printf("added by node %d\n", i_rx);
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
				n_tx->wait = stats.t + rand() % (int)(wfac*pow(2, n_tx->attempts)) + 1; // exponential backoff
				if (flag_steps) printf("node %d scheduled attempt n. %d at t=%ld\n", i_tx, (n_tx->attempts+1), n_tx->wait);
			}

			stats.tx++;
		}
		// end current transmissions
		for (i = 0; i < n; i++) nodes[i].transmitting = 0;

		stats.t++; // all messages dispatched for t, go to t+1
		if (flag_steps) printf("\n");
	}
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

void read_graph(float *graph, int n)
{
	int i;

	for (i = 0; i < n*n; i++) scanf("%f", &graph[i]);
}

void print_nodes(struct node *nodes, int n)
{
	int i;

	printf("     # ");
	for (i = 0; i < n; i++) printf("%3d ", i);

	printf("\ntran.: ");
	for (i = 0; i < n; i++)
	{
		if (nodes[i].transmitting) printf("  * ");
		else printf("    ");
	}

	printf("\ndepth: ");
	for (i = 0; i < n; i++)
	{
		if (nodes[i].depth < 0) printf("    ");
		else printf("%3d ", nodes[i].depth);
	}

	if (flag_coll)
	{
		printf("\nchan.: ");
		for (i = 0; i < n; i++)
		{
			if (nodes[i].channel < 0) printf("    ");
			else if (nodes[i].channel > 999) printf(" >> ");
			else printf("%3ld ", nodes[i].channel);
		}
	}

	printf("\nmess.: ");
	for (i = 0; i < n; i++) printf("%3d ", nodes[i].messages_len);
	printf("\n");
}