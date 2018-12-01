# set terminal pdf size 10cm,3cm font 'Times,16'

unset tics
unset key
unset border
set xrange [-1:10]
set yrange [0:2]
set xtics 0,1,9 axis
set ytics 1 scale 0 format ''
set style arrow 1 head filled size screen 0.03,15,45

set for [i = 0:8] label (i+1) sprintf("%d",i%3) at (i+0.5),0.5 center
set for [i = 0:2] label (i+10) sprintf("%d",i) at (i*3+1.5),1.5 center

set label 13 '{/:Italic slot}' at -0.3,0.5 right
set label 14 '{/:Italic frame}' at -0.3,1.5 right

set arrow 1 from 0,0 to 10,0 arrowstyle 1

set arrow 2 from 0,1 to 9,1 nohead dt 3
set arrow 3 from 0,2 to 9,2 nohead dt 3
set arrow 4 from 0,0 to 0,2 nohead dt 3
set arrow 5 from 1,0 to 1,1 nohead dt 3
set arrow 6 from 2,0 to 2,1 nohead dt 3
set arrow 7 from 3,0 to 3,2 nohead dt 3
set arrow 8 from 4,0 to 4,1 nohead dt 3
set arrow 9 from 5,0 to 5,1 nohead dt 3
set arrow 10 from 6,0 to 6,2 nohead dt 3
set arrow 11 from 7,0 to 7,1 nohead dt 3
set arrow 12 from 8,0 to 8,1 nohead dt 3
set arrow 13 from 9,0 to 9,2 nohead dt 3

plot NaN