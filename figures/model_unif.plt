# set terminal pdf size 3.5in,3.5in font 'Times,16'

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

set size square
set xrange [-5:5]
set yrange [-5:5]

set object 1 circle at 0,0 size 5 fc rgb 'black'

set arrow 1 from 0,0 to 5*sqrt(2)/2,(-5*sqrt(2)/2)

set object 2 circle at 2*sqrt(2)/2,(-2*sqrt(2)/2) front size char 1 fs solid fc rgb 'white'
set label 1 '{/:Italic r}' at 2*sqrt(2)/2,(-2*sqrt(2)/2) center front

plot '$vertices' pt 3 lc 'black'