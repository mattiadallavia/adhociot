# set terminal pdf size 8cm,3.5cm font 'Times,14'

$vertices << EOD
-8 4
-4 5
-1 4
-6 1
-2 0
-8 -1
-5 -2

0 -3
3 -2
7 -3
4 2
7 1
10 2
8 4
2 4
EOD

$arcs << EOD
-5 3
-8 4

-5 3
-4 5

-4 5
-1 4

-5 3
-6 1

-5 3
-2 0

-6 1
-8 -1

-6 1
-5 -2


5 -1
3 -2

5 -1
7 -3

3 -2
0 -3

5 -1
4 2

5 -1
7 1

7 1
10 2

7 1
8 4

4 2
2 4
EOD

$sinks << EOD
-5 3
5 -1
EOD

$path << EOD
-10 0
-5 3
5 -1
9 -0.5
10 0
EOD

unset tics
unset key
unset border
set xrange [-10:10]
set yrange [-3:5]
set style textbox opaque noborder margin 1,1
set style arrow 1 head filled size screen 0.04,15,45

set arrow 1 from 9,-0.5 to 10,0 arrowstyle 1

plot '$path' with lines smooth csplines dt 4 lc 'black', \
     '$arcs' with lines lc 'black', \
     '$vertices' pt 7 ps 2 lc 'white', \
     '$vertices' pt 3 lc 'black', \
     '$sinks' pt 7 ps 2 lc 'white', \
     '$sinks' pt 13 lc 'black'
