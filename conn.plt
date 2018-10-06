# ./stat 10 100 5 0.5 7 0.25 1000 1000 > stat.dat
# set terminal pngcairo size 2400,2400 fontscale 3 linewidth 3

set multiplot layout 2, 2
unset key

# 1
set xlabel 'gain'
set ylabel 'nodes'

splot 'stat.dat' with lines

# 2
set contour
set view map
set grid
unset surface

set cntrparam levels discrete 0.1, 0.5, 0.9
splot 'stat.dat' with lines

# 3
unset ylabel
set ytics 0.1

set cntrparam levels discrete 20, 50, 80
splot 'stat.dat' using 1:3:2 with lines

# 4
set xlabel 'nodes'

set cntrparam levels discrete 2, 3, 4, 5
splot 'stat.dat' using 2:3:1 with lines

unset multiplot
reset
