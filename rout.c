#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define NODE_WAITING   0 // waiting to be activated by the reception of one message
#define NODE_ACTIVE    1 // ready to transmit

#define FRAME(T) (T / 3)
#define SLOT(T) (T % 3)
#define IN_RANGE(A, B, GRAPH, N) ((A != B) && (GRAPH[A*N+B] <= 1))

struct message
{
	int num;
	int num_ack;
	long ch_ack;
};

struct node
{
	int state;
	int depth;
	long ch;
	long wait;
	int att;
	int trans;
	struct message *mess;
	int mess_len;
};

struct stat
{
	long t;
	long tx;
	long rx;
	long coll;
};

void disp(struct stat *st, struct node *nodes, float *graph, int n);
struct message peek(struct node *n);
int find(struct node *n, int num);
void push(struct node *n, struct message m);
void delete(struct node *n, unsigned int pos);
void read_graph(float *graph, int n);
void print_nodes(struct node *nodes, int n);

static int flag_steps = 0;
static int flag_act = 0;
static int flag_coll = 0;
static int bfac = 5;
static float wfac = 30;

static struct option long_options[] =
{
    {"steps",      no_argument,       &flag_steps, 1},
    {"actions",    no_argument,       &flag_act,   1},
    {"collisions", no_argument,       &flag_coll,  1},
    {"wfactor",    required_argument, 0,         'w'},
    {"bfactor",    required_argument, 0,         'b'},
    {0, 0, 0, 0}
};

// usage: ./rout
//  --steps         print intermediate steps
//  --actions       print nodes actions
//  --collisions    enable collision detection and avoidance
//  --wfactor W     set wait factor
//  --bfactor B     set branching factor

int main(int argc, char **argv)
{
	int i, opt;
	int n, env, range, seed, conn;
	int dis, dis_t;
	struct stat st;
	float *graph;
	struct node *nodes;

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
	printf("%d\t%d\t%d\t%d\t%d\t%f\t%d\n", env, n, range, seed, conn, wfac, bfac);

	srand(seed);
	graph = malloc((n+1) * (n+1) * sizeof (float));
	nodes = calloc((n+1), sizeof (struct node));
	memset(&st, 0, sizeof (struct stat));

	read_graph(graph, n+1);

	// satellites initialization
	for (i = 0; i < (n+1); i++)
	{
		nodes[i].depth = -1;
		nodes[i].ch = -1;
		nodes[i].mess = malloc((n+1) * sizeof (struct message));
		nodes[i].mess_len = 1;
		nodes[i].mess[0].num = i;
		nodes[i].mess[0].num_ack = -1;
		nodes[i].mess[0].ch_ack = -1;
	}
	// the gateway initiates the process sending a void message
	nodes[0].state = NODE_ACTIVE;
	nodes[0].depth = 0;
	nodes[0].ch = 0;

	do
	{
		dis = dis_t = 0;

		// check every active node for mess to dispatch
		for (i = 0; i < (n+1); i++) if ((nodes[i].state == NODE_ACTIVE) &&
			                            (nodes[i].mess_len > 0))
		{
			dis++; // count nodes with mess to dispatch, now or in the future

			// if it is the right slot for this node
			// and is scheduled for this (or a past) time
			if (((!flag_coll) || (SLOT(st.t) == (2 - nodes[i].depth % 3))) &&
				(st.t >= nodes[i].wait))
			{
				dis_t++; // nodes with mess to dispatch during current t
				nodes[i].trans = 1;
			}
		}

		if (flag_steps && dis_t)
		{
			printf("t=%ld (frame %ld, slot %ld)\n", st.t, FRAME(st.t), SLOT(st.t));
			print_nodes(nodes, n+1);
		}

		disp(&st, nodes, graph, n+1);

		if (flag_steps && dis_t) printf("\n");

		// end current transmissions
		for (i = 0; i < (n+1); i++) nodes[i].trans = 0;

		st.t++; // all mess dispatched for t, go to t+1
	}
	while (dis);

	printf("%ld\t%ld\t%ld\t%ld\n", st.t, st.tx, st.rx, st.coll);
}

void disp(struct stat *st, struct node *nodes, float *graph, int n)
{
	int i, pos, coll;
	int i_tx, i_rx;
	float dist;
	struct node *n_tx, *n_rx;
	struct message m_tx, m_rx;

	// dispatch all transmissions
	for (i_tx = 0; i_tx < n; i_tx++) if (nodes[i_tx].trans)
	{
		n_tx = &nodes[i_tx];
		m_tx = peek(n_tx);
		m_rx.num = m_tx.num;
		m_rx.num_ack = i_tx;
		m_rx.ch_ack = n_tx->ch;

		if (flag_act) printf("node %d: transmitting mess. %d on ch. %ld\n",
		                     i_tx, m_tx.num, n_tx->ch);

		// transmit to all connected nodes
		// node who are trans cannot receive at the same time
		for (i_rx = 0; i_rx < n; i_rx++) if (IN_RANGE(i_tx, i_rx, graph, n) &&
		                                     !nodes[i_rx].trans)
		{
			n_rx = &nodes[i_rx];
			dist = graph[i_tx+n*i_rx];

			// collision detection
			coll = 0;
			if (flag_coll) for (i = 0; i < n; i++)
			{
				// is there anyone else connected to me trans
				// on the same ch at the same time?
				if (i != i_tx &&
					nodes[i].trans &&
					nodes[i].ch == n_tx->ch &&
					IN_RANGE(i, i_rx, graph, n))
				{
					coll++;
					st->coll++;
					if (flag_act) printf("node %d: coll. between mess. %d from %d and "
						                 "mess. %d from %d\n",
						                 i_rx, m_tx.num, i_tx,
						                 peek(&nodes[i]).num, i);
				}
			}
			// if a collision happened the message is not received
			if (coll) continue;

			// activate waiting nodes
			if (n_rx->state == NODE_WAITING)
			{
				n_rx->state = NODE_ACTIVE;
				// wait a frame proportional to the distance
				if (flag_coll) n_rx->wait = st->t + (int)wfac*dist;
				if (flag_act) printf("node %d: activated, tx. sched. for t=%ld\n",
				                     i_rx, n_rx->wait);
			}

			// update depths
			if (n_rx->depth < 0 ||
			    n_tx->depth + 1 < n_rx->depth) // better path
			{
				n_rx->depth = n_tx->depth + 1;
				n_rx->ch = -1;
			}

			// choose ch
			// not initialized or p-1 node changed ch (or overflow)
			if (flag_coll &&
				n_tx->depth == n_rx->depth-1 &&
				(n_rx->ch < 0 || n_rx->ch / bfac != n_tx->ch))
			{
				n_rx->ch = bfac*n_tx->ch;
			}

			// select new ch if we receive an ack
			// for another node transmitted on our ch
			if (flag_coll &&
				n_tx->depth < n_rx->depth &&
				m_tx.num_ack != i_rx &&
				m_tx.ch_ack == n_rx->ch)
			{
				n_rx->ch++;
				if (flag_act) printf("node %d: ch. %ld in use by node %d, "
				                     "selects ch. %ld\n",
				                     i_rx, (n_rx->ch-1), m_tx.num_ack, n_rx->ch);
			}

			// ack for a message we sent
			if (n_tx->depth < n_rx->depth &&
				m_tx.num_ack == i_rx)
			{
				// reset the exp. backoff
				n_rx->att = 0;
				n_rx->wait = 0;
				if (flag_act) printf("node %d: received an ack, tx. resched. asap\n", i_rx);
			}

			// remove mess. from stack if we receive an ack from nearer the dest.
			// from us or another node, doesn't matter
			if (n_tx->depth < n_rx->depth &&
				(pos = find(n_rx, m_rx.num)) >= 0)
			{
				delete(n_rx, pos);
				if (flag_act) printf("node %d: removed mess. %d\n", i_rx, m_rx.num);
			}

			// relay message if we are nearer the gateway
			if (n_tx->depth > n_rx->depth)
			{
				// remove old instances of the same message
				if ((pos = find(n_rx, m_rx.num)) >= 0) delete(n_rx, pos);
				push(n_rx, m_rx);
				if (flag_act) printf("node %d: added mess. %d\n", i_rx, m_rx.num);
			}

			st->rx++;
		}

		// after trans
		if (i_tx == 0) // if sink node
		{
			// remove sent message
			delete(n_tx, find(n_tx, m_tx.num));
		}
		else
		{
			// delay transmissions unil sent message is acknowledged
			n_tx->att++;
			// exponential backoff
			n_tx->wait = st->t + 3 * ((rand() % (int)pow(2, n_tx->att)) + 1);
			if (flag_act) printf("node %d: resched. att. %d at t=%ld\n",
			                     i_tx, (n_tx->att+1), n_tx->wait);
		}

		st->tx++;
	}
}

struct message peek(struct node *n)
{
	return n->mess[n->mess_len - 1];
}

int find(struct node *n, int num)
{
	int i;

	for (i = 0; i < n->mess_len; i++)
	{
		if (n->mess[i].num == num) return i;
	}

	return -1;
}

void push(struct node *n, struct message m)
{
	n->mess[n->mess_len] = m;
	n->mess_len++;
}

void delete(struct node *n, unsigned int pos)
{
	int i;

	n->mess_len--;
	for (i = pos; i < n->mess_len; i++) n->mess[i] = n->mess[i+1];
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
		if (nodes[i].trans) printf("  * ");
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
			if (nodes[i].ch < 0) printf("    ");
			else if (nodes[i].ch > 999) printf(" >> ");
			else printf("%3ld ", nodes[i].ch);
		}
	}

	printf("\nmess.: ");
	for (i = 0; i < n; i++) printf("%3d ", nodes[i].mess_len);
	printf("\n");
}