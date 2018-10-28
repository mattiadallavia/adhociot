set multiplot layout 2, 2
unset key

# 1
set xlabel 'gain'
set ylabel 'nodes'

splot 'stat.dat' using 1:2:4 with lines

# 2
set contour
set view map
set grid
unset surface

set cntrparam levels discrete 0.1, 0.5, 0.9
splot 'stat.dat' using 1:2:4 with lines

# 3
unset ylabel
set ytics 0.1

set cntrparam levels discrete 20, 50, 80
splot 'stat.dat' using 1:4:2 with lines

# 4
set xlabel 'nodes'

set cntrparam levels discrete 2, 3, 4, 5
splot 'stat.dat' using 2:4:1 with lines

unset multiplot
reset