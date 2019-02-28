#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int col_bundle;
char buf[1000]; // line buffer
int buf_cur = 0;
int buf_len = 0;
float row[100]; // line data
int row_len = 0;
float bundle_prev = -1;

// usage: ./bund a

int main(int argc, char **argv)
{
	int i, r;
	char c;

	col_bundle = atoi(argv[1]) - 1;

	while (1)
	{
		while ((r = read(0, buf + buf_len, 1)) > 0) // read line
		{
			c = buf[buf_len];

			if (isspace(c) && (buf_len > 0))
			{
				// buf[buf_len] = 0; // tokenize
				row[row_len++] = atof(buf + buf_cur);
				buf_cur = buf_len + 1;
			}

			if (c == '\n') break;

			buf_len++;
		}

		buf[buf_len] = 0; // cap

		if (r == 0) return 0;

		if ((row[col_bundle] != bundle_prev) && (bundle_prev >= 0)) // bundle col value change
		{
			printf("\n");
		}

		printf("%s\n", buf);

		bundle_prev = row[col_bundle];
		buf_len = 0;
		buf_cur = 0;
		row_len = 0;
	}
}
