#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct node
{
	int number;
	int visited;
	float lat;
	float lon;
};

struct sink
{
	int number;
	float lat;
	float lon;
};

void visit(int index, float lat, float lon, int depth);
int geodist(float lat1, float lon1, float lat2, float lon2);

struct node nodes[5000];
int nodes_len = 0;
int range;
int depth_max;
char date_str_curr[20];

// only northern italy stations
// awk '{ if ( $2 >= 44.5 ) { print } }' stations.dat > stations_north.dat

// sort trucks by date
// sort -k 4,4 -k 1,1 trucks.dat -o trucks.day.dat

// nodes visited each day
// cut -f 1 stations.day.dat | uniq -c

// nodes visited a certain number of days
// cut -f 2 stations.day.dat | sort | uniq -c | sed 's/^ *//' | cut -d ' ' -f 1 | sort | uniq -c

int main(int argc, char **argv)
{
	int i, r;
	char *nodes_filename, *sinks_filename;
	FILE *nodesin, *sinksin;
	struct sink s;
	char date_str[20];

	nodes_filename = argv[1];
	sinks_filename = argv[2];
	range = atoi(argv[3]);
	depth_max = atoi(argv[4]);

	nodesin = fopen(nodes_filename, "r");
	sinksin = fopen(sinks_filename, "r");

	while (EOF != fscanf(nodesin, "%d\t%f\t%f", &nodes[nodes_len].number,
	                                            &nodes[nodes_len].lat,
	                                            &nodes[nodes_len].lon))
	{
		nodes_len++;
	}

	while (EOF != (r = fscanf(sinksin, "%d\t%f\t%f\t%s\t%*s", &s.number, &s.lat, &s.lon, date_str)))
	{
		// date change
		if (strcmp(date_str, date_str_curr) != 0)
		{
			for (i = 0; i < nodes_len; i++) nodes[i].visited = 0;
			strcpy(date_str_curr, date_str);
		}

		visit(-1, s.lat, s.lon, 0);
	}
}

void visit(int index, float lat, float lon, int depth)
{
	int i;

	if (index != -1) // if not a sink
	{
		nodes[index].visited = 1;
		printf("%s\t%d\t%f\t%f\t%d\n", date_str_curr, nodes[index].number, nodes[index].lat, nodes[index].lon, depth);
	}

	if (depth < depth_max) for (i = 0; i < nodes_len; i++)
	{
		if ((index != i) && !nodes[i].visited && (geodist(lat, lon, nodes[i].lat, nodes[i].lon) <= range))
		{
			visit(i, nodes[i].lat, nodes[i].lon, depth + 1);
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