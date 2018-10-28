# set terminal pdf size 3.5in,3.5in font 'Times,16'

$vertices << EOD
0 0
0.70 0.70
1.41 1.41
2.12 2.12
2.83 2.83
0.70 -0.70
1.41 -1.41
2.12 -2.12
2.83 -2.83
-0.70 0.70
-1.41 1.41
-2.12 2.12
-2.83 2.83
-0.70 -0.70
-1.41 -1.41
-2.12 -2.12
-2.83 -2.83
EOD

unset tics
unset key
unset border

set size square
set xrange [-4:4]
set yrange [-4:4]

set object 1 circle at 0,0 size 4 fc rgb 'black'

set arrow 1 from 0,0 to 4,0

set object 2 circle at 2,0 front size char 1 fs solid fc rgb 'white'
set label 1 '{/:Italic r}' at 2,0 center front

set object 3 circle at 2.83,2.83 front size char 1 fs solid fc rgb 'white' back
set object 4 circle at -2.83,2.83 front size char 1 fs solid fc rgb 'white' back
set object 5 circle at 2.83,-2.83 front size char 1 fs solid fc rgb 'white' back
set object 6 circle at -2.83,-2.83 front size char 1 fs solid fc rgb 'white' back

plot '$vertices' pt 3 lc 'black'