# ./stat 0 1000 50 0 20 1 1000 100 > stat.dat
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

set cntrparam levels discrete 100, 500, 900
splot 'stat.dat' using 1:3:2 with lines

# 4
set xlabel 'nodes'

set cntrparam levels discrete 5, 10, 15
splot 'stat.dat' using 2:3:1 with lines

unset multiplot
reset
