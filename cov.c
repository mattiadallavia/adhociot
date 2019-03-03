#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NODE_WAITING   0 // waiting to be activated by the reception of one message
#define NODE_ACTIVE    1 // ready to transmit

struct node
{
	int number;
	int state;
	float lat;
	float lon;
};

struct sink
{
	int number;
	float lat;
	float lon;
};

int geodist(float lat1, float lon1, float lat2, float lon2);

struct node nodes[5000];
int nodes_len = 0;

int main(int argc, char **argv)
{
	int i, r, range;
	char *nodes_filename, *sinks_filename;
	FILE *nodesin, *sinksin;
	struct sink s;

	nodes_filename = argv[1];
	sinks_filename = argv[2];
	range = atoi(argv[3]);

	nodesin = fopen(nodes_filename, "r");
	sinksin = fopen(sinks_filename, "r");

	while (EOF != fscanf(nodesin, "%d\t%f\t%f", &nodes[nodes_len].number,
	                                            &nodes[nodes_len].lat,
	                                            &nodes[nodes_len].lon))
	{
		nodes_len++;
	}

	while (EOF != (r = fscanf(sinksin, "%d\t%f\t%f\t%*s\t%*s", &s.number, &s.lat, &s.lon)))
	{
		for (i = 0; i < nodes_len; i++)
		{
			if ((nodes[i].state == NODE_WAITING) && geodist(s.lat, s.lon, nodes[i].lat, nodes[i].lon) <= range)
			{
				nodes[i].state = NODE_ACTIVE;
				printf("%d\t%f\t%f\n", nodes[i].number, nodes[i].lat, nodes[i].lon);
			}
		}
	}
}

// distance between two coordiantes in meters
int geodist(float lat1, float lon1, float lat2, float lon2)
{
	double lat1r, lon1r, lat2r, lon2r;

	lat1r = lat1 * M_PI / 180.0;
	lon1r = lon1 * M_PI / 180.0;
	lat2r = lat2 * M_PI / 180.0;
	lon2r = lon2 * M_PI / 180.0;

	return 637279.50 * 20.0 * asin(sqrt((sin((lat2r - lat1r)/2.0)*sin((lat2r - lat1r)/2.0)) + 
			                  cos(lat1r)*cos(lat2r)*(sin((lon2r-lon1r)/2.0)*sin((lon2r-lon1r)/2.0))));             
 }