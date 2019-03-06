#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct node
{
	int number;
	float lat;
	float lon;
};

int geodist(float lat1, float lon1, float lat2, float lon2);

struct node nodes[5000];
int nodes_len = 0;
int range;

// mean number of nodes in the direct range of each node, including itself
// ./den stations.dat 10000 | cut -f 1 | uniq -c | sed 's/^ *//' | ./st --a 1 --m --var

// number of nodes covered versus how many nodes have in range that number of nodes
// distribution of the coverage count
// ./den stations.dat 10000 | cut -f 1 | uniq -c | sed 's/^ *//' | cut -d ' ' -f 1 | sort -n | uniq -c | sed 's/^ *//'

int main(int argc, char **argv)
{
	int i, j, d;
	char *nodes_filename;
	FILE *nodesin;

	nodes_filename = argv[1];
	range = atoi(argv[2]);

	nodesin = fopen(nodes_filename, "r");

	while (EOF != fscanf(nodesin, "%d\t%f\t%f", &nodes[nodes_len].number,
	                                            &nodes[nodes_len].lat,
	                                            &nodes[nodes_len].lon))
	{
		nodes_len++;
	}

	for (i = 0; i < nodes_len; i++) for (j = 0; j < nodes_len; j++)
	{
		// including itself
		if ((d = geodist(nodes[j].lat, nodes[j].lon, nodes[i].lat, nodes[i].lon)) <= range)
		{
			printf("%d\t%d\t%d\n", i, j, d);
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