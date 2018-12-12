# set terminal pdf size 4cm,4cm font 'Times,14'

$vertices << EOD
0 0
0 2
2 0
2 2
2 -2
-2 0
0 -2
-2 -2
-2 2
0 4
4 0
0 -4
-4 0
2 4
4 2
2 -4
-2 4
-4 -2
4 -2
-4 2
-2 -4
EOD

unset tics
unset key
unset border
set xrange [-5:5]
set yrange [-5:5]

set object 1 circle at 0,0 size 5 fc 'black'

plot '$vertices' pt 3 lc 'black'