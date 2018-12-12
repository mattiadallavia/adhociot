# ./st 1 2 --a 3 --m < conn.dat > conn.aggr.dat
# set terminal pdf size 12cm,6cm font 'Times,14'

unset key

set multiplot layout 1,2

# tf
set xlabel 'd'
set ylabel 'n' offset 7
set zlabel 'c' offset 1,0
set ytics 200
set ztics 0.2
set view 60, 350, 1.2, 0.8

splot 'conn.aggr.dat' using 1:2:($3/$2) with lines lc 'black'

# retx
set xlabel 'd' offset 0,0.5
set ylabel 'c' offset 0,0
set contour
set view map
unset surface
set xtics 2
set ytics autofreq

set label 1 'n = 100' at 5.6,0.70 rotate by -84
set label 2 'n = 500' at 11.1,0.70 rotate by -84
set label 3 'n = 900' at 14.7,0.70 rotate by -82

set cntrparam levels discrete 100, 500, 900
splot 'conn.aggr.dat' using 1:($3/$2):2 with lines

unset multiplot
reset