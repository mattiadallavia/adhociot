# set terminal pdf size 8cm,6cm font 'Times,16' dl 1.5

$vertices << EOD
-4 2
-6 7
3 2
7 5
6 -1
-2 -5
-6 -2
EOD

$arcs << EOD
0 0
-4 2

0 0
3 2

0 0
-2 -5

-4 2
-6 7

3 2
7 5

3 2
6 -1

-4 2
-6 -2
EOD

$arcs_extra << EOD
6 -1
7 5

-6 -2
-2 -5
EOD

$depths << EOD
0 2
1 2
2 2
2 1
2 0
2 -1
2 -2
1 -2
0 -2
-1 -2
-2 -2
-2 -1
-2 0
-2 1
-2 2
-1 2
0 2

0 4
3 4
6 5
5 2
5 0
5 -2
0 -4
-2 -18
-7 -7
-4 -5
-4 -3
2 -2
-11 2
-6 3
-7 5
-4 4
-2 4
0 4

0 7
2 7
5 7
7 7
10 10
9 5
9 3
8 1
15 -1
8 -3
0 -7
-5 -25
-4 -7
-8 -4
-13 -2
-7 0
-7 2
-7 5
-8 7
-10 13
-6 9
-4 9
-2 7
0 7
EOD

unset tics
unset key
unset border
set style textbox opaque noborder margins 3,3

set xrange [-8:9]
set yrange [-7.5:9]

set label 1 '' at 0,0 point pt 7 ps 3 lc 'white' front
set label 2 '{/:Bold S}' at 0,0 front center

set label 3 '{/:Italic 0}' at 0,2 center boxed front
set label 4 '{/:Italic 1}' at 0,4 center boxed front
set label 5 '{/:Italic 2}' at 0,7 center boxed front

plot '$arcs' with lines lc 'black', \
     '$arcs_extra' with lines dt 2 lc 'black', \
     '$vertices' pt 7 ps 3 lc 'white', \
     '$vertices' pt 3 lc 'black', \
     '$depths' with lines smooth bezier dt 4 lc 'black'